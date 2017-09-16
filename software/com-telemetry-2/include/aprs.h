/*
 * aprs.h - APRS definitions for sending telemetry packets over the Amateur Packet Reporting
 * System (144.390 MHz); the standard format will allow repeating
 */

#ifndef APRS_H_
#define APRS_H_

#include "callsign.h"
#include <dac.h>
#include <packets.h>

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
// Temperature offset added to reported temperature
#define APRS_TEMP_OFFSET 500

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
// Our balloon callsign (balloon = 11)
#define APRS_FLAGBITS_SRC    APRS_FLAGS(11, 0)

// Converts a raw temperature in (degrees C * 100) to (degrees C * 10) + 500, from -50 C to 50 C
unsigned int aprsConvertTemp(int32_t temperature);
// Sends an APRS packet with the specified telemetry values
void aprsSend(const GPSLocation *location, const TelemetryData *data);

#endif
