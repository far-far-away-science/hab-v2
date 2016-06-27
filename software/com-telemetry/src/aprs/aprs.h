#pragma once

#include <gps/nmea_messages.h>
#include <telemetry/telemetry.h>

#include "ax25.h"

typedef struct Callsign_t
{
    const uint8_t callsign[7];
    const uint8_t ssid;
} Callsign;

extern const Callsign CALLSIGN_SOURCE;
extern const Callsign CALLSIGN_DESTINATION_1;
extern const Callsign CALLSIGN_DESTINATION_2;

bool encodeNmeaAprsMessage(const Callsign* pCallsign, const NmeaMessage* pNmeaMessage, Ax25EncodedMessage* pEncodedMessage);
bool encodeTelemetryAprsMessage(const Callsign* pCallsign, const Telemetry* pTelemetry, Ax25EncodedMessage* pEncodedMessage);
