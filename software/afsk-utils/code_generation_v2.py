import definitions
import definitions_derived
import afsk_modulation_fixedpoint

class Generator:
    def __init__(self):
        self.modulation = afsk_modulation_fixedpoint.AfskModulationFixedPoint([0x65], 3)
        self.modulation.afskModulate()

    def generateDefinitionsHeader(self, filePath):
        text = '''#pragma once

#include <stdint.h>
#include <stdbool.h>

//
// To figure out what those values mean see afsk-utils Python project,
// code_generation_v2.py file
//

#define APRS_SIGNAL_GENERATION_FREQUENCY ''' + str(definitions.APRS_SIGNAL_GENERATION_FREQUENCY) + '''

#define QUANT_MIN_VALUE ''' + str(definitions.QUANT_MIN_VALUE) + '''
#define QUANT_STEP_SIZE ''' + str(self.modulation.CONST_QUANT_STEP_SIZE.getInternalRepresentation()) + '''

#define SCALED_PI_OVER_TWO    ''' + str(self.modulation.CONST_PI_OVER_TWO.getInternalRepresentation()) + '''
#define SCALED_THREE_HALFS_PI ''' + str(self.modulation.CONST_THREE_HALFS_PI.getInternalRepresentation()) + '''

#define HALF_PERIOD_F1200 ''' + str(self.modulation.CONST_HALF_PERIOD_F1200.getInternalRepresentation()) + '''
#define HALF_PERIOD_F2200 ''' + str(self.modulation.CONST_HALF_PERIOD_F2200.getInternalRepresentation()) + '''

#define PRECISION_QUANT_DIVISOR       ''' + str(self.modulation.CONST_PRECISION_QUANT_DIVISOR) + '''
#define PRECISION_QUANT_ROUND_SUMMAND ''' + str(self.modulation.CONST_PRECISION_QUANT_ROUND_SUMMAND) + '''

#define TRIG_PARAM_SCALER_F1200            ''' + str(self.modulation.CONST_TRIG_PARAM_SCALER_F1200.getInternalRepresentation()) + '''
#define TRIG_PARAM_SCALER_F2200            ''' + str(self.modulation.CONST_TRIG_PARAM_SCALER_F2200.getInternalRepresentation()) + '''
#define PRECISION_TRIG_PARAM_DIVISOR       ''' + str(self.modulation.CONST_PRECISION_TRIG_PARAM_DIVISOR) + '''
#define PRECISION_TRIG_PARAM_ROUND_SUMMAND ''' + str(self.modulation.CONST_PRECISION_TRIG_PARAM_ROUND_SUMMAND) + '''

#define INVERSE_TRIG_PARAM_SCALER                  ''' + str(self.modulation.CONST_INVERSE_TRIG_SCALER.getInternalRepresentation()) + '''
#define PRECISION_INVERSE_TRIG_PARAM_DIVISOR       ''' + str(self.modulation.CONST_PRECISION_INVERSE_TRIG_PARAM_DIVISOR) + '''
#define PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND ''' + str(self.modulation.CONST_PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND) + '''

#define QUANTS_COUNT_PER_SYMBOL ''' + str(int(definitions_derived.F1200_QUANTS_COUNT_PER_SYMBOL)) + '''

#define QUANTS_COUNT_PER_SYMBOL_F1200 ''' + str(self.modulation.CONST_F1200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()) + '''
#define QUANTS_COUNT_PER_SYMBOL_F2200 ''' + str(self.modulation.CONST_F2200_QUANTS_COUNT_PER_SYMBOL.getInternalRepresentation()) + '''

#define RECIPROCAL_ANGULAR_FREQUENCY_F1200 ''' + str(self.modulation.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F1200.getInternalRepresentation()) + '''
#define RECIPROCAL_ANGULAR_FREQUENCY_F2200 ''' + str(self.modulation.CONST_RECIPROCAL_ANGULAR_FREQUENCY_F2200.getInternalRepresentation()) + '''

#define PRECISION_OUTPUT_AMPLITUDE_DIVISOR       ''' + str(self.modulation.CONST_PRECISION_OUTPUT_AMPLITUDE_DIVISOR) + '''
#define PRECISION_OUTPUT_AMPLITUDE_ROUND_SUMMAND ''' + str(self.modulation.CONST_PRECISION_OUTPUT_AMPLITUDE_ROUNT_SUMMAND) + '''

#define LEADING_WARMUP_QUANTS_COUNT ''' + str(definitions_derived.LEADING_WARMUP_QUANTS_COUNT) + '''

extern const uint16_t amplitudeFromTable[];
extern const uint32_t scaledSineValueFromTable[];
extern const uint16_t scaledArcSineValueFromTable[];

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   uint32_t targetFrequencyReciprocalAngularFrequency,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol);

#define REDUCE_PRECISION(value, precision_rounding, precision_divisor) \\
    ((value) + (precision_rounding)) / (precision_divisor)

#define IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(value) \\
    (!(value > SCALED_PI_OVER_TWO && value < SCALED_THREE_HALFS_PI))

#define CALCULATE_F1200_TRIG_ARG_FROM_QUANT_IDX(currentF1200Quant) \\
    REDUCE_PRECISION(TRIG_PARAM_SCALER_F1200 * (currentF1200Quant), PRECISION_TRIG_PARAM_ROUND_SUMMAND, PRECISION_TRIG_PARAM_DIVISOR)

#define CALCULATE_F2200_TRIG_ARG_FROM_QUANT_IDX(currentF2200Quant) \\
    REDUCE_PRECISION(TRIG_PARAM_SCALER_F2200 * (currentF2200Quant), PRECISION_TRIG_PARAM_ROUND_SUMMAND, PRECISION_TRIG_PARAM_DIVISOR)

#define CALCULATE_F1200_AMPLITUDE_FROM_TRIG_ARG(currentF1200TrigArg) \\
    amplitudeFromTable[currentF1200TrigArg]

#define CALCULATE_F2200_AMPLITUDE_FROM_TRIG_ARG(currentF2200TrigArg) \\
    amplitudeFromTable[currentF2200TrigArg]

#define CALCULATE_F1200_QUANT_IDX_FROM_F2200_TRIG_ARG(currentF2200TrigArg) \\
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(currentF2200TrigArg, RECIPROCAL_ANGULAR_FREQUENCY_F1200, HALF_PERIOD_F1200, QUANTS_COUNT_PER_SYMBOL_F1200)

#define CALCULATE_F2200_QUANT_IDX_FROM_F1200_TRIG_ARG(currentF1200TrigArg) \\
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(currentF1200TrigArg, RECIPROCAL_ANGULAR_FREQUENCY_F2200, HALF_PERIOD_F2200, QUANTS_COUNT_PER_SYMBOL_F2200)

'''

        with open(filePath, 'w+') as f:
            f.write(text)

    def generateDefinitionsSource(self, filePath):
        text = '''#include "afsk.h"

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   uint32_t targetFrequencyReciprocalAngularFrequency,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol)
{
    const bool isOtherFrequencySlopePositive = IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(otherFrequencyCurrentTrigArg);
    const uint32_t otherFrequencyCurrentAmplitude = scaledSineValueFromTable[otherFrequencyCurrentTrigArg];
    const uint32_t inverseTrigArg = REDUCE_PRECISION(otherFrequencyCurrentAmplitude * INVERSE_TRIG_PARAM_SCALER, PRECISION_INVERSE_TRIG_PARAM_ROUND_SUMMAND, PRECISION_INVERSE_TRIG_PARAM_DIVISOR);
    const uint32_t result = REDUCE_PRECISION(scaledArcSineValueFromTable[inverseTrigArg] * targetFrequencyReciprocalAngularFrequency, PRECISION_QUANT_ROUND_SUMMAND, PRECISION_QUANT_DIVISOR);

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
generator.generateDefinitionsSource("../com-telemetry/src/aprs/generated/afsk.c")
generator.generateSineTableSource("../com-telemetry/src/aprs/generated/sine.c")
generator.generateArcSineTableSource("../com-telemetry/src/aprs/generated/arcsine.c")
generator.generateAmplitudeTableSource("../com-telemetry/src/aprs/generated/amplitude.c")
