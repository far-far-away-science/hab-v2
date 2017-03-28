#pragma once

#ifdef TEST
    #include "../afsk.h"
    #include "../aprs_impl.h"

    #define TEST_BUFFER_SIZE 128

    extern AfskContext g_testAfskCtx;
    extern uint8_t g_testUint8Buffer[TEST_BUFFER_SIZE];
    extern uint16_t g_testUint16Buffer[TEST_BUFFER_SIZE];
    extern Ax25EncodingContext g_testAx25EncodingCtx;
    extern Ax25EncodedMessage g_testAx25EncodedMessage;

    void resetTestData(void);
#endif
