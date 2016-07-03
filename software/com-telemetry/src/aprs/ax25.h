#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_AX25_MESSAGE_LENGTH 200

// to abort previous frame send at least 15 ones without any stuffing (putting zeroes in between to break ones sequences longer than 5)
// this parameter is N * 8 ones (only full bytes are supported but more ones is even a bit more reliable)
#define LEADING_FF_BYTES_COUNT_TO_CANCEL_PREVIOUS_PACKET 6

#define PREFIX_FLAGS_COUNT 1
#define SUFFIX_FLAGS_COUNT 10

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

bool encodeAndAppendPrefixAsAx25(Ax25EncodingContext* pAx25EncodingContext, Ax25EncodedMessage* pResultAprsEncodedMessage);

bool encodeAndAppendDataAsAx25(const uint8_t* pMessageData,
                               uint16_t messageDataSize,
                               FCS_TYPE fcsType,
                               SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                               Ax25EncodingContext* pAx25EncodingContext,
                               Ax25EncodedMessage* pResultAprsEncodedMessage);

bool encodeAndAppendSuffixAsAx25(Ax25EncodingContext* pAx25EncodingContext, Ax25EncodedMessage* pResultAprsEncodedMessage);
