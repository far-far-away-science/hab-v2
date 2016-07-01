#include "test/test.h"
#include "trace_uart.h"

#ifdef TEST

    #include <string.h>
    #include <stdarg.h>

    static uint8_t g_buffer[256];

    void TRACE_STRING(const char* pString)
    {
        transmitBuffer((uint8_t*) pString, strlen(pString));
        transmitBuffer((uint8_t*) "\n", 1);
    }

    void TRACE_STRING_NO_EOL(const char* pString)
    {
        transmitBuffer((uint8_t*) pString, strlen(pString));
    }

    void TRACE_FORMAT_STRING(const char* pFormat, ...)
    {
        va_list args;
        va_start(args, pFormat);
        int size = sprintf((char*) g_buffer, pFormat, args);
        va_end(args);
        transmitBuffer((uint8_t*) g_buffer, size);
    }

#endif
