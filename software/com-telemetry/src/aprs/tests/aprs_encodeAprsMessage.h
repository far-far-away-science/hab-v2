#pragma once

#include <test/test.h>

#include "../aprs_impl.h"

#include "test_data.h"

#ifdef TEST

    const uint8_t EXPECTED_DATA[] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0x80,
        0x30,
        0xDB,
        0xD2,
        0x2C,
        0x8B,
        0x6A,
        0x0B,
        0x25,
        0x2B,
        0x29,
        0x25,
        0x2B,
        0x29,
        0x07,
        0xCF,
        0x24,
        0x2D,
        0xD3,
        0x76,
        0x95,
        0x09,
        0x54,
        0xF5,
        0x6D,
        0x2F,
        0x35,
        0x2F,
        0x2F,
        0x2B,
        0x9D,
        0x8A,
        0x84,
        0x76,
        0x8F,
        0x8C,
        0x8A,
        0x9E,
        0x8A,
        0x8A,
        0x8A,
        0x62,
        0x73,
        0x77,
        0x89,
        0x74,
        0x61,
        0x71,
        0x85,
        0x8A,
        0x76,
        0x9D,
        0xDE,
        0x62,
        0x75,
        0x75,
        0x71,
        0x77,
        0x75,
        0x61,
        0x77,
        0x77,
        0x8F,
        0x76,
        0x9D,
        0x30,
        0x9D,
        0x74,
        0x9D,
        0x7A,
        0x9D,
        0x74,
        0x61,
        0x75,
        0x77,
        0x9D,
        0x8E,
        0x74,
        0x61,
        0x8F,
        0x62,
        0x23,
        0x9D,
        0x8C,
        0x8C,
        0x9E,
        0x76,
        0x9D,
        0xDC,
        0x62,
        0x9D,
        0x66,
        0x8F,
        0x8E,
        0xD6,
        0x2F,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
        0x02,
    };

#endif

TEST_CLASS(aprs_encodeAprsMessage,
{
    TEST_METHOD(should_properly_encode_aprs_message,
    {
        resetTestData();

        encodeAprsMessage(&CALLSIGN_SOURCE,
                          (uint8_t*) "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76",
                          70,
                          &g_testAx25EncodedMessage);

        ASSERT_INT_ARE_EQUAL(112, g_testAx25EncodedMessage.size.chars);
        ASSERT_INT_ARE_EQUAL(2, g_testAx25EncodedMessage.size.lastCharBits);

        ASSERT_BINARY_ARE_EQUAL(EXPECTED_DATA, g_testAx25EncodedMessage.buffer, g_testAx25EncodedMessage.size.chars);
    })
})
