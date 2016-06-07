#pragma once

#include "test/test.h"
#include "aprs/aprs_board_impl.h"
#include "stm32l0xx_hal.h"

#ifdef TEST
    #define DAC_BUFFER_SIZE 128

    static uint16_t g_dacBuffer[DAC_BUFFER_SIZE];
    static AprsEncodedMessage g_aprsEncodedMessage;
#endif

TEST_CLASS(aprs_board_encodeAprsMessageAsAfsk_perf,
{
    TEST_METHOD(encoding_to_afsk_should_work_fast_enough_for_dac_dma_to_function_prperly,
    {
        resetAfskContext(&g_aprsEncodedMessage.afskContext);
        g_aprsEncodedMessage.size.chars = MAX_APRS_MESSAGE_LENGTH;
        g_aprsEncodedMessage.size.lastCharBits = 0;
        for (uint32_t i = 0; i < MAX_APRS_MESSAGE_LENGTH; ++i)
        {
            g_aprsEncodedMessage.buffer[i] = 0xAA;
        }

        uint16_t trialsCount = 0;

        const uint32_t startTimeMilliseconds = HAL_GetTick();

        while (encodeAprsMessageAsAfsk(&g_aprsEncodedMessage, g_dacBuffer, DAC_BUFFER_SIZE))
        {
            ++trialsCount;
        }

        const uint32_t endTimeMilliseconds = HAL_GetTick();
        const float actualRuntimeMilliseconds = (endTimeMilliseconds - startTimeMilliseconds) / (float) trialsCount;
        const float expectedRuntimeMilliseconds = 1000.0f * DAC_BUFFER_SIZE / APRS_SIGNAL_GENERATION_FREQUENCY;
        const float speedUpFactor = actualRuntimeMilliseconds / expectedRuntimeMilliseconds;

        // this is the time it takes on RELEASE build at 16MHz on STM32L073.
        // DEBUG is too slow (~1.15 or so) not sure what else to do on debug.
        ASSERT_IS_TRUE(speedUpFactor <= 0.86f);
        // approximating division by (((v >> 6) * 67) >> 20)
        // ASSERT_IS_TRUE(speedUpFactor <= 0.40f);
    })
})
