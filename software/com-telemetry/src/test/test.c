#include "test.h"

#if defined(TEST)
    #include <string.h>

    #include <gps/tests/nmea_buffer_advanceUint8Index.h>
    #include <gps/tests/nmea_buffer_nmeaReadMessage.h>
    #include <gps/tests/nmea_buffer_nmeaReceiveCharacter.h>

#include <aprs/tests/afsk_encodeAx25MessageAsAfsk_perf.h>

    void ASSERT_IS_TRUE(bool value)
    {
        if (!value)
        {
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void ASSERT_IS_FALSE(bool value)
    {
        ASSERT_IS_TRUE(!value);
    }

    void ASSERT_ARE_EQUAL(uint8_t expected, uint8_t actual)
    {
        ASSERT_IS_TRUE(expected == actual);
    }

    void ASSERT_ARE_STR_EQUAL(uint8_t* pExpected, uint8_t* pActual, uint16_t size)
    {
        ASSERT_IS_TRUE(memcmp(pExpected, pActual, size) == 0);
    }

    void executeTests()
    {
        RUN_TEST_CLASS(nmeaBuffer_advanceUint8Index);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReadMessage);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReceiveCharacter);
        RUN_TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf);
    }

#endif
