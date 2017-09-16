/*
 * main.c - Telemetry main code
 */

#include <main.h>
#include <periph.h>
#include <adxl345.h>
#include <bkup.h>
#include <bme280.h>
#include <lora.h>
#include <nmea.h>
#include <onewire.h>
#include <packets.h>
#include <stmtime.h>

// Telemetry data and GPS locations gathered from sensors
static TelemetryData data;
static GPSLocation gpsLoc[2];
static GPSLocation location;

#ifdef HABV2
// Selects the specified SPI device from SPI_DEVICE_* in main.h
void spiSelect(uint32_t device) {
	ioSetOutput(PIN_LATCH_A0, (device & 0x1U) != 0U);
	ioSetOutput(PIN_LATCH_A1, (device & 0x2U) != 0U);
	ioSetOutput(PIN_LATCH_A2, (device & 0x4U) != 0U);
	// Pulse the latch enable to lock in the pin selection
	ioSetOutput(PIN_LATCH_CS, false);
	__dsb();
	ioSetOutput(PIN_LATCH_CS, true);
}
#endif

#if defined(PHOENIX) && defined(DS18B20)
// Configuration byte sequence for the DS18B20 series sensors
static const uint8_t oneWireConfig[] = { 0xCCU, 0x4EU, 0x00U, 0xFFU, 0x7FU };
#endif

// Blinks the diagnostic LED to indicate failures; assumes status is not zero
static INLINE void diagBlink(uint32_t status) {
	// On red for 1 second before blink
	setLED(65535U, 0U, 0U);
	delay(100U);
	// Distinguish first blink from the initial status
	setLED(0U, 0U, 0U);
	delay(10U);
	for (uint32_t i = ERR_BLINK_BITS; i; i--) {
		// Look at status LSB; error bits are arranged in order in main.h with the LSB
		// being blinked out first
		if (status & 1U)
			setLED(65535U, 0U, 0U);
		else
			setLED(0U, 65535U, 0U);
		// LED on for 300ms (warning: total period needs to be less than WDT reset period)
		delay(30U);
		// All LEDs off for 100ms
		setLED(0U, 0U, 0U);
		delay(10U);
		// Show next bit
		status >>= 1;
	}
	setLED(32768U, 0U, 0U);
}

// Sets the specified flag and starts the SysTick timer to trigger a poll every 10ms
static INLINE void pollStop(uint32_t flag) {
	sysTickDisable();
	clearFlags(flag);
}

// Sets the specified flag and starts the SysTick timer to trigger a poll every 10ms
static INLINE void pollStart(uint32_t flag) {
	sysTickEnable();
	setFlags(flag);
}

// Handles a system tick (every 10ms) when the HX1 or LoRA radio needs to be serviced
static INLINE void handleTick(const uint32_t flags, const bool hx1Ready) {
	if ((flags & FLAG_HX1_WARMUP) != 0U && hx1Ready) {
		// HX1 is ready, compose and send APRS data
#ifdef DISABLE_RADIO
		pollStop(FLAG_HX1_WARMUP | FLAG_XMIT);
#else
		pollStop(FLAG_HX1_WARMUP);
#endif
		populatePacket(&data);
#ifndef DISABLE_RADIO
		aprsSend(&location, &data);
#endif
	} else {
#ifndef DISABLE_RADIO
		if ((flags & FLAG_LORA) != 0U && loRAGetTXStatus()) {
			// LoRA transmitted, enter standby
			pollStop(FLAG_LORA | FLAG_XMIT);
			loRAStandby();
			setLED(0U, 0U, 0U);
		}
#endif
	}
}

// Initializes the sensors, returning true if they were successfully brought up
static INLINE bool sensorInit(void) {
	uint32_t sensorStatus = 0U, power;
	// Read in power level
	adcStart();
	delay(10U);
	power = readPowerLevel(&data);
	// Check power level
	if (power < POWER_LOW)
		sensorStatus = ERR_POWER_CRIT;
	else if (power < POWER_OK)
		sensorStatus = ERR_POWER_LOW;
	// Bring up sensors
	if (!bme280Init())
		sensorStatus |= ERR_BME280;
	if (accelInit())
		accelResume();
	else
		sensorStatus |= ERR_ACCEL;
	// Configure LoRA radio power to maximum in standby mode
	if (loRAInit())
		loRASetPower(17U);
	else
		sensorStatus |= ERR_LORA;
#ifdef PHOENIX
	if (oneWireReset()) {
#ifdef DS18B20
		// Bring up 1-wire sensors by setting all to 12-bit mode
		oneWireWriteBytes(oneWireConfig, sizeof(oneWireConfig) / sizeof(uint8_t));
		oneWireReset();
#endif
	} else
		// Neither is responding
		sensorStatus |= ERR_TEMP_1 | ERR_TEMP_2;
#endif
	// Initialize packets
	initPacket(&data);
	initLocation(&location);
	initLocation(&gpsLoc[0]);
	initLocation(&gpsLoc[1]);
	// If good, set LED green; if not, turn on diagnostic blink
	if (sensorStatus == 0U)
		setLED(0U, 65535U, 0U);
	else if (sensorStatus == ERR_POWER_LOW)
		setLED(32767U, 65535U, 0U);
	else
		diagBlink(sensorStatus);
	// Turn on the Pi
	ioSetOutput(PIN_EN_PI, true);
	return sensorStatus == 0U;
}

// Starts measuring all sensor data for transmission
static INLINE void sensorStart(void) {
	// Start BME280 measurement
	bme280Measure();
#ifdef PHOENIX
	// All nodes: convert T (max 750 ms, DS18B20)
	oneWireReset();
	oneWireWrite(0xCCU);
	oneWireWrite(0x44U);
#ifdef PARASITE_POWER
	ioSetOutput(PIN_OW_TEMP, true);
	ioSetDirection(PIN_OW_TEMP, DDR_OUTPUT);
#endif
#endif
	adcStart();
}

// Main program
int main(void) {
#ifndef DISABLE_RADIO
	uint32_t aprsSendAt = 0U;
#endif
#ifdef DUAL_GPS
	bool primaryGPS = false;
#endif
	uint32_t gpsTimeout = GPS_TIMEOUT;
	// Turn on green LED if all sensors OK
	sensorInit();
	watchdogStart();
	// Initialize all packets
	while (1) {
		const uint32_t now = sysTime, flags = clearFlags(FLAG_ADC_READY | FLAG_SYSTICK |
			FLAG_HX1_ANY | FLAG_RTC_30S | FLAG_RTC_1S | FLAG_GPS_1_READY | FLAG_GPS_2_READY |
			FLAG_BUTTON | FLAG_RTC_ALARM);
#ifndef DISABLE_RADIO
		// Check the flags
		if ((flags & FLAG_HX1_ANY) != 0U && audioInterrupt(flags)) {
			// APRS data complete
			audioShutdown();
			// Start transmitting data from LoRA radio
			loRASend(&location, &data);
			// If LoRA gets stuck on, allow new transmission to start
			clearFlags(FLAG_XMIT);
			pollStart(FLAG_LORA);
		}
#endif
		if (flags & FLAG_SYSTICK)
			// HX1 warmup or LoRA radio check
			handleTick(flags, now >= aprsSendAt);
		if (flags & FLAG_RTC_1S) {
			// Log data every second
			logData(gpsLoc, &data);
			// Count down to disabling primary GPS if it stops sending new data
			if (gpsTimeout)
				gpsTimeout--;
		}
		if (flags & FLAG_RTC_ALARM) {
			// RTC alarm triggered, shut down the PI
			disableAlarm();
			ioSetOutput(PIN_EN_PI, false);
		}
		if ((flags & (FLAG_RTC_30S | FLAG_BUTTON)) != 0U && (flags & FLAG_XMIT) == 0U) {
			// Transmission start, only if not already transmitting
			setLED(0U, 0U, 32768U);
			sensorStart();
#ifndef DISABLE_RADIO
			// Power up the HX1
			audioInit();
#endif
			// Start APRS once HX1 is ready
			aprsSendAt = now + 60U;
			pollStart(FLAG_HX1_WARMUP | FLAG_XMIT);
		}
		if ((flags & FLAG_GPS_1_READY) != 0U && gpsParse(&gpsLoc[0], 0U)) {
			// Handle incoming GPS data on GPS #1
			loadGPSTime(flags, location.gpsTime);
			primaryGPS = gpsCoordsOK(&gpsLoc[0]);
			if (primaryGPS)
				gpsCoordsCopy(&location, &gpsLoc[0]);
			// Reset GPS timeout
			gpsTimeout = GPS_TIMEOUT;
		}
#ifdef DUAL_GPS
		if ((flags & FLAG_GPS_2_READY) != 0U && gpsParse(&gpsLoc[1], 1U)) {
			// Handle incoming GPS data on GPS #2
			loadGPSTime(flags, location.gpsTime);
			if ((!primaryGPS || !gpsTimeout) && gpsCoordsOK(&gpsLoc[1]))
				// If primary GPS is reporting bad data, or has said nothing for a while
				gpsCoordsCopy(&location, &gpsLoc[1]);
		}
#endif
		// Feed the watchdog
		feedWatchdog();
		__sleep();
	}
	return 0;
}
