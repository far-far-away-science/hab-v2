/*
 * aprs.c - APRS packet generator
 */

#include <main.h>
#include <dac.h>
#include <string.h>

// Control field is 0x03 (UI frame)
#define APRS_FIELD_CTRL 0x03
// Protocol ID is 0xF0 (none)
#define APRS_FIELD_PROTO 0xF0

// Stores the APRS packet under construction
#define APRS_PACKET_LEN 128
static char aprsPacket[APRS_PACKET_LEN];

// Callsigns to send the APRS packet - last character is a flags byte
// Bits 7-5 are always 1, 4-1 are the SSID, 0 is true for last address or false otherwise
#define APRS_CALLSIGN_LENGTH 7
static const char CALLSIGN_DEST_1[] = "WIDE1 \xE2";
static const char CALLSIGN_DEST_2[] = "WIDE2 \xE5";
static const char CALLSIGN_SRC[] =    "KI7LCY\xF6";

// Prepares the proper headers, including callsigns, in the buffer, returning the number of
// bytes added
static uint32_t aprsBuildHeader() {
	char *data = &aprsPacket[0];
	// Append final destination, then source, then the repeater (destination #2)
	memcpy(data, CALLSIGN_DEST_1, APRS_CALLSIGN_LENGTH);
	data += APRS_CALLSIGN_LENGTH;
	memcpy(data, CALLSIGN_SRC, APRS_CALLSIGN_LENGTH);
	data += APRS_CALLSIGN_LENGTH;
	memcpy(data, CALLSIGN_DEST_2, APRS_CALLSIGN_LENGTH);
	data += APRS_CALLSIGN_LENGTH;
	// Append control and ID fields
	*data++ = (char)APRS_FIELD_CTRL;
	*data++ = (char)APRS_FIELD_PROTO;
	return 3 * APRS_CALLSIGN_LENGTH + 2;
}

// Sends an APRS packet with the current telemetry values
void aprsSend() {
	uint32_t size = aprsBuildHeader();
	// Add 2 byte payload for testing
	aprsPacket[size++] = '@';
	aprsPacket[size++] = '1';
	audioPlay(aprsPacket, size);
}
