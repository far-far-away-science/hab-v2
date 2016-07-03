#pragma once

#include "test/test.h"
#include "stm32l0xx_hal.h"

#include "../aprs_impl.h"

#include "test_data.h"

TEST_CLASS(aprs_encodeAprsMessage_perf,
{
    TEST_METHOD(encoding_aprs_should_work_fast_enough,
    {
        resetTestData();

        const uint16_t trialsCount = 1000;
        const uint32_t startTimeMilliseconds = HAL_GetTick();

        for (uint16_t i = 0; i < trialsCount; ++i)
        {
            encodeAprsMessage(&CALLSIGN_SOURCE,
                              (uint8_t*) "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76",
                              70,
                              &g_testAx25EncodedMessage);
            g_testAx25EncodedMessage.size.chars = 0;
            g_testAx25EncodedMessage.size.lastCharBits = 0;
        }

        const uint32_t endTimeMilliseconds = HAL_GetTick();
        const float actualRuntimeMilliseconds = (float) (endTimeMilliseconds - startTimeMilliseconds) / ((float) trialsCount);

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 1.42f);
#else
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(actualRuntimeMilliseconds, 2.65f);
#endif
    })
})
