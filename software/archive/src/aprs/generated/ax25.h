#pragma once

#include <stdint.h>

typedef struct AX25EncodedData_t
{
    uint16_t dataGivenThatPreviousBitWasZero;
    uint8_t dataBitsCount;
    uint8_t newNumberOfOnes;
} AX25EncodedData;

//
// To figure out what those values mean see ax25-utils Python project,
// code_generation_v2.py file
//

extern const AX25EncodedData byte2ax25EncodedData[];

#define FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ZERO 127
#define FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ONE 128

#define GET_VALUE_IF_LAST_BIT_IS_ONE(pAx25EncodedData) \
    ((~(pAx25EncodedData)->dataGivenThatPreviousBitWasZero) & ((1 << ((pAx25EncodedData)->dataBitsCount)) - 1))
#define GET_VALUE_IF_LAST_BIT_IS_ZERO(pAx25EncodedData) \
    ((pAx25EncodedData)->dataGivenThatPreviousBitWasZero)
#define GET_LAST_BIT(value, pAx25EncodedData) \
    (((value) >> ((pData)->dataBitsCount - 1)) & 1)

#define GENERATE_AX25_TABLE_INDEX(currentNumberOfOnes, byte) \
    (((currentNumberOfOnes) << 8) + (byte))

#define GET_AX25_ENCODED_DATA_FOR_BYTE(currentNumberOfOnes, byte) \
    &byte2ax25EncodedData[GENERATE_AX25_TABLE_INDEX((currentNumberOfOnes), (byte))];
