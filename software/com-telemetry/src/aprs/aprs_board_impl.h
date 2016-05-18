#pragma once

#include "aprs_board.h"

#define PREFIX_FLAGS_COUNT 1
#define SUFFIX_FLAGS_COUNT 10

#define MAX_APRS_PAYLOAD_LENGTH 128

// bit stream will have extra bits in it so it must be larger than message buffer
// in worst case we will insert extra 0 for every 5 bits
#define APRS_BITSTREAM_MAX_LEN (APRS_PAYLOAD_LEN + APRS_PAYLOAD_LEN / 5 + 1)

// should be around 0.5ms for HX-1 warm-up (10 / 1200 = 8ms)
#define LEADING_WARMUP_AMPLITUDE_DC_PULSES_COUNT     10
// to abort previous frame send at least 15 ones without any stuffing (putting zeroes in between)
#define LEADING_ONES_COUNT_TO_CANCEL_PREVIOUS_PACKET 48

/*
 * FCS
 */
 
#define FCS_POLYNOMIAL                0x8408
#define FCS_INITIAL_VALUE             0xFFFF
#define FCS_POST_PROCESSING_XOR_VALUE 0xFFFF

typedef enum FCS_TYPE_t
{
    FCS_NONE,
    FCS_CALCULATE,
} FCS_TYPE;

typedef enum STUFFING_TYPE_t
{
    ST_NO_STUFFING,
    ST_PERFORM_STUFFING,
} STUFFING_TYPE;

typedef enum SHIFT_ONE_LEFT_TYPE_t
{
    SHIFT_ONE_LEFT_NO,
    SHIFT_ONE_LEFT,
} SHIFT_ONE_LEFT_TYPE;

typedef struct EncodingContext_t
{
    uint16_t fcs;
    uint8_t lastBit;
    uint8_t numberOfOnes;
} EncodingContext;

void advanceBitstreamBit(BitstreamSize* pResultBitstreamSize);

bool encodeAndAppendBits(const uint8_t* pMessageData,
                         uint16_t messageDataSize,
                         STUFFING_TYPE stuffingType,
                         FCS_TYPE fcsType,
                         SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                         EncodingContext* pEncodingContext,
                         AprsEncodedMessage* pResultAprsEncodedMessage);

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, AprsEncodedMessage* pEncdedMessage);

uint8_t createGpsAprsPayload(const GpsData* pGpsData, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);
uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);
