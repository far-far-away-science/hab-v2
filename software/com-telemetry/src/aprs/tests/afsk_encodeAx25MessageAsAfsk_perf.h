#pragma once

#include <test/test.h>

#include "../afsk.h"
#include "../generated/afsk.h"

#include "test_data.h"

TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf,
{
    TEST_METHOD(encoding_to_afsk_should_work_fast_enough_for_dac_dma_to_function_prperly,
    {
        resetTestData();

        resetAfskContext(&g_testAfskCtx);
        g_testAx25EncodedMessage.size.chars = MAX_AX25_MESSAGE_LENGTH;
        g_testAx25EncodedMessage.size.lastCharBits = 0;
        for (uint32_t i = 0; i < MAX_AX25_MESSAGE_LENGTH; ++i)
        {
            g_testAx25EncodedMessage.buffer[i] = 0xAA;
        }

        uint16_t trialsCount = 0;

        const uint32_t startTimeMilliseconds = getTimeMilliseconds();

        while (encodeAx25MessageAsAfsk(&g_testAx25EncodedMessage, &g_testAfskCtx, g_testUint16Buffer, TEST_BUFFER_SIZE))
        {
            ++trialsCount;
        }

        const uint32_t endTimeMilliseconds = getTimeMilliseconds();
        const float actualRuntimeMilliseconds = (endTimeMilliseconds - startTimeMilliseconds) / (float) trialsCount;
        const float expectedRuntimeMilliseconds = 1000.0f * TEST_BUFFER_SIZE / APRS_SIGNAL_GENERATION_FREQUENCY;
        const float speedUpFactor = actualRuntimeMilliseconds / expectedRuntimeMilliseconds;

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(speedUpFactor, 0.32f);
#else
        ASSERT_FLOAT_IS_LESS_THAN_OR_EQUAL(speedUpFactor, 0.62f);
#endif
    })
})
