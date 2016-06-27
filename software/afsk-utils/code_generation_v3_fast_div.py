import numpy

import definitions
import definitions_derived
import afsk_modulation_fixedpoint_fast_div

#
# TODO kpl need to add range checks everywhere as fast div isn't awefully precise
#

class Generator:
    def __init__(self):
        self.modulation = afsk_modulation_fixedpoint_fast_div.AfskModulationFixedPointFastDiv(definitions.FixedPointFastDivPrecisionData(), [0x65], 8)
        self.modulation.afskModulate()

    def formatBestFastDivision(self, fastDivisionAlias):
        (firstDivisorPowerOfTwo, multiplier, lastDivisorPowerOfTwo, precisionSummand, clampValue) = self.modulation.getBestFastDivision(fastDivisionAlias)
        if firstDivisorPowerOfTwo != 0:
            return 'CLAMP(((((((uint32_t) value) + ' + str(precisionSummand) + ') >> ' + str(firstDivisorPowerOfTwo) + ') * ' + str(multiplier) + ') >> ' + str(lastDivisorPowerOfTwo) + '), ' + str(clampValue) + ')'
        else:
            return 'CLAMP((((((uint32_t) value) + ' + str(precisionSummand) + ') * ' + str(multiplier) + ') >> ' + str(lastDivisorPowerOfTwo) + ')' + str(clampValue) + ')'

    def generateDefinitionsHeader(self, filePath):
        text = '''#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef DEBUG
    inline uint32_t CLAMP(uint32_t value, uint32_t maxValue)
    {
        return value > maxValue ? maxValue : value;
    }
#else
    uint32_t CLAMP(uint32_t value, uint32_t maxValue);
#endif

#define RESET_CONTEXT_GENERATED_PART(pAfskContext) \\
    { \\
        (pAfskContext)->currentF1200TrigArg = 0; \\
        (pAfskContext)->currentF2200TrigArg = 0; \\
    }

//
// To figure out what those values mean see afsk-utils Python project,
// code_generation_v3_fast_div.py file
//

#define APRS_SIGNAL_GENERATION_FREQUENCY ((uint32_t) ''' + str(definitions.APRS_SIGNAL_GENERATION_FREQUENCY) + ''')

#define QUANT_MIN_VALUE ((uint32_t) ''' + str(definitions.QUANT_MIN_VALUE) + ''')
#define QUANT_STEP_SIZE ((uint32_t) ''' + str(self.modulation.CONST_QUANT_STEP_SIZE.getInternalRepresentation()) + ''')

#define SCALED_PI_OVER_TWO    ((uint32_t) ''' + str(self.modulation.CONST_PI_OVER_TWO.getInternalRepresentation()) + ''')
#define SCALED_THREE_HALFS_PI ((uint32_t) ''' + str(self.modulation.CONST_THREE_HALFS_PI.getInternalRepresentation()) + ''')

#define HALF_PERIOD_F1200 ((uint32_t) ''' + str(self.modulation.CONST_HALF_PERIOD_F1200.getInternalRepresentation()) + ''')
#define HALF_PERIOD_F2200 ((uint32_t) ''' + str(self.modulation.CONST_HALF_PERIOD_F2200.getInternalRepresentation()) + ''')

#define QUANTS_COUNT_PER_SYMBOL_F1200 ((uint32_t) ''' + str(self.modulation.CONST_F1200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()) + ''')
#define QUANTS_COUNT_PER_SYMBOL_F2200 ((uint32_t) ''' + str(self.modulation.CONST_F2200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()) + ''')

#define PRECISION_CONVERTER_QUANT_IDX_F1200(value)''' + self.formatBestFastDivision('CONST_PRECISION_QUANT_DIVISOR_F1200') + '''
#define PRECISION_CONVERTER_QUANT_IDX_F2200(value)''' + self.formatBestFastDivision('CONST_PRECISION_QUANT_DIVISOR_F2200') + '''

#define TRIG_PARAM_SCALER_F1200 ((uint32_t) ''' + str(self.modulation.CONST_TRIG_PARAM_SCALER_F1200.getInternalRepresentation()) + ''')
#define TRIG_PARAM_SCALER_F2200 ((uint32_t) ''' + str(self.modulation.CONST_TRIG_PARAM_SCALER_F2200.getInternalRepresentation()) + ''')
#define PRECISION_CONVERTER_TRIG_PARAM_F1200(value) ''' + self.formatBestFastDivision('CONST_PRECISION_TRIG_PARAM_DIVISOR_F1200') + '''
#define PRECISION_CONVERTER_TRIG_PARAM_F2200(value) ''' + self.formatBestFastDivision('CONST_PRECISION_TRIG_PARAM_DIVISOR_F2200') + '''

#define INVERSE_TRIG_PARAM_SCALER ((uint32_t) ''' + str(self.modulation.CONST_INVERSE_TRIG_SCALER.getInternalRepresentation()) + ''')
#define PRECISION_CONVERTER_INVERSE_TRIG_PARAM(value) ''' + self.formatBestFastDivision('CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR') + '''

#define QUANTS_COUNT_PER_SYMBOL ((uint32_t) ''' + str(int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)) + ''')

#define RECIPROCAL_ANGULAR_FREQUENCY_F1200 ((uint32_t) ''' + str(self.modulation.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200.getInternalRepresentation()) + ''')
#define RECIPROCAL_ANGULAR_FREQUENCY_F2200 ((uint32_t) ''' + str(self.modulation.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200.getInternalRepresentation()) + ''')

#define LEADING_WARMUP_QUANTS_COUNT ((uint32_t) ''' + str(definitions_derived.LEADING_WARMUP_QUANTS_COUNT) + ''')

extern const uint16_t amplitudeFromTable[];
extern const uint32_t scaledSineValueFromTable[];
extern const uint16_t scaledArcSineValueFromTable[];

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   bool isTargetFrequency1200,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol);

#define IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(value) \\
    (!((value) > SCALED_PI_OVER_TWO && (value) < SCALED_THREE_HALFS_PI))

#define CALCULATE_F1200_TRIG_ARG_FROM_QUANT_IDX(currentF1200Quant) \\
    PRECISION_CONVERTER_TRIG_PARAM_F1200(TRIG_PARAM_SCALER_F1200 * (currentF1200Quant))

#define CALCULATE_F2200_TRIG_ARG_FROM_QUANT_IDX(currentF2200Quant) \\
    PRECISION_CONVERTER_TRIG_PARAM_F2200(TRIG_PARAM_SCALER_F2200 * (currentF2200Quant))

#define CALCULATE_F1200_AMPLITUDE_FROM_QUANT_IDX(pAfskCtx) \\
    amplitudeFromTable[(pAfskCtx)->currentF1200TrigArg = CALCULATE_F1200_TRIG_ARG_FROM_QUANT_IDX((pAfskCtx)->currentF1200Quant)]

#define CALCULATE_F2200_AMPLITUDE_FROM_QUANT_IDX(pAfskCtx) \\
    amplitudeFromTable[(pAfskCtx)->currentF2200TrigArg = CALCULATE_F2200_TRIG_ARG_FROM_QUANT_IDX((pAfskCtx)->currentF2200Quant)]

#define CALCULATE_F1200_QUANT_IDX_FROM_F2200_QUANT_IDX(pAfskCtx) \\
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude((pAfskCtx)->currentF2200TrigArg, true, HALF_PERIOD_F1200, QUANTS_COUNT_PER_SYMBOL_F1200)

#define CALCULATE_F2200_QUANT_IDX_FROM_F1200_QUANT_IDX(pAfskCtx) \\
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude((pAfskCtx)->currentF1200TrigArg, false, HALF_PERIOD_F2200, QUANTS_COUNT_PER_SYMBOL_F2200)
'''

        with open(filePath, 'w+') as f:
            f.write(text)

    def generateContextGeneratedHeader(self, filePath):
        text = '''#pragma once

uint32_t currentF1200TrigArg;
uint32_t currentF2200TrigArg;'''

        with open(filePath, 'w+') as f:
            f.write(text)

    def generateDefinitionsSource(self, filePath):
        maxArcSine = max(self.modulation.trigTables.arcSineValues).getInternalRepresentation()
        str1200 = self.formatBestFastDivision('CONST_PRECISION_QUANT_DIVISOR_F1200')
        str2200 = self.formatBestFastDivision('CONST_PRECISION_QUANT_DIVISOR_F2200')
        if str1200 == str2200:
            raise RuntimeError('you should consider handling F1200 and F2200 cases in the same way as they have same division approximation')

        text = '''#include "afsk.h"

#ifdef DEBUG
    uint32_t CLAMP(uint32_t value, uint32_t maxValue)
    {
        return value > maxValue ? maxValue : value;
    }
#endif

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   bool isTargetFrequency1200,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol)
{
    const bool isOtherFrequencySlopePositive = IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(otherFrequencyCurrentTrigArg);
    const uint32_t otherFrequencyCurrentAmplitude = scaledSineValueFromTable[otherFrequencyCurrentTrigArg];
    const uint32_t inverseTrigArg = PRECISION_CONVERTER_INVERSE_TRIG_PARAM(otherFrequencyCurrentAmplitude * INVERSE_TRIG_PARAM_SCALER);
    const uint32_t result = isTargetFrequency1200 ? PRECISION_CONVERTER_QUANT_IDX_F1200(scaledArcSineValueFromTable[inverseTrigArg] * RECIPROCAL_ANGULAR_FREQUENCY_F1200) :
                                                    PRECISION_CONVERTER_QUANT_IDX_F2200(scaledArcSineValueFromTable[inverseTrigArg] * RECIPROCAL_ANGULAR_FREQUENCY_F2200);

    if (!isOtherFrequencySlopePositive)
    {
        if (result > targetFrequencyHalfPeriod)
        {
            return targetFrequencyQuantsCountPerSymbol + targetFrequencyHalfPeriod - result;
        }
        else
        {
            return targetFrequencyHalfPeriod - result;
        }
    }
    else
    {
        return result;
    }
}
'''
        with open(filePath, 'w+') as f:
            f.write(text)

    def generateSineTableSource(self, filePath):
        text = '''#include "afsk.h"

const uint32_t scaledSineValueFromTable[] =
{
'''
        for value in self.modulation.trigTables.sineValues:
            text += '    ' + str(value.getInternalRepresentation()) + ',\n'
        text += '''};
'''
        with open(filePath, 'w+') as f:
            f.write(text)

    def generateArcSineTableSource(self, filePath):
        text = '''#include "afsk.h"

const uint16_t scaledArcSineValueFromTable[] =
{
'''
        for value in self.modulation.trigTables.arcSineValues:
            text += '    ' + str(value.getInternalRepresentation()) + ',\n'
        text += '''};
'''
        with open(filePath, 'w+') as f:
            f.write(text)

    def generateAmplitudeTableSource(self, filePath):
        text = '''#include "afsk.h"

const uint16_t amplitudeFromTable[] =
{
'''
        for value in self.modulation.trigTables.sineValues:
            text += '    ' + str(self.modulation.convertAmplitudeToFinalFormat(value)) + ',\n'
        text += '''};
'''
        with open(filePath, 'w+') as f:
            f.write(text)

generator = Generator()
generator.generateDefinitionsHeader("../com-telemetry/src/aprs/generated/afsk.h")
generator.generateContextGeneratedHeader("../com-telemetry/src/aprs/generated/afsk_context_generated.h")
generator.generateDefinitionsSource("../com-telemetry/src/aprs/generated/afsk.c")
generator.generateSineTableSource("../com-telemetry/src/aprs/generated/sine.c")
generator.generateArcSineTableSource("../com-telemetry/src/aprs/generated/arcsine.c")
generator.generateAmplitudeTableSource("../com-telemetry/src/aprs/generated/amplitude.c")
