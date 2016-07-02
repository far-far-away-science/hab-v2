#include "test/test.h"
#include "trace_uart.h"

#ifdef TEST

    #include <string.h>
    #include <stdarg.h>

    void TRACE_STRING(const char* pString)
    {
        transmitBuffer((uint8_t*) pString, strlen(pString));
        transmitBuffer((uint8_t*) "\r\n", 1);
    }

    void TRACE_STRING_NO_EOL(const char* pString)
    {
        transmitBuffer((uint8_t*) pString, strlen(pString));
    }

#endif
