#pragma once

#ifdef TRIG_SLOW
    #define AMPLITUDE_SCALED_AND_SHIFTED_SINE(v) (AMPLITUDE_SHIFT + AMPLITUDE_SCALER * sinf(v))
    #define COSINE_G_THAN_0(v)                   (cosf(v) >= 0)
    #define INVERSE_SINE(v)                      asinf(v)
#else
    #include <stdint.h>
    #include <stdbool.h>

    extern const int TRIG_COUNT;
    extern const int INVERSE_TRIG_COUNT;
    extern const int TRIG_MULTIPLIER;
    extern const int INVERSE_TRIG_MULTIPLIER;

    extern const float AMPLITUDE_SCALED_AND_SHIFTED_SINE_TABLE[];
    extern const float COS_G_THAN_0[];
    extern const float ASIN[];

    #define TRIG_FLOAT_TO_INT(value) \
        (int) ((value) * TRIG_MULTIPLIER + 0.5f)

    #define INVERSE_TRIG_FLOAT_TO_INT(value) \
        (INVERSE_TRIG_MULTIPLIER + (int) ((value) * INVERSE_TRIG_MULTIPLIER + 0.5f))

    #define AMPLITUDE_SCALED_AND_SHIFTED_SINE(v) AMPLITUDE_SCALED_AND_SHIFTED_SINE_TABLE[TRIG_FLOAT_TO_INT(v)]
    #define COSINE_G_THAN_0(v)                   COS_G_THAN_0[TRIG_FLOAT_TO_INT(v)]
    #define INVERSE_SINE(v)                      ASIN[INVERSE_TRIG_FLOAT_TO_INT(v)]
#endif
