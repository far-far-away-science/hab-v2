#pragma once

#ifdef TEST
    #include "../afsk.h"
    #include "../aprs_impl.h"

    #define TEST_DAC_BUFFER_SIZE 128

    extern AfskContext g_testAfskContext;
    extern uint16_t g_testDacBuffer[TEST_DAC_BUFFER_SIZE];
    extern Ax25EncodingContext g_testEncodingCtx;
    extern Ax25EncodedMessage g_testAx25EncodedMessage;

    void resetTestData(void);
#endif
