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

#define PI 3.141592654f

#define QUANT_STEP_SIZE 1.0f

#define QUANT_MIN_VALUE 0
#define QUANT_MAX_VALUE 4095

// to abort previous frame send at least 15 ones without any stuffing (putting zeroes in between to break ones sequences longer than 5)
#define LEADING_ONES_COUNT_TO_CANCEL_PREVIOUS_PACKET 48

/*
 * those values are calculated from previous ones
 */

// should be around 5-10 ms for HX-1 warm-up
#define LEADING_WARMUP_QUANTS_COUNT (APRS_SIGNAL_GENERATION_FREQUENCY * 0.01f /* 10 ms */)

#define F1200_QUANTS_COUNT_PER_SYMBOL (APRS_SIGNAL_GENERATION_FREQUENCY / 1200.0f)
#define F2200_QUANTS_COUNT_PER_SYMBOL (APRS_SIGNAL_GENERATION_FREQUENCY / 2200.0f)

#define AMPLITUDE_SCALER ((QUANT_MAX_VALUE - QUANT_MIN_VALUE) / 2)
#define RECIPROCAL_AMPLITUDE_SCALER (1.0f / AMPLITUDE_SCALER)

#define AMPLITUDE_SHIFT (AMPLITUDE_SCALER + QUANT_MIN_VALUE)

#define HALF_PERIOD_F1200 (F1200_QUANTS_COUNT_PER_SYMBOL / 2.0f)
#define HALF_PERIOD_F2200 (F2200_QUANTS_COUNT_PER_SYMBOL / 2.0f)

#define ANGULAR_FREQUENCY_F1200 (2.0f * PI / F1200_QUANTS_COUNT_PER_SYMBOL)
#define ANGULAR_FREQUENCY_F2200 (2200.0f * ANGULAR_FREQUENCY_F1200 / 1200.0f)

#define RECIPROCAL_ANGULAR_FREQUENCY_F1200 (1.0f / ANGULAR_FREQUENCY_F1200)
#define RECIPROCAL_ANGULAR_FREQUENCY_F2200 (1.0f / ANGULAR_FREQUENCY_F2200)

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

float normalizePulseWidth(float width);
