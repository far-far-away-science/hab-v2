/*
 * nmea.c - GPS NMEA parser for determining latitude and longitude from standard GPS data
 * Also stores the current GPS time (subtly different from UTC!)
 */

#include <comm.h>
#include <nmea.h>
#include <string.h>

// Number of GPS positions allocated
#define N_GPS 2
// The maximum number of characters expected in a parseable GPS line
#define GPS_MAX_LEN 64

// GPS state machine for stream reading
typedef struct {
	uint32_t len;
	char line[GPS_MAX_LEN];
} GPSState;

static GPSState gpsState[2];

// Tokenizes a string, like strtok(), but re-entrantly; the delimiter is always ','
static uint32_t tokenize(char **string, char *token, uint32_t length) {
	char *str = *string; uint32_t count = 0;
	// Iterate until token hit or end of string
	while (*str && *str != ',' && count < length) {
		*token++ = *str++;
		count++;
	}
	// Skip the token and null terminate
	if (*str) str++;
	*token = '\0';
	*string = str;
	return count;
}

// Parses a number from the null-terminated buffer. Does not perform conversion of GPS
// coordinates into decimal degrees, since the APRS system sends the coordinates in ddmm.mmmm
// format
static int32_t parse(char *buffer, int32_t precision) {
	int32_t value = 0, decimalPos = -1024; char c;
	while (*buffer) {
		c = *buffer++;
		if (c >= '0' && c <= '9') {
			value = (value * 10) + (int32_t)(c - '0');
			decimalPos++;
		} else if (c == '.')
			// Decimal point found
			decimalPos = 0;
	}
	// If no decimal point found, value has zero digits after
	if (decimalPos < 0) decimalPos = 0;
	// If precision > decimal point index, multiply by 10
	while (precision > decimalPos) {
		value *= 10;
		decimalPos++;
	}
	// If precision < decimal point index, divide by 10
	while (precision < decimalPos) {
		value /= 10;
		decimalPos--;
	}
	return value;
}

// Handles an incoming character from the specified GPS index, returning true if a line has
// been completed
bool gpsCharHandler(char c, const uint32_t index) {
	bool endLine = false;
	// Determine which GPS is being used
	GPSState * const state = &gpsState[index];
	uint32_t len = state->len;
	if (len == 0U) {
		// Wait for start
		if (c == '$') {
			state->line[0] = c;
			len = 1U;
		}
	} else {
		// Increment length
		if (len >= (GPS_MAX_LEN - 1U) || c == '\r' || c == '\n') {
			// END OF LINE!
			state->line[len] = '\0';
			len = 0U;
			endLine = true;
		} else {
			state->line[len] = c;
			len++;
		}
	}
	state->len = len;
	return endLine;
}

// Reports true if the location is within the bounds of rationality (inside the continental US)
bool gpsCoordsOK(const GPSLocation *data) {
	const int32_t lat = data->latitude, lon = data->longitude;
	// 23.658174-49.703529 deg N, 64.204870-128.800619 deg W
	return (lat > 23394904 && lat < 49422117) && (lon > -128480371 && lon < -64122922);
}

// Parses a complete GPS line of information from the buffer and updates the GPS state
// accordingly. The GPS buffer line MUST be null-terminated. Reports true if coordinates were
// parsed, or false otherwise
bool gpsParse(GPSLocation *data, const uint32_t index) {
	int32_t lat, lon, alt;
	bool parsed = false;
	// Determine which GPS is being used
	GPSState * const state = &gpsState[index];
	// Buffer is stored in global variable
	char * const buffer = &(state->line[0]);
	char *str = &buffer[7], gpt[32];
	if (strncmp(buffer, "$GPGGA", 6) == 0) {
		// Time
		tokenize(&str, gpt, sizeof(gpt));
		data->gpsTime = parse(gpt, 0);
		// Latitude
		tokenize(&str, gpt, sizeof(gpt));
		lat = parse(gpt, 4);
		// N/S
		tokenize(&str, gpt, sizeof(gpt));
		if (gpt[0] == 'S') lat = -lat;
		data->latitude = lat;
		// Longitude
		tokenize(&str, gpt, sizeof(gpt));
		lon = parse(gpt, 4);
		// E/W
		tokenize(&str, gpt, sizeof(gpt));
		if (gpt[0] == 'W') lon = -lon;
		data->longitude = lon;
		// Fix?
		tokenize(&str, gpt, sizeof(gpt));
		if (gpt[0] == '1' || gpt[0] == '2') {
			// GPS satellites in use
			tokenize(&str, gpt, sizeof(gpt));
			if (gpt[0]) {
				data->satellites = (uint8_t)parse(gpt, 0);
				// Horizontal dilution of precision
				tokenize(&str, gpt, sizeof(gpt));
				// Altitude in meters, min 0 m
				tokenize(&str, gpt, sizeof(gpt));
				alt = parse(gpt, 0);
				if (alt < 0) alt = 0;
				data->altitude = (uint32_t)alt;
				parsed = true;
			}
		}
	} else if (strncmp(buffer, "$GPVTG", 6) == 0) {
		// Heading (true)
		tokenize(&str, gpt, sizeof(gpt));
		data->heading = (uint16_t)parse(gpt, 0);
		// 'T'
		tokenize(&str, gpt, sizeof(gpt));
		// Not used
		tokenize(&str, gpt, sizeof(gpt));
		// Not used
		tokenize(&str, gpt, sizeof(gpt));
		// Speed over ground
		tokenize(&str, gpt, sizeof(gpt));
		data->speed = (uint8_t)parse(gpt, 0);
		// Do not set parsed to true
		// Checks and copies to the true position should only occur on position update
	}
	return parsed;
}
