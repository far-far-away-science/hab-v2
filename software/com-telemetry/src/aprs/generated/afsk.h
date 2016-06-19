#pragma once

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

#define RESET_CONTEXT_GENERATED_PART(pAfskContext) \
    { \
        (pAfskContext)->currentF1200TrigArg = 0; \
        (pAfskContext)->currentF2200TrigArg = 0; \
    }

//
// To figure out what those values mean see afsk-utils Python project,
// code_generation_v2.py file
//

#define APRS_SIGNAL_GENERATION_FREQUENCY ((uint32_t)96000)

#define QUANT_MIN_VALUE ((uint32_t)0)
#define QUANT_STEP_SIZE ((uint32_t)10000)

#define SCALED_PI_OVER_TWO    ((uint32_t)768)
#define SCALED_THREE_HALFS_PI ((uint32_t)2304)

#define HALF_PERIOD_F1200 ((uint32_t)400000)
#define HALF_PERIOD_F2200 ((uint32_t)218182)

#define QUANTS_COUNT_PER_SYMBOL_F1200 ((uint32_t)800000)
#define QUANTS_COUNT_PER_SYMBOL_F2200 ((uint32_t)436364)

#define PRECISION_CONVERTER_QUANT_IDX_F1200(value) CLAMP(((((((uint32_t) value) + 500) >> 3) * 33) >> 12), 800000)
#define PRECISION_CONVERTER_QUANT_IDX_F2200(value) CLAMP(((((((uint32_t) value) + 500) >> 3) * 66) >> 13), 436364)

#define TRIG_PARAM_SCALER_F1200 ((uint32_t)3840)
#define TRIG_PARAM_SCALER_F2200 ((uint32_t)7040)
#define PRECISION_CONVERTER_TRIG_PARAM_F1200(value) CLAMP(((((((uint32_t) value) + 500000) >> 6) * 67) >> 20), 3072)
#define PRECISION_CONVERTER_TRIG_PARAM_F2200(value) CLAMP(((((((uint32_t) value) + 500000) >> 6) * 67) >> 20), 3072)

#define INVERSE_TRIG_PARAM_SCALER ((uint32_t)750)
#define PRECISION_CONVERTER_INVERSE_TRIG_PARAM(value) CLAMP(((((((uint32_t) value) + 500000) >> 6) * 67) >> 20), 3072)

#define QUANTS_COUNT_PER_SYMBOL ((uint32_t)80)

#define RECIPROCAL_ANGULAR_FREQUENCY_F1200 ((uint32_t)127324)
#define RECIPROCAL_ANGULAR_FREQUENCY_F2200 ((uint32_t)69449)

#define LEADING_WARMUP_QUANTS_COUNT ((uint32_t)960)

extern const uint16_t amplitudeFromTable[];
extern const uint32_t scaledSineValueFromTable[];
extern const uint16_t scaledArcSineValueFromTable[];

uint32_t calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude(uint32_t otherFrequencyCurrentTrigArg,
                                                                   bool isTargetFrequency1200,
                                                                   uint32_t targetFrequencyHalfPeriod,
                                                                   uint32_t targetFrequencyQuantsCountPerSymbol);

#define IS_COSINE_GREATER_OR_EQUAL_TO_ZERO(value) \
    (!((value) > SCALED_PI_OVER_TWO && (value) < SCALED_THREE_HALFS_PI))

#define CALCULATE_F1200_TRIG_ARG_FROM_QUANT_IDX(currentF1200Quant) \
    PRECISION_CONVERTER_TRIG_PARAM_F1200(TRIG_PARAM_SCALER_F1200 * (currentF1200Quant))

#define CALCULATE_F2200_TRIG_ARG_FROM_QUANT_IDX(currentF2200Quant) \
    PRECISION_CONVERTER_TRIG_PARAM_F2200(TRIG_PARAM_SCALER_F2200 * (currentF2200Quant))

#define CALCULATE_F1200_AMPLITUDE_FROM_QUANT_IDX(afskCtx) \
    amplitudeFromTable[(afskCtx).currentF1200TrigArg = CALCULATE_F1200_TRIG_ARG_FROM_QUANT_IDX((afskCtx).currentF1200Quant)]

#define CALCULATE_F2200_AMPLITUDE_FROM_QUANT_IDX(afskCtx) \
    amplitudeFromTable[(afskCtx).currentF2200TrigArg = CALCULATE_F2200_TRIG_ARG_FROM_QUANT_IDX((afskCtx).currentF2200Quant)]

#define CALCULATE_F1200_QUANT_IDX_FROM_F2200_QUANT_IDX(afskCtx) \
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude((afskCtx).currentF2200TrigArg, true, HALF_PERIOD_F1200, QUANTS_COUNT_PER_SYMBOL_F1200)

#define CALCULATE_F2200_QUANT_IDX_FROM_F1200_QUANT_IDX(afskCtx) \
    calculateQuantIndexFromOtherFrequencyQuantIdxAndAmplitude((afskCtx).currentF1200TrigArg, false, HALF_PERIOD_F2200, QUANTS_COUNT_PER_SYMBOL_F2200)
