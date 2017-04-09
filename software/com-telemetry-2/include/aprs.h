/*
 * aprs.h - APRS definitions for sending telemetry packets over the Amateur Packet Reporting
 * System (144.390 MHz); the standard format will allow repeating
 */

#ifndef APRS_H_
#define APRS_H_

#include "callsign.h"
#include <dac.h>

// Control field is 0x03 (UI frame)
#define APRS_FIELD_CTRL 0x03
// Protocol ID is 0xF0 (none)
#define APRS_FIELD_PROTO 0xF0

// Length of APRS call signs excluding flag bits
#define APRS_CALLSIGN_LENGTH 6
// Length of APRS course/speed field
#define APRS_CSE_LENGTH 7
// Length of APRS latitude field
#define APRS_LATITUDE_LENGTH 8
// Length of APRS longitude field
#define APRS_LONGITUDE_LENGTH 9
// Length of APRS location field
#define APRS_LOCATION_LENGTH (APRS_LATITUDE_LENGTH + APRS_LONGITUDE_LENGTH + 2)
// Length of APRS time field
#define APRS_TIME_LENGTH 8
// Length of APRS telemetry field
#define APRS_TELEM_LENGTH 42

// Last character is a flags byte
// Bits 7-5 are always 1, 4-1 are the SSID, 0 is true for last address or false otherwise
#define APRS_FLAGS(_ssid, _last) ((char)(((_last) ? 0x1 : 0x0) | (((_ssid) & 0xF) << 1) | 0xE0))

// Callsigns to send the APRS packet - last character is a flags byte
// WIDE1-1
#define APRS_FLAGBITS_DEST_1 APRS_FLAGS(1, 0)
#define APRS_CALLSIGN_DEST_1 "WIDE1 "
// WIDE2-2
#define APRS_FLAGBITS_DEST_2 APRS_FLAGS(2, 1)
#define APRS_CALLSIGN_DEST_2 "WIDE2 "
// Our balloon callsign (balloon = -11)
#define APRS_FLAGBITS_SRC    APRS_FLAGS(11, 0)

// UTC time structure (obtained from GPS / RTC)
typedef struct {
	// Hour 00-23
	uint32_t hour;
	// Minute 00-59
	uint32_t minute;
	// Second 00-59
	uint32_t second;
} UTCTime;

// GPS location structure
typedef struct {
	// Latitude in signed DDMM.MM
	int32_t latitude;
	// Longitude in signed DDDMM.MM
	int32_t longitude;
	// Course made good in degrees 0-359
	uint16_t heading;
	// Speed in knots
	uint16_t speed;
	// Altitude in ft
	uint32_t altitude;
} GPSLocation;

// Temperature offset added to reported temperature
#define APRS_TEMP_OFFSET 500

// Telemetry data to be sent to APRS
typedef struct {
	// Packet sequence number 0-999
	uint32_t sequence;
	// Main battery voltage in volts * 10
	uint32_t powerLevel;
	// Ambient temperature in (degrees C * 10) + offset: 0 C = 500, -50 C = 0, +49.9 C = 999
	uint32_t ambientTemp[2];
	// CPU temperature using the same formula
	uint32_t cpuTemp;
} TelemetryData;

// Sends an APRS packet with the specified telemetry values
void aprsSend(UTCTime *time, GPSLocation *location, TelemetryData *data);

#endif
