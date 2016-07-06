#pragma once

#include <test/test.h>

#include "../afsk.h"
#include "../generated/afsk.h"

#include "test_data.h"

TEST_CLASS(ax25_encodeAndAppendDataAsAx25_perf,
{
    TEST_METHOD(encoding_to_ax25_should_work_fast_enough,
    {
        resetTestData();

        g_testAx25EncodingCtx.lastBit = 1;

        const uint16_t trialsCount = 1000;
        const uint32_t startTimeMilliseconds = getTimeMilliseconds();

        for (uint16_t i = 0; i < trialsCount; ++i)
        {
            encodeAndAppendDataAsAx25((uint8_t*) "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76",
                                      70,
                                      FCS_CALCULATE,
                                      SHIFT_ONE_LEFT,
                                      &g_testAx25EncodingCtx,
                                      &g_testAx25EncodedMessage);
            g_testAx25EncodedMessage.size.chars = 0;
            g_testAx25EncodedMessage.size.lastCharBits = 0;
        }

        const uint32_t endTimeMilliseconds = getTimeMilliseconds();
        const float actualRuntimeMilliseconds = (float) (endTimeMilliseconds - startTimeMilliseconds) / ((float) trialsCount);

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 0.65f);
#else
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 1.18f);
#endif
    })
})
