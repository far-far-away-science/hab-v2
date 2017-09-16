/*
 * nmea.h - GPS NMEA parser for determining latitude and longitude from standard GPS data
 * Also stores the current GPS time (subtly different from UTC!)
 */

#ifndef NMEA_H_
#define NMEA_H_

#include <aprs.h>
#include <cortex.h>
#include <stdint.h>
#include <string.h>

// Convenience routine to copy GPS coordinate information from one variable to another
static INLINE void gpsCoordsCopy(GPSLocation *out, const GPSLocation *in) {
	memcpy(out, in, sizeof(GPSLocation));
}

// Handles an incoming character from the specified GPS index, returning true if a line has
// been completed
bool gpsCharHandler(char c, uint32_t index);
// Reports true if the location is within the bounds of rationality (inside the continental US)
bool gpsCoordsOK(const GPSLocation *data);
// Parses a complete GPS line of information from the buffer and updates the GPS state
// accordingly. The GPS buffer line MUST be null-terminated. Reports true if coordinates were
// parsed, or false otherwise
bool gpsParse(GPSLocation *data, uint32_t index);

#endif
