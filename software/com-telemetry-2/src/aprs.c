/*
 * aprs.c - APRS packet generator
 */

#include <main.h>
#include <aprs.h>
#include <printf.h>

// Stores the APRS packet under construction
#define APRS_PACKET_LEN 128
static char aprsPacket[APRS_PACKET_LEN];

// A version of memcpy that optionally shifts the copied data left one bit, byte by byte
static void * copyData(void *dest, const void *source, uint32_t length, bool shift) {
	uint8_t *ret = (uint8_t *)dest, *src = (uint8_t *)source;
	uint32_t data;
	for (; length; length--) {
		// Shift left if requested
		data = (uint32_t)*src++;
		if (shift)
			data <<= 1;
		*ret++ = (uint8_t)data;
	}
	return ret;
}

// Prepares the proper headers, including callsigns, in the buffer, returning the number of
// bytes added
static uint32_t aprsBuildHeader() {
	char *data = &aprsPacket[0];
	// Append final destination, then source, then the repeater (destination #2)
	data = copyData(data, APRS_CALLSIGN_DEST_1, APRS_CALLSIGN_LENGTH, true);
	*data++ = APRS_FLAGBITS_DEST_1;
	data = copyData(data, APRS_CALLSIGN_SRC, APRS_CALLSIGN_LENGTH, true);
	*data++ = APRS_FLAGBITS_SRC;
	data = copyData(data, APRS_CALLSIGN_DEST_2, APRS_CALLSIGN_LENGTH, true);
	*data++ = APRS_FLAGBITS_DEST_2;
	// Append control and ID fields
	*data++ = (char)APRS_FIELD_CTRL;
	*data++ = (char)APRS_FIELD_PROTO;
	return 3 * (APRS_CALLSIGN_LENGTH + 1) + 2;
}

// Sends an APRS packet with the specified telemetry values
void aprsSend(UTCTime *time, GPSLocation *location, TelemetryData *data) {
	uint32_t size = aprsBuildHeader();
	char *packet = &aprsPacket[size];
	unsigned int alt = 0U;
	// Add time in UTC, character / is for timestamped reports without APRS messaging
	sprintf(packet, "/%02u%02u%02uh", (unsigned int)time->hour,
		(unsigned int)time->minute, (unsigned int)time->second);
	packet += APRS_TIME_LENGTH;
	if (location == NULL) {
		// Unknown position, use correct symbol
		copyData(packet, "0000.00N\\00000.00E.000/000", APRS_LOCATION_LENGTH +
			APRS_CSE_LENGTH, false);
		packet += APRS_LOCATION_LENGTH;
	} else {
		unsigned int absLon, absLat;
		int32_t lon = location->longitude, lat = location->latitude;
		// Calculate unsigned versions of lat and lon
		if (lon < 0)
			absLon = (unsigned int)-lon;
		else
			absLon = (unsigned int)lon;
		if (lat < 0)
			absLat = (unsigned int)-lat;
		else
			absLat = (unsigned int)lat;
		alt = location->altitude;
		// Add GPS coordinates to packet using balloon symbol again
		sprintf(packet, "%04u.%02u%c/%05u.%02u%cO", absLat / 100U, absLat % 100U,
			(lat < 0) ? 'S' : 'N', absLon / 100U, absLon % 100U, (lon < 0) ? 'W' : 'E');
		packet += APRS_LOCATION_LENGTH;
		// Course and speed in degrees and knots
		sprintf(packet, "%03u/%03u", (unsigned int)location->heading,
			(unsigned int)location->speed);
	}
	packet += APRS_CSE_LENGTH;
	// Append telemetry packet and update size
	size += sprintf(packet, "T#%03u,%03u,%03u,%03u,000,000,00000000/a=%06u",
		(unsigned int)data->sequence, (unsigned int)data->powerLevel,
		(unsigned int)data->ambientTemp, (unsigned int)data->cpuTemp, alt) +
		APRS_CSE_LENGTH + APRS_LOCATION_LENGTH + APRS_TIME_LENGTH;
	// Output the audio stream minus zero terminator!
	audioPlay(aprsPacket, size - 1U);
}
