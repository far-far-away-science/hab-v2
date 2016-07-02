#pragma once

#include "test/test.h"
#include "stm32l0xx_hal.h"

#include "../afsk.h"
#include "../generated/afsk.h"

#ifdef TEST
    static Ax25EncodingContext g_encodingCtx;
    static Ax25EncodedMessage g_ax25EncodedMessage;
#endif

TEST_CLASS(ax25_encodeAndAppendDataAsAx25_perf,
{
    TEST_METHOD(encoding_to_ax25_should_work_fast_enough,
    {
        g_encodingCtx.lastBit = 1;

        const uint16_t trialsCount = 1000;
        const uint32_t startTimeMilliseconds = HAL_GetTick();

        for (uint16_t i = 0; i < trialsCount; ++i)
        {
            encodeAndAppendDataAsAx25((uint8_t*) "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76",
                                      70,
                                      ST_PERFORM_STUFFING,
                                      FCS_CALCULATE, SHIFT_ONE_LEFT_NO,
                                      &g_encodingCtx,
                                      &g_ax25EncodedMessage);
            g_ax25EncodedMessage.size.chars = 0;
            g_ax25EncodedMessage.size.lastCharBits = 0;
        }

        const uint32_t endTimeMilliseconds = HAL_GetTick();
        const float actualRuntimeMilliseconds = (float) (endTimeMilliseconds - startTimeMilliseconds) / ((float) trialsCount);

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 2.39f);
#else
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 5.95f);
#endif
    })
})
