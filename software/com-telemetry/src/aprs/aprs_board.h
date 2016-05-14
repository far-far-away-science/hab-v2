#pragma once

#include <gps/nmea_messages.h>
#include <telemetry/telemetry.h>

#define APRS_SIGNAL_GENERATION_FREQUENCY 100000

typedef struct Callsign_t
{
    const uint8_t callsign[7];
    const uint8_t ssid;
} Callsign;

extern const Callsign CALLSIGN_SOURCE;
extern const Callsign CALLSIGN_DESTINATION_1;
extern const Callsign CALLSIGN_DESTINATION_2;

bool sendAprsMessage(const GpsData* pGpsData);
bool sendTelemetryMessage(const Telemetry* pTelemetry);

/*
 * returns true if data was filled in
 */
bool fillInAmplitudesBuffer(uint16_t* pBuffer, uint32_t bufferSize);
