#include "test.h"

#if defined(TEST)
    #include <math.h>
    #include <string.h>

    #include <gps/tests/nmea_buffer_advanceUint8Index.h>
    #include <gps/tests/nmea_buffer_nmeaReadMessage.h>
    #include <gps/tests/nmea_buffer_nmeaReceiveCharacter.h>

    #include <aprs/tests/afsk_encodeAx25MessageAsAfsk_perf.h>
    #include <aprs/tests/ax25_encodeAndAppendDataAsAx25_perf.h>

    static char g_buffer[256];

    void assertIsTrue(bool value, uint32_t lineNumber)
    {
        if (!value)
        {
            sprintf(g_buffer, "    ASSERT IsTrue failed, line=%u\r\n", (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertIsFalse(bool value, uint32_t lineNumber)
    {
        if (value)
        {
            sprintf(g_buffer, "    ASSERT IsFalse failed, line=%u\r\n", (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertIntAreEqual(uint8_t expected, uint8_t actual, uint32_t lineNumber)
    {
        if (expected != actual)
        {
            sprintf(g_buffer, "    ASSERT %u == %u failed, line=%u\r\n", (unsigned int) expected, (unsigned int) actual, (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertStrAreEqual(uint8_t* pExpected, uint8_t* pActual, uint16_t size, uint32_t lineNumber)
    {
        if (memcmp(pExpected, pActual, size) != 0)
        {
            sprintf(g_buffer, "    ASSERT '%s' == '%s' (size = %u) failed, line=%u\r\n", pExpected, pActual, (unsigned int) size, (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertFloatIsLessThanOrEqual(float value1, float value2, uint32_t lineNumber)
    {
        if (value1 > value2)
        {
            float intPart1;
            float fractionalPart1 = modff(value1, &intPart1);

            float intPart2;
            float fractionalPart2 = modff(value2, &intPart2);

            int32_t whole1 = (int32_t) intPart1;
            uint32_t fractional1 = (uint32_t) (fractionalPart1 * 100000);

            int32_t whole2 = (int32_t) intPart2;
            uint32_t fractional2 = (uint32_t) (fractionalPart2 * 100000);

            sprintf(g_buffer, "    ASSERT %i.%u <= %i.%u failed, line=%u\r\n",
                    (int) whole1,
                    (unsigned int) fractional1,
                    (int) whole2,
                    (unsigned int) fractional2,
                    (unsigned int) lineNumber);

            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void executeTests()
    {
        TRACE_STRING("\r\n");
        RUN_TEST_CLASS(nmeaBuffer_advanceUint8Index);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReadMessage);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReceiveCharacter);
        RUN_TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf);
        RUN_TEST_CLASS(ax25_encodeAndAppendDataAsAx25_perf);
        TRACE_STRING("Done!");
    }

#endif
