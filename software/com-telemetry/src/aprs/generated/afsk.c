#include "afsk.h"

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
