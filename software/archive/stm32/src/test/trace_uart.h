#pragma once

#include "test/test.h"

#ifdef TEST

    #include <stm32l0xx_hal_dma.h>
    #include <stm32l0xx_hal_uart.h>

    void traceUartInit(void);
    void traceUartDeInit(void);
    void traceUartMspInit(UART_HandleTypeDef* pUart);
    void traceUartMspDeInit(UART_HandleTypeDef* pUart);
    void transmitBuffer(uint8_t* pBuffer, uint8_t bufferSize);

#else

    #define traceUartMspInit(pUart)
    #define traceUartMspDeInit(pUart)

#endif
