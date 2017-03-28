#pragma once

#include <string.h>

#include <test/test.h>

#include "gps/nmea_buffer.h"

TEST_CLASS(nmeaBuffer_nmeaReceiveCharacter,
{
    TEST_METHOD(should_receive_full_message,
    {
        NmeaRingBuffer rb = { 0 };
        char message[] = { "$this is a test message\r\n" };
        for (int i = 0; i < strlen(message); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(1, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message), rb.buffer[0].size);
        ASSERT_IS_FALSE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message, rb.buffer[0].message, strlen(message));
    })

    TEST_METHOD(should_receive_2_full_messages,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a 1st test message\r\n" };
        char message2[] = { "$this is a 2nd test message!!!\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(2, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message1), rb.buffer[0].size);
        ASSERT_IS_FALSE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1, rb.buffer[0].message, strlen(message1));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));
    })

    TEST_METHOD(should_receive_3_full_messages,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a 1st test message\r\n" };
        char message2[] = { "$this is a 2nd test message!!!\r\n" };
        char message3[] = { "$this is a third test message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message3); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message3[i], false);
            if (i < strlen(message3) - 1)
            {
                ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
            }
            else
            {
                ASSERT_INT_ARE_EQUAL(NRBS_FULL, rb.statusMask);
            }
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(0, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message1), rb.buffer[0].size);
        ASSERT_IS_FALSE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1, rb.buffer[0].message, strlen(message1));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));

        ASSERT_INT_ARE_EQUAL(strlen(message3), rb.buffer[2].size);
        ASSERT_IS_FALSE(rb.buffer[2].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message3, rb.buffer[2].message, strlen(message3));
    })

    TEST_METHOD(should_receive_3_messages_without_CRLF_separator,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a 1st test message" };
        char message2[] = { "$this is a 2nd test message!!!" };
        char message3[] = { "$this is a third test message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message3); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message3[i], false);
            if (i < strlen(message3) - 1)
            {
                ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
            }
            else
            {
                ASSERT_INT_ARE_EQUAL(NRBS_FULL, rb.statusMask);
            }
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(0, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message1), rb.buffer[0].size);
        ASSERT_IS_FALSE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1, rb.buffer[0].message, strlen(message1));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));

        ASSERT_INT_ARE_EQUAL(strlen(message3), rb.buffer[2].size);
        ASSERT_IS_FALSE(rb.buffer[2].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message3, rb.buffer[2].message, strlen(message3));
    })

    TEST_METHOD(should_skip_text_which_does_not_start_with_dollar_sign,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "this is a test message" };
        char message2[] = { "$this is a real test message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], false);
            ASSERT_INT_ARE_EQUAL(NRBS_WAIT_UNTIL_NEXT_MESSAGE, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(1, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[0].size);
        ASSERT_IS_FALSE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[0].message, strlen(message2));
    })

    TEST_METHOD(should_handle_messages_which_are_too_long,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a test message 1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM this is a test message~!@#%^&*()_+{}|:\"<>?,./;'[]\\-=" };
        char message2[] = { "$this is a real test message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], false);
            if (i < NMEA_MESSAGE_MAX_LEN)
            {
                ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
            }
            else
            {
                ASSERT_INT_ARE_EQUAL(NRBS_WAIT_UNTIL_NEXT_MESSAGE, rb.statusMask);
            }
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(2, rb.endIdx);

        char message1Fixed[] = { "$this is a test message 1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM this is a test message~!@#%^&*()_+{}|:\"<>" };

        ASSERT_INT_ARE_EQUAL(strlen(message1Fixed), rb.buffer[0].size);
        ASSERT_IS_TRUE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1Fixed, rb.buffer[0].message, strlen(message1Fixed));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));
    })

    TEST_METHOD(should_store_error_information_passed_in,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a test message\r\n" };
        char message2[] = { "$this is a real test message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], i == 5);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(2, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message1), rb.buffer[0].size);
        ASSERT_IS_TRUE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1, rb.buffer[0].message, strlen(message1));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));
    })

    TEST_METHOD(should_recover_from_buffer_full_after_read,
    {
        NmeaRingBuffer rb = { 0 };
        char message1[] = { "$this is a test message\r\n" };
        char message2[] = { "$this is a real test 2nd message\r\n" };
        char message3[] = { "$this is a real test third message\r\n" };
        char message4[] = { "$this is a real test 4 message\r\n" };
        for (int i = 0; i < strlen(message1); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message1[i], i == 5);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message2); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message2[i], false);
            ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
        }
        for (int i = 0; i < strlen(message3); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message3[i], false);
            if (i < strlen(message3) - 1)
            {
                ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
            }
            else
            {
                ASSERT_INT_ARE_EQUAL(NRBS_FULL, rb.statusMask);
            }
        }

        ASSERT_INT_ARE_EQUAL(0, rb.startIdx);
        ASSERT_INT_ARE_EQUAL(0, rb.endIdx);

        ASSERT_INT_ARE_EQUAL(strlen(message1), rb.buffer[0].size);
        ASSERT_IS_TRUE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message1, rb.buffer[0].message, strlen(message1));

        ASSERT_INT_ARE_EQUAL(strlen(message2), rb.buffer[1].size);
        ASSERT_IS_FALSE(rb.buffer[1].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message2, rb.buffer[1].message, strlen(message2));

        ASSERT_INT_ARE_EQUAL(strlen(message3), rb.buffer[2].size);
        ASSERT_IS_FALSE(rb.buffer[2].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message3, rb.buffer[2].message, strlen(message3));

        NmeaMessage result;
        ASSERT_IS_TRUE(nmeaReadMessage(&rb, &result));

        for (int i = 0; i < strlen(message4); ++i)
        {
            nmeaReceiveCharacter(&rb, (uint8_t) message4[i], false);
            if (i < strlen(message4) - 1)
            {
                ASSERT_INT_ARE_EQUAL(0, rb.statusMask);
            }
            else
            {
                ASSERT_INT_ARE_EQUAL(NRBS_FULL, rb.statusMask);
            }
        }

        ASSERT_INT_ARE_EQUAL(strlen(message4), rb.buffer[0].size);
        ASSERT_IS_TRUE(rb.buffer[0].hasError);
        ASSERT_STR_ARE_EQUAL((uint8_t*) message4, rb.buffer[0].message, strlen(message4));
    })
})
