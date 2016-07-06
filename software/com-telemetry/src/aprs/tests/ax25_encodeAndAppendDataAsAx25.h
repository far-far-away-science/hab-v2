#pragma once

#include <test/test.h>

#include "../afsk.h"
#include "../generated/afsk.h"

#include "test_data.h"

TEST_CLASS(ax25_encodeAndAppendDataAsAx25,
{
    TEST_METHOD(should_perform_stuffing_that_is_adding_0_after_five_ones_if_requested,
    {
        resetTestData();

        g_testUint8Buffer[0] = 0xFF; // 0b1111'1111
        g_testUint8Buffer[1] = 0xFF; // 0b1111'1111

        ASSERT_IS_TRUE(encodeAndAppendDataAsAx25(g_testUint8Buffer, 2, FCS_NONE, SHIFT_ONE_LEFT_NO, &g_testAx25EncodingCtx, &g_testAx25EncodedMessage));

        ASSERT_INT_ARE_EQUAL(2, g_testAx25EncodedMessage.size.chars);
        ASSERT_INT_ARE_EQUAL(3, g_testAx25EncodedMessage.size.lastCharBits);
        ASSERT_INT_ARE_EQUAL(0xE0 /* 0b1110'0000 */, g_testAx25EncodedMessage.buffer[0]);
        ASSERT_INT_ARE_EQUAL(0x07 /* 0b0000'0111 */, g_testAx25EncodedMessage.buffer[1]);
        ASSERT_INT_ARE_EQUAL(0x06 /* 0b0000'0110 */, g_testAx25EncodedMessage.buffer[2]);
    })

    TEST_METHOD(while_performing_stuffing_0_bit_should_reset_count_of_ones,
    {
        resetTestData();

        g_testUint8Buffer[0] = 0xFF; // 0b1111'1111
        g_testUint8Buffer[1] = 0xDE; // 0b1101'1110

        ASSERT_IS_TRUE(encodeAndAppendDataAsAx25(g_testUint8Buffer, 2, FCS_NONE, SHIFT_ONE_LEFT_NO, &g_testAx25EncodingCtx, &g_testAx25EncodedMessage));

        ASSERT_INT_ARE_EQUAL(2, g_testAx25EncodedMessage.size.chars);
        ASSERT_INT_ARE_EQUAL(1, g_testAx25EncodedMessage.size.lastCharBits);
        ASSERT_INT_ARE_EQUAL(0xE0 /* 0b1110'0000 */, g_testAx25EncodedMessage.buffer[0]);
        ASSERT_INT_ARE_EQUAL(0xC1 /* 0b1100'0001 */, g_testAx25EncodedMessage.buffer[1]);
        ASSERT_INT_ARE_EQUAL(0x01 /* 0b0000'0001 */, g_testAx25EncodedMessage.buffer[2]);
    })

    TEST_METHOD(should_work_correctly_when_one_byte_was_used_for_7_bits_and_followed_by_10_bit_data,
    {
        resetTestData();

        g_testUint8Buffer[0] = 0xFF;
        g_testUint8Buffer[1] = 0xFF;
        g_testUint8Buffer[2] = 0xFF;
        g_testUint8Buffer[3] = 0xFF;
        g_testUint8Buffer[4] = 0xFE;
        g_testUint8Buffer[5] = 0xFF;

        ASSERT_IS_TRUE(encodeAndAppendDataAsAx25(g_testUint8Buffer, 6, FCS_NONE, SHIFT_ONE_LEFT_NO, &g_testAx25EncodingCtx, &g_testAx25EncodedMessage));

        ASSERT_INT_ARE_EQUAL(7, g_testAx25EncodedMessage.size.chars);
        ASSERT_INT_ARE_EQUAL(1, g_testAx25EncodedMessage.size.lastCharBits);
        ASSERT_INT_ARE_EQUAL(0xE0, g_testAx25EncodedMessage.buffer[0]);
        ASSERT_INT_ARE_EQUAL(0x07, g_testAx25EncodedMessage.buffer[1]);
        ASSERT_INT_ARE_EQUAL(0x7E, g_testAx25EncodedMessage.buffer[2]);
        ASSERT_INT_ARE_EQUAL(0xE0, g_testAx25EncodedMessage.buffer[3]);
        ASSERT_INT_ARE_EQUAL(0xC7, g_testAx25EncodedMessage.buffer[4]);
        ASSERT_INT_ARE_EQUAL(0x0F, g_testAx25EncodedMessage.buffer[5]);
        ASSERT_INT_ARE_EQUAL(0xFC, g_testAx25EncodedMessage.buffer[6]);
        ASSERT_INT_ARE_EQUAL(0x00, g_testAx25EncodedMessage.buffer[7]);
    })
})
