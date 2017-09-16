/*
 * packets.c - functions related to composing and sending packets
 */

#include <main.h>
#include <adxl345.h>
#include <aprs.h>
#include <bme280.h>
#include <lora.h>
#include <onewire.h>
#include <packets.h>
#include <printf.h>
#include <stmtime.h>

// LoRA packet to be sent
static LoRAData loraData;

#ifdef PHOENIX
// 1-wire temperature sensor addresses (should all end with 0x28)
// Since searching is a pain and error-prone, better to hook them up 1 by 1 and code it in
static const uint64_t tempAddr[] = { 0xA50316A14D8EFF28U, 0xB00416A12F3DFF28U };
#endif

// Loads the system time from the time indicated in GPS messages
void loadGPSTime(const uint32_t flags, uint32_t time) {
	// If time needs to be updated
	if (!(flags & FLAG_TIME_SET)) {
		// Divide down into hhmmss, keep non-const for later call to addTime
		uint32_t hour = time / 10000U, minute;
		time -= hour * 10000U;
		minute = time / 100U;
		time -= minute * 100U;
		// If time is not 00:00:00
		if (hour || minute || time) {
			setTime(hour, minute, time, 2017U, 1U, 1U);
			// Time has been updated
			setFlags(FLAG_TIME_SET);
			// Arm PI shutdown if enabled, rounds to nearest minute
#if PI_SHUTDOWN > 0
			addTime(&hour, &minute, PI_SHUTDOWN / 60U);
			setAlarmTo(hour, minute, 0U);
#endif
		}
	}
}

// Logs the telemetry data to the UART port, along with both (use array!) GPS locations
void logData(const GPSLocation *location, const TelemetryData *data) {
	// Obtain components as needed
	const UTCTime *ts = &(data->timestamp);
	const GPSLocation *gps1 = &location[0], *gps2 = &location[1];
	// Generate time
	const unsigned int time = (UI(ts->hour) * 10000U) + (UI(ts->minute) * 100U) +
		UI(ts->second);
	int32_t x, y, z;
	// Time, GPS locations
	printf("%06d,%d,%d,%u,%u,%d,%d,%u,%u,", time, SI(gps1->latitude), SI(gps1->longitude),
		UI(gps1->altitude), UI(gps1->satellites), SI(gps2->latitude), SI(gps2->longitude),
		UI(gps2->altitude), UI(gps2->satellites));
	// Temperature, power, pressure
	printf("%d,%d,%d,%u,%u", SI(data->cpuTemp), SI(data->ambientTemp[0]),
		SI(data->ambientTemp[1]), UI(data->powerLevel), UI(data->pressure));
	// Acceleration
	if (accelRead(&x, &y, &z)) {
		// Scale to mG, 3.9 per LSB
		x = (x * 39 + 5) / 10;
		y = (y * 39 + 5) / 10;
		z = (z * 39 + 5) / 10;
	} else {
		x = 0;
		y = 0;
		z = 0;
	}
	printf("%d,%d,%d\r\n", SI(x), SI(y), SI(z));
}

// Sends a LoRA message with the specified data
// More details on the format can be found in the LoraMap project
void loRASend(const GPSLocation *location, const TelemetryData *data) {
	// Cannot use local member as that would be trashed mid-transmit
	loraData.latitude = location->latitude;
	loraData.longitude = location->longitude;
	loraData.altitude = (uint16_t)location->altitude;
	loraData.powerLevel = (uint16_t)data->powerLevel;
	loraData.ambientTemp = (uint16_t)aprsConvertTemp(data->cpuTemp);
	loraData.pressure = (uint16_t)((data->pressure + 5U) / 10U);
	loRAStartTransmit(&loraData, sizeof(loraData));
}

#ifndef DUMP_ID
// Converts the temperature as read from the DS18B20 into a value in (degrees C * 100)
static int32_t oneWireConvertTemp(int16_t value) {
#ifdef DS18B20
	return ((int32_t)value * 100 + 8) >> 4;
#else
	return ((int32_t)value * 100 + 8) >> 1;
#endif
}
#endif

#ifdef PHOENIX
// Reads the 1-wire temperature sensor data
static INLINE void sensorReadTemp(TelemetryData *data) {
	feedWatchdog();
#ifdef PARASITE_POWER
	ioSetDirection(PIN_OW_TEMP, DDR_OUTPUT_OD);
#endif
	// Read 1-wire temperature sensor data
	oneWireReset();
#ifdef DUMP_ID
	uint8_t address[8];
	// To get a device address, TX 0x33 and RX 8 bytes
	oneWireWrite(0x33U);
	// Read address 8 bytes
	oneWireReadBytes(&address, 8U);
	for (uint32_t i = 8U; i; i--)
		printf("%02X", address[i - 1U]);
	puts("\r");
	oneWireReset();
	data->ambientTemp[0] = 0U;
	data->ambientTemp[1] = 0U;
#elif defined(SINGLE_SENSOR)
	// All nodes
	oneWireWrite(0xCCU);
	// Read Scratchpad 2 bytes
	oneWireWrite(0xBEU);
	oneWireReadBytes(&temp, 2U);
	// End read
	oneWireReset();
	// Temperature is in degrees C * 16
	data->ambientTemp[0] = oneWireConvertTemp(temp);
	data->ambientTemp[1] = 0U;
#else
	int16_t temp;
	for (uint32_t i = 0U; i < 2U; i++) {
		// Match ROM address
		oneWireWrite(0x55);
		oneWireWriteBytes(&tempAddr[i], 8U);
		// Read Scratchpad 2 bytes
		oneWireWrite(0xBEU);
		oneWireReadBytes(&temp, 2U);
		// End read
		oneWireReset();
		// Temperature is in degrees C * 16
		data->ambientTemp[i] = oneWireConvertTemp(temp);
	}
#endif
}
#endif

// Populates the APRS packet with the correct telemetry information
void populatePacket(TelemetryData *data) {
	uint32_t seq = (uint32_t)data->sequence, hh, mm, ss, humid, pres;
	int32_t temp;
	UTCTime *time = &(data->timestamp);
	// Retrieve the time from the RTC
	getTime(&hh, &mm, &ss);
	readDate();
	time->hour = (uint16_t)hh;
	time->minute = (uint8_t)mm;
	time->second = (uint8_t)ss;
	// Data from BME280 (instead of CPU temperature which is pretty inaccurate)
	//  Temperature
	bme280Read(&temp, &humid, &pres);
	data->cpuTemp = temp;
	//  Ambient temperature from One-Wire sensors
	sensorReadTemp(data);
	//  Pressure in Pa
	data->pressure = pres;
	readPowerLevel(data);
	// Add one to sequence number
	if (seq >= 999U)
		seq = 1U;
	else
		seq++;
	data->sequence = (uint16_t)seq;
}

// Reads and stores the power level in the telemetry data
uint32_t readPowerLevel(TelemetryData *data) {
	// Battery: 430K / 100K divider makes ADC value 0.189 * true voltage
	//  4095 = 3.3V so voltage in decivolts (is this a thing?) = x * 174.9 / 4095
	//  Account for 16x oversampling by shifting an additional 4 places
	uint32_t power = ((uint32_t)adcResults[ADC_IDX_BAT] * 175U + 32768U) >> 16;
	if (power > 999U)
		power = 999U;
	if (data)
		data->powerLevel = power;
	return power;
}
