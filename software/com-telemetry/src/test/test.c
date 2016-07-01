#include "test.h"

#if defined(TEST)
    #include <string.h>

    #include <gps/tests/nmea_buffer_advanceUint8Index.h>
    #include <gps/tests/nmea_buffer_nmeaReadMessage.h>
    #include <gps/tests/nmea_buffer_nmeaReceiveCharacter.h>

    #include <aprs/tests/afsk_encodeAx25MessageAsAfsk_perf.h>
    #include <aprs/tests/ax25_encodeAndAppendDataAsAx25_perf.h>

    void AssertIsTrue(bool value, uint32_t lineNumber)
    {
        if (!value)
        {
            TRACE_FORMAT_STRING("AssertIsTrue failed, line=%u\n", lineNumber);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void AssertIsFalse(bool value, uint32_t lineNumber)
    {
        if (value)
        {
            TRACE_FORMAT_STRING("AssertIsFalse failed, line=%u\n", lineNumber);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void AssertIntAreEqual(uint8_t expected, uint8_t actual, uint32_t lineNumber)
    {
        if (expected != actual)
        {
            TRACE_FORMAT_STRING("Assert %u == %u failed, line=%u\n", expected, actual, lineNumber);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void AssertStrAreEqual(uint8_t* pExpected, uint8_t* pActual, uint16_t size, uint32_t lineNumber)
    {
        if (memcmp(pExpected, pActual, size) != 0)
        {
            TRACE_FORMAT_STRING("Assert '%s' == '%s' (size = %u) failed, line=%u\n", pExpected, pActual, size, lineNumber);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void AssertFloatIsLessThanOrEqual(float value1, float value2, uint32_t lineNumber)
    {
        if (value1 > value2)
        {
            // TODO sprintf doesn't work for floats.
            // TODO need to convert floats to str manually (didn't find a way to enable this functionality in embedded)
            TRACE_FORMAT_STRING("Assert %f <= %f failed, line=%u\n", value1, value2, lineNumber);
            ERROR_TEST_FAILED();
            HANG();
        }
    }

    void executeTests()
    {
        TRACE_STRING("\n");
        RUN_TEST_CLASS(nmeaBuffer_advanceUint8Index);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReadMessage);
        RUN_TEST_CLASS(nmeaBuffer_nmeaReceiveCharacter);
        RUN_TEST_CLASS(afsk_encodeAx25MessageAsAfsk_perf);
        RUN_TEST_CLASS(ax25_encodeAndAppendDataAsAx25_perf);
        TRACE_STRING("Done!");
    }

#endif
