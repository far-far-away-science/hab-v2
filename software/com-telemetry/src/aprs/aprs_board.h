#pragma once

#include <gps/nmea_messages.h>
#include <telemetry/telemetry.h>

#include "generated/afsk.h"

#define MAX_APRS_MESSAGE_LENGTH 200

typedef struct BitstreamSize_t
{
    uint16_t chars;
    uint8_t lastCharBits;
} BitstreamSize;

typedef struct AfskContext_t
{
    BitstreamSize pos;
    uint16_t leadingWarmUpQuantsLeft;
    bool currentFrequencyIsF1200;
    uint32_t currentF1200Quant;
    uint32_t currentF2200Quant;
    uint32_t currentSymbolQuant;
    bool lastCharacterGenerated;
#include "generated/afsk_context_generated.h"
} AfskContext;

typedef struct Callsign_t
{
    const uint8_t callsign[7];
    const uint8_t ssid;
} Callsign;

typedef struct AprsEncodedMessage_t
{
    BitstreamSize size;
    uint8_t buffer[MAX_APRS_MESSAGE_LENGTH];
    AfskContext afskContext;
} AprsEncodedMessage;

extern const Callsign CALLSIGN_SOURCE;
extern const Callsign CALLSIGN_DESTINATION_1;
extern const Callsign CALLSIGN_DESTINATION_2;

bool isAprsMessageEmtpy(const AprsEncodedMessage* pMessage);

bool encodeGpsAprsMessage(const Callsign* pCallsign, const GpsData* pGpsData, AprsEncodedMessage* pEncdedMessage);
bool encodeTelemetryAprsMessage(const Callsign* pCallsign, const Telemetry* pTelemetry, AprsEncodedMessage* pEncdedMessage);

/*
 * Should fill full requested length of the buffer. If no data available add Zeroes.
 *
 * returns true if data was filled in
 */
bool encodeAprsMessageAsAfsk(AprsEncodedMessage* pMessage, uint16_t* pOutputBuffer, uint32_t outputBufferSize);
