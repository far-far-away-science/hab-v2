/*
 * packets.h - Packet declarations used when transmitting data over LoRA and APRS
 */

#ifndef PACKETS_H_
#define PACKETS_H_

#include <stdint.h>

// UTC time structure (obtained from GPS / RTC)
typedef struct {
	// Hour 00-23
	uint16_t hour;
	// Minute 00-59
	uint8_t minute;
	// Second 00-59
	uint8_t second;
} UTCTime;

// GPS location structure
typedef struct {
	// Latitude in signed DDMM.MMMM
	int32_t latitude;
	// Longitude in signed DDDMM.MMMM
	int32_t longitude;
	// Course made good in degrees 0-359
	uint16_t heading;
	// Speed in knots
	uint8_t speed;
	// Satellites visible
	uint8_t satellites;
	// Altitude in meters
	uint32_t altitude;
	// GPS time in seconds
	uint32_t gpsTime;
} GPSLocation;

// Telemetry data to be sent to APRS
typedef struct {
	// Timestamp of the telemetry packet
	UTCTime timestamp;
	// Packet sequence number 0-999
	uint32_t sequence;
	// Main battery voltage in volts * 10
	uint32_t powerLevel;
	// Ambient temperature in (degrees C * 100)
	int32_t ambientTemp[2];
	// CPU temperature using the same formula
	int32_t cpuTemp;
	// Ambient pressure in Pa
	uint32_t pressure;
} TelemetryData;

// Telemetry data to be sent to LoRA
typedef struct {
	// GPS latitude in signed DDMM.MMMM
	int32_t latitude;
	// GPS longitude in signed DDMM.MMMM
	int32_t longitude;
	// Altitude in meters
	uint16_t altitude;
	// Main battery voltage in volts * 10
	uint16_t powerLevel;
	// Ambient temperature in (degrees C * 10) + offset
	uint16_t ambientTemp;
	// Pressure in Pa / 10
	uint16_t pressure;
} LoRAData;

// Initializes the packet to be sent over APRS
static INLINE void initPacket(TelemetryData *data) {
	data->sequence = 0U;
}

// Resets the location information to its initial state
static INLINE void initLocation(GPSLocation *location) {
	location->latitude = 0;
	location->longitude = 0;
	location->altitude = 0U;
	location->heading = 0U;
	location->speed = 0U;
}

// Loads the system time from the time indicated in GPS messages
void loadGPSTime(const uint32_t flags, uint32_t time);
// Logs the telemetry data to the UART port, along with both (use array!) GPS locations
void logData(const GPSLocation *location, const TelemetryData *data);
// Sends a LoRA message with the specified data
// More details on the format can be found in the LoraMap project
void loRASend(const GPSLocation *location, const TelemetryData *data);
// Populates the APRS packet with the correct telemetry information
void populatePacket(TelemetryData *data);
// Reads and stores the power level in the telemetry data
uint32_t readPowerLevel(TelemetryData *data);

#endif
