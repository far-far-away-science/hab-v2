#pragma once

#include <aprs/aprs.h>

#define APRS_PAYLOAD_BUFFER_MAX_LENGTH 128

/*
 * those values are calculated in advance depending on MCU/etc
 */

#define PREFIX_FLAGS_COUNT 1
#define SUFFIX_FLAGS_COUNT 10

// to abort previous frame send at least 15 ones without any stuffing (putting zeroes in between to break ones sequences longer than 5)
// this parameter is N * 8 ones (only full bytes are supported but more ones is even a bit more reliable)
#define LEADING_FF_BYTES_COUNT_TO_CANCEL_PREVIOUS_PACKET 6

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, Ax25EncodedMessage* pAx25EncodedMessage);

uint8_t createGpsAprsPayload(const GpsData* pGpsData, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);
uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);

uint8_t threeDigitInt2str(uint16_t value, uint8_t* pBuffer);
