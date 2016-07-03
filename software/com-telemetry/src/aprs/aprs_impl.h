#pragma once

#include <aprs/aprs.h>

#define APRS_PAYLOAD_BUFFER_MAX_LENGTH 34

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, Ax25EncodedMessage* pAx25EncodedMessage);

uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);

uint8_t threeDigitInt2str(uint16_t value, uint8_t* pBuffer);
