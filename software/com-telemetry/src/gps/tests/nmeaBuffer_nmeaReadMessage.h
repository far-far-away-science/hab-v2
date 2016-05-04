#pragma once

#include <string.h>

#include "test\test.h"

#include "..\nmeaBuffer.h"

TEST_CLASS(nmeaBuffer_nmeaReadMessage,
{
    TEST_METHOD(should_return_false_if_buffer_is_empty,
    {
        NmeaMessage result = { 0 };
        NmeaRingBuffer ringBuffer = { 0 };

        ASSERT_IS_FALSE(nmeaReadMessage(&ringBuffer, &result));
    })

    void createMessage(NmeaMessage* pMessage, char* pStr, bool hasError)
    {
        pMessage->hasError = hasError;
        pMessage->size = strlen(pStr);
        memcpy(pMessage->message, pStr, pMessage->size);
    }

    TEST_METHOD(should_read_message_if_buffer_has_one_message_and_end_is_after_start,
    {
        NmeaMessage result = { 0 };
        NmeaRingBuffer ringBuffer = { 0 };
        ringBuffer.startIdx = 0;
        ringBuffer.endIdx = 1;
        createMessage(&ringBuffer.buffer[0], "test1", false);

        ASSERT_IS_TRUE(nmeaReadMessage(&ringBuffer, &result));
        ASSERT_IS_FALSE(result.hasError);
        ASSERT_ARE_EQUAL(5, result.size);
        ASSERT_ARE_STR_EQUAL((uint8_t*) "test1", result.message, 5);

        ASSERT_IS_FALSE(nmeaReadMessage(&ringBuffer, &result));
    })

    TEST_METHOD(should_read_message_if_buffer_has_one_message_and_end_is_before_start,
    {
        NmeaMessage result = { 0 };
        NmeaRingBuffer ringBuffer = { 0 };
        ringBuffer.startIdx = NMEA_RING_BUFFER_MAX_MESSAGES_COUNT - 1;
        ringBuffer.endIdx = 0;
        createMessage(&ringBuffer.buffer[NMEA_RING_BUFFER_MAX_MESSAGES_COUNT - 1], "test1", true);

        ASSERT_IS_TRUE(nmeaReadMessage(&ringBuffer, &result));
        ASSERT_IS_TRUE(result.hasError);
        ASSERT_ARE_EQUAL(5, result.size);
        ASSERT_ARE_STR_EQUAL((uint8_t*) "test1", result.message, 5);

        ASSERT_IS_FALSE(nmeaReadMessage(&ringBuffer, &result));
    })

    TEST_METHOD(should_read_all_messages_from_full_buffer,
    {
        NmeaMessage result = { 0 };
        NmeaRingBuffer ringBuffer = { 0 };
        ringBuffer.startIdx = 0;
        ringBuffer.endIdx = 0;
        ringBuffer.statusMask = NRBS_FULL;
        createMessage(&ringBuffer.buffer[0], "test1", false);
        createMessage(&ringBuffer.buffer[1], "test2a", true);
        createMessage(&ringBuffer.buffer[2], "test3", false);

        ASSERT_IS_TRUE(nmeaReadMessage(&ringBuffer, &result));
        ASSERT_IS_FALSE(result.hasError);
        ASSERT_ARE_EQUAL(5, result.size);
        ASSERT_ARE_STR_EQUAL((uint8_t*) "test1", result.message, 5);

        ASSERT_IS_TRUE(nmeaReadMessage(&ringBuffer, &result));
        ASSERT_IS_TRUE(result.hasError);
        ASSERT_ARE_EQUAL(6, result.size);
        ASSERT_ARE_STR_EQUAL((uint8_t*) "test2a", result.message, 6);

        ASSERT_IS_TRUE(nmeaReadMessage(&ringBuffer, &result));
        ASSERT_IS_FALSE(result.hasError);
        ASSERT_ARE_EQUAL(5, result.size);
        ASSERT_ARE_STR_EQUAL((uint8_t*) "test3", result.message, 5);

        ASSERT_IS_FALSE(nmeaReadMessage(&ringBuffer, &result));
    })
})
