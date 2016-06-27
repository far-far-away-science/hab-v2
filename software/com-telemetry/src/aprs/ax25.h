#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_AX25_MESSAGE_LENGTH 200

typedef struct BitstreamSize_t
{
    uint16_t chars;
    uint8_t lastCharBits;
} BitstreamSize;

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

typedef struct Ax25EncodingContext_t
{
    uint8_t lastBit;
    uint8_t numberOfOnes;
} Ax25EncodingContext;

typedef struct Ax25EncodedMessage_t
{
    BitstreamSize size;
    uint8_t buffer[MAX_AX25_MESSAGE_LENGTH];
} Ax25EncodedMessage;


bool isAx25MessageEmtpy(const Ax25EncodedMessage* pMessage);

void advanceBitstreamBit(BitstreamSize* pResultBitstreamSize);

bool encodeAndAppendDataAsAx25(const uint8_t* pMessageData,
                               uint16_t messageDataSize,
                               STUFFING_TYPE stuffingType,
                               FCS_TYPE fcsType,
                               SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                               Ax25EncodingContext* pAx25EncodingContext,
                               Ax25EncodedMessage* pResultAprsEncodedMessage);
