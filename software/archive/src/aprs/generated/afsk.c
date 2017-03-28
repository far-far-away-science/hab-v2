#include "afsk.h"

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   uint32_t targetReciprocalAngularFrequency,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol)
{
    const bool isOtherFrequencySlopePositive = IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(otherFrequencyCurrentTrigArg);
    const uint32_t otherFrequencyCurrentAmplitude = scaledSineValueFromTable[otherFrequencyCurrentTrigArg];
    const uint32_t inverseTrigArg = PRECISION_CONVERTER_INVERSE_TRIG_PARAM(otherFrequencyCurrentAmplitude * INVERSE_TRIG_PARAM_SCALER);
    const uint32_t result = PRECISION_CONVERTER_QUANT_IDX(scaledArcSineValueFromTable[inverseTrigArg] * targetReciprocalAngularFrequency);

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
