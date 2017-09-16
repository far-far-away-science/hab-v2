/*
 * aprs.c - APRS packet generator
 */

#include <main.h>
#include <aprs.h>
#include <printf.h>

// Stores the APRS packet under construction
#define APRS_PACKET_LEN 128
static char aprsPacket[APRS_PACKET_LEN];

// Converts a signed coordinate to its unsigned absolute value
static INLINE unsigned int absCoord(int32_t rawCoord) {
	unsigned int coord;
	if (rawCoord < 0)
		coord = UI(-rawCoord);
	else
		coord = UI(rawCoord);
	return coord;
}

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

// Converts a raw altitude from m to ft
static INLINE unsigned int aprsConvertAlt(uint32_t altitude) {
	// ~ 3.2808
	altitude = (altitude * 105U) >> 5;
	// Max 6 digits
	if (altitude > 999999U)
		altitude = 999999U;
	return UI(altitude);
}

// Converts the system flags byte into the status word for the digital telemetry APRS field
static unsigned int aprsConvertFlags(const uint32_t flags) {
	unsigned int word = 0U;
	// Cannot convert directly because result word can only have '1' or '0'
	if (flags & FLAG_CRASHED)
		word  = 0x10000000U;
	if (flags & FLAG_LSI)
		word += 0x01000000U;
	if (flags & FLAG_HSI)
		word += 0x00100000U;
	if (flags & FLAG_TIME_SET)
		word += 0x00010000U;
	return word;
}

// Converts the heading and speed, only coercing in range 0..999
static INLINE unsigned int aprsConvertHead(uint32_t heading) {
	if (heading > 999U)
		heading = 999U;
	return UI(heading);
}

// Converts a raw pressure from Pa to kPa
static INLINE unsigned int aprsConvertPres(uint32_t pressure) {
	pressure = (pressure + 500U) / 1000U;
	if (pressure > 999U)
		pressure = 999U;
	return UI(pressure);
}

// Converts a raw temperature in (degrees C * 100) to (degrees C * 10) + 500
unsigned int aprsConvertTemp(int32_t temperature) {
	temperature = ((temperature + 5) / 10) + APRS_TEMP_OFFSET;
	if (temperature < 0)
		temperature = 0;
	if (temperature > 999)
		temperature = 999;
	return UI(temperature);
}

// Sends an APRS packet with the specified telemetry values
void aprsSend(const GPSLocation *location, const TelemetryData *data) {
	unsigned int alt = 0U, size = aprsBuildHeader();
	const UTCTime *time = &(data->timestamp);
	char *packet = &aprsPacket[size];
	// Add time in UTC, character / is for timestamped reports without APRS messaging
	snprintf(packet, APRS_TIME_LENGTH + 1, "/%02u%02u%02uh", UI(time->hour), UI(time->minute),
		UI(time->second));
	packet += APRS_TIME_LENGTH;
	if (location == NULL || location->latitude == 0U) {
		// Unknown position, use correct symbol
		copyData(packet, "0000.00N\\00000.00E.000/000", APRS_LOCATION_LENGTH +
			APRS_CSE_LENGTH, false);
		packet += APRS_LOCATION_LENGTH;
	} else {
		unsigned int absLon, absLat;
		const int32_t lon = location->longitude, lat = location->latitude;
		// Calculate unsigned versions of lat and lon, round to nearest 100th of a minute by
		// taking off 2 decimal places
		absLon = absCoord((lon + 50) / 100);
		absLat = absCoord((lat + 50) / 100);
		// Altitude from m to ft
		alt = aprsConvertAlt(location->altitude);
		// Add GPS coordinates to packet using balloon symbol again
		snprintf(packet, APRS_LOCATION_LENGTH + 1, "%04u.%02u%c/%05u.%02u%cO", absLat / 100U,
			absLat % 100U, (lat < 0) ? 'S' : 'N', absLon / 100U, absLon % 100U,
			(lon < 0) ? 'W' : 'E');
		packet += APRS_LOCATION_LENGTH;
		// Course and speed in degrees and knots
		snprintf(packet, APRS_CSE_LENGTH + 1, "%03u/%03u", aprsConvertHead(location->heading),
			aprsConvertHead(location->speed));
	}
	packet += APRS_CSE_LENGTH;
	// Append telemetry packet and update size
	size += snprintf(packet, APRS_TELEM_LENGTH + 1, "T#%03u,%03u,%03u,%03u,%03u,%03u,%08X"
		"/a=%06u", UI(data->sequence), UI(data->powerLevel), aprsConvertTemp(data->cpuTemp),
		aprsConvertTemp(data->ambientTemp[0]), aprsConvertTemp(data->ambientTemp[1]),
		aprsConvertPres(data->pressure), aprsConvertFlags(sysFlags), alt) - 1;
	// Output the audio stream minus zero terminator!
	audioPlay(aprsPacket, size + APRS_CSE_LENGTH + APRS_LOCATION_LENGTH + APRS_TIME_LENGTH);
}
