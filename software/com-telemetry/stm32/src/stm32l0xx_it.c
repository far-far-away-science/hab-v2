#include <gps/nmeaBuffer.h>
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

#include "uart.h"
#include "errors.h"

extern UART_HandleTypeDef g_copernicusUartHandle;
extern NmeaRingBuffer g_nmeaRingBuffer;

void SysTick_Handler(void) {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void COPERNICUS_LPUART_IRQHandler(void) {
    uint8_t character;

    if (UART_GetCharacter(&g_copernicusUartHandle, &character)) {
        if (g_copernicusUartHandle.ErrorCode & HAL_UART_ERROR_ORE) {
            // reset overrun error
            __HAL_UART_FLUSH_DRREGISTER(&g_copernicusUartHandle);
        }

        const bool hasError = g_copernicusUartHandle.ErrorCode != HAL_UART_ERROR_NONE;

        nmeaReceiveCharacter(&g_nmeaRingBuffer, character, hasError);

        if (hasError)
        {
            ERROR_UART();
            // we don't need to remember the error
            g_copernicusUartHandle.ErrorCode = HAL_UART_ERROR_NONE;
        }
    }
}
