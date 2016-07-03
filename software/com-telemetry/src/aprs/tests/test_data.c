#include "test_data.h"

#ifdef TEST
    #include <string.h>

    AfskContext g_testAfskContext = { 0 };
    uint16_t g_testDacBuffer[TEST_DAC_BUFFER_SIZE] = { 0 };
    Ax25EncodingContext g_testEncodingCtx = { 0 };
    Ax25EncodedMessage g_testAx25EncodedMessage = { 0 };

    void resetTestData(void)
    {
        memset(&g_testDacBuffer, 0, TEST_DAC_BUFFER_SIZE);
        memset(&g_testEncodingCtx, 0, sizeof(Ax25EncodingContext));
        memset(&g_testAx25EncodedMessage, 0, sizeof(Ax25EncodedMessage));
        memset(&g_testAx25EncodedMessage, 0, sizeof(Ax25EncodedMessage));
    }

#endif
