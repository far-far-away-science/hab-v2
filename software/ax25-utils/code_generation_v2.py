import numpy
import data_generator

class Generator:
    def __init__(self):
        self.combinations = data_generator.generateAllByteToAx25DataCombinations()
        self.frameSeparatorOne = data_generator.calculateNewAx25DataFromOldImpl(1, 0, 0x7E, False)
        self.frameSeparatorZero = data_generator.calculateNewAx25DataFromOldImpl(0, 0, 0x7E, False)

    def generateDefinitionsHeader(self, filePath):
        text = '''#pragma once

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

#define FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ZERO ''' + str(self.frameSeparatorZero[0]) + '''
#define FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ONE ''' + str(self.frameSeparatorOne[0]) + '''

#define GET_VALUE_IF_LAST_BIT_IS_ONE(pAx25EncodedData) \\
    ((~(pAx25EncodedData)->dataGivenThatPreviousBitWasZero) & ((1 << ((pAx25EncodedData)->dataBitsCount)) - 1))
#define GET_VALUE_IF_LAST_BIT_IS_ZERO(pAx25EncodedData) \\
    ((pAx25EncodedData)->dataGivenThatPreviousBitWasZero)
#define GET_LAST_BIT(value, pAx25EncodedData) \\
    (((value) >> ((pData)->dataBitsCount - 1)) & 1)

#define GENERATE_AX25_TABLE_INDEX(currentNumberOfOnes, byte) \\
    (((currentNumberOfOnes) << 8) + (byte))

#define GET_AX25_ENCODED_DATA_FOR_BYTE(currentNumberOfOnes, byte) \\
    &byte2ax25EncodedData[GENERATE_AX25_TABLE_INDEX((currentNumberOfOnes), (byte))];
'''
        with open(filePath, 'w+') as f:
            f.write(text)

    def generateSource(self, filePath):
        text = '''#include "ax25.h"

const AX25EncodedData byte2ax25EncodedData[] =
{
'''
        i = 0
        for (oldNumberOfOnes, byte2Encode, newDataGiventLastBitWasZero, newLastBitGiventLastBitWasZero, newDataGiventLastBitWasOne, newLastBitGiventLastBitWasOne, newDataNumberOfBits, newNumberOfOnes) in self.combinations:
            text += '  {' + '{:>3}'.format(newDataGiventLastBitWasZero) + ', ' + '{:>2}'.format(newDataNumberOfBits) + ', ' + '{:>2}'.format(newNumberOfOnes) + '}, ' + \
                    '// idx = ' + '{:0>4}'.format(i) + ', oldNumberOfOnes = ' + str(oldNumberOfOnes) + ', byte2Encode = ' + '{:0>3}'.format(byte2Encode) + '\n'
            i += 1
        text += '''};
'''
        with open(filePath, 'w+') as f:
            f.write(text)

generator = Generator()
generator.generateDefinitionsHeader("../com-telemetry/src/aprs/generated/ax25.h")
generator.generateSource("../com-telemetry/src/aprs/generated/ax25.c")
