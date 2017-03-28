#include "test_data.h"

#ifdef TEST
    #include <string.h>

    AfskContext g_testAfskCtx = { 0 };
    uint8_t g_testUint8Buffer[TEST_BUFFER_SIZE] = { 0 };
    uint16_t g_testUint16Buffer[TEST_BUFFER_SIZE] = { 0 };
    Ax25EncodingContext g_testAx25EncodingCtx = { 0 };
    Ax25EncodedMessage g_testAx25EncodedMessage = { 0 };

    void resetTestData(void)
    {
        memset(&g_testUint8Buffer, 0, TEST_BUFFER_SIZE);
        memset(&g_testUint16Buffer, 0, sizeof(uint16_t) * TEST_BUFFER_SIZE);

        memset(&g_testAfskCtx, 0, sizeof(AfskContext));
        memset(&g_testAx25EncodingCtx, 0, sizeof(Ax25EncodingContext));

        memset(&g_testAx25EncodedMessage, 0, sizeof(Ax25EncodedMessage));
    }

#endif
