#include "afsk.h"

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
