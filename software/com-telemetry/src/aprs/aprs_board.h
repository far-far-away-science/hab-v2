#pragma once

#include <gps/nmea_messages.h>
#include <telemetry/telemetry.h>

#define MAX_APRS_MESSAGE_LENGTH          384
#define APRS_SIGNAL_GENERATION_FREQUENCY 100000

typedef struct Callsign_t
{
    const uint8_t callsign[7];
    const uint8_t ssid;
} Callsign;

typedef struct BitstreamSize_t
{
    uint16_t charsCount;
    uint8_t lastCharBitsCount;
} BitstreamSize;

typedef struct AprsEncodedMessage_t
{
    BitstreamSize size;
    uint8_t buffer[MAX_APRS_MESSAGE_LENGTH];
} AprsEncodedMessage;

extern const Callsign CALLSIGN_SOURCE;
extern const Callsign CALLSIGN_DESTINATION_1;
extern const Callsign CALLSIGN_DESTINATION_2;

bool isAprsMessageEmtpy(const AprsEncodedMessage* pMessage);

bool encodeGpsAprsMessage(const Callsign callsign, const GpsData* pGpsData, AprsEncodedMessage* pEncdedMessage);
bool encodeTelemetryAprsMessage(const Callsign callsign, const Telemetry* pTelemetry, AprsEncodedMessage* pEncdedMessage);

/*
 * Should fill full requested length of the buffer. If no data available add Zeroes.
 *
 * returns true if data was filled in
 */
bool fillInAmplitudesBuffer(const AprsEncodedMessage* pMessage, uint16_t* pOutputBuffer, uint32_t outputBufferSize);
