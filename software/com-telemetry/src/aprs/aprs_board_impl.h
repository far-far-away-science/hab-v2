#pragma once

#include "aprs_board.h"

#define APRS_PAYLOAD_BUFFER_MAX_LENGTH 128

/*
 * FCS
 */
 
#define FCS_POLYNOMIAL                0x8408
#define FCS_INITIAL_VALUE             0xFFFF
#define FCS_POST_PROCESSING_XOR_VALUE 0xFFFF

/*
 * those values are calculated in advance depending on MCU/etc
 */

#define PREFIX_FLAGS_COUNT 1
#define SUFFIX_FLAGS_COUNT 10

// to abort previous frame send at least 15 ones without any stuffing (putting zeroes in between to break ones sequences longer than 5)
#define LEADING_ONES_COUNT_TO_CANCEL_PREVIOUS_PACKET 48

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

void resetAfskContext(AfskContext* pAfskContext);

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, AprsEncodedMessage* pEncdedMessage);

uint8_t createGpsAprsPayload(const GpsData* pGpsData, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);
uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength);
