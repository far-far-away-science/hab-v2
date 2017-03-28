#include "test.h"

#if defined(TEST)
    #include <math.h>
    #include <stdio.h>
    #include <string.h>

    #include <gps/tests/nmea_buffer_advanceUint8Index.h>
    #include <gps/tests/nmea_buffer_nmeaReadMessage.h>
    #include <gps/tests/nmea_buffer_nmeaReceiveCharacter.h>

    #include <aprs/tests/aprs_encodeAprsMessage.h>
    #include <aprs/tests/aprs_encodeAprsMessage_perf.h>
    #include <aprs/tests/ax25_encodeAndAppendDataAsAx25.h>
    #include <aprs/tests/afsk_encodeAx25MessageAsAfsk_perf.h>
    #include <aprs/tests/ax25_encodeAndAppendDataAsAx25_perf.h>

    static bool g_isFailed = false;
    static char g_buffer[256] = { 0 };

    void assertIsTrue(bool value, uint32_t lineNumber)
    {
        if (!value)
        {
            g_isFailed = true;
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
            g_isFailed = true;
            sprintf(g_buffer, "    ASSERT IsFalse failed, line=%u\r\n", (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertIntAreEqual(uint32_t expected, uint32_t actual, uint32_t lineNumber)
    {
        if (expected != actual)
        {
            g_isFailed = true;
            sprintf(g_buffer, "    ASSERT %u == %u failed, line=%u\r\n", (unsigned int) expected, (unsigned int) actual, (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertStrAreEqual(const uint8_t* pExpected, const uint8_t* pActual, uint16_t size, uint32_t lineNumber)
    {
        if (memcmp(pExpected, pActual, size) != 0)
        {
            g_isFailed = true;
            sprintf(g_buffer, "    ASSERT '%s' == '%s' (size = %u) failed, line=%u\r\n", pExpected, pActual, (unsigned int) size, (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertBinaryAreEqual(const uint8_t* pExpected, const uint8_t* pActual, uint16_t size, uint32_t lineNumber)
    {
        if (memcmp(pExpected, pActual, size) != 0)
        {
            g_isFailed = true;
            sprintf(g_buffer, "    ASSERT, line=%u\r\n", (unsigned int) lineNumber);
            TRACE_STRING_NO_EOL(g_buffer);
            for (uint16_t i = 0; i < size; ++i)
            {
                if (pExpected[i] != pActual[i])
                {
                    sprintf(g_buffer, "      [%u] %u != %u\r\n", (unsigned int) i, (unsigned int) pExpected[i], (unsigned int) pActual[i]);
                    TRACE_STRING_NO_EOL(g_buffer);
                }
            }
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void assertFloatIsLessThanOrEqual(float value1, float value2, uint32_t lineNumber)
    {
        if (value1 > value2)
        {
            g_isFailed = true;

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
        g_isFailed = false;
        TRACE_STRING("\r\n");
        RUN_TEST_CLASS(nmeaBuffer_advanceUint8Index);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReadMessage);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReceiveCharacter);
        RUN_TEST_CLASS(ax25_encodeAndAppendDataAsAx25);
        RUN_TEST_CLASS(ax25_encodeAndAppendDataAsAx25_perf);
        RUN_TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf);
        RUN_TEST_CLASS(aprs_encodeAprsMessage_perf);
        RUN_TEST_CLASS(aprs_encodeAprsMessage);
        if (g_isFailed)
        {
            TRACE_STRING("=================");
            TRACE_STRING("= FAILED !!!!!! =");
            TRACE_STRING("=================");
        }
        else
        {
            TRACE_STRING("Done!");
        }
    }

#endif
