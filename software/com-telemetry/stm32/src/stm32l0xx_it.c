#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

#include "uart.h"
#include "gps\nmea.h"

extern UART_HandleTypeDef g_copernicusUartHandle;

void SysTick_Handler(void) {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void COPERNICUS_LPUART_IRQHandler(void) {
    uint8_t c;

    if (UART_GetCharacter(&g_copernicusUartHandle, &c)) {
        if (g_copernicusUartHandle.ErrorCode & HAL_UART_ERROR_ORE) {
            // reset overrun error
            __HAL_UART_FLUSH_DRREGISTER(&g_copernicusUartHandle);
        }

        nmeaReceiveCharacter(c, g_copernicusUartHandle.ErrorCode != HAL_UART_ERROR_NONE);

        // we don't need to remember the error
        g_copernicusUartHandle.ErrorCode = HAL_UART_ERROR_NONE;
    }
}
