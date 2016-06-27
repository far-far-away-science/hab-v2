#pragma once

#include "test/test.h"
#include "stm32l0xx_hal.h"

#include "../afsk.h"
#include "../generated/afsk.h"

#ifdef TEST
    #define DAC_BUFFER_SIZE 128

    static AfskContext g_afskContext;
    static uint16_t g_dacBuffer[DAC_BUFFER_SIZE];
    static Ax25EncodedMessage g_ax25EncodedMessage;
#endif

TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf,
{
    TEST_METHOD(encoding_to_afsk_should_work_fast_enough_for_dac_dma_to_function_prperly,
    {
        resetAfskContext(&g_afskContext);
        g_ax25EncodedMessage.size.chars = MAX_AX25_MESSAGE_LENGTH;
        g_ax25EncodedMessage.size.lastCharBits = 0;
        for (uint32_t i = 0; i < MAX_AX25_MESSAGE_LENGTH; ++i)
        {
            g_ax25EncodedMessage.buffer[i] = 0xAA;
        }

        uint16_t trialsCount = 0;

        const uint32_t startTimeMilliseconds = HAL_GetTick();

        while (encodeAx25MessageAsAfsk(&g_ax25EncodedMessage, &g_afskContext, g_dacBuffer, DAC_BUFFER_SIZE))
        {
            ++trialsCount;
        }

        const uint32_t endTimeMilliseconds = HAL_GetTick();
        const float actualRuntimeMilliseconds = (endTimeMilliseconds - startTimeMilliseconds) / (float) trialsCount;
        const float expectedRuntimeMilliseconds = 1000.0f * DAC_BUFFER_SIZE / APRS_SIGNAL_GENERATION_FREQUENCY;
        const float speedUpFactor = actualRuntimeMilliseconds / expectedRuntimeMilliseconds;

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_IS_TRUE(speedUpFactor <= 0.32f);
#else
        ASSERT_IS_TRUE(speedUpFactor <= 0.62f);
#endif
    })
})
