#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

extern UART_HandleTypeDef g_copernicusUartHandle;

void SysTick_Handler(void) {
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void COPERNICUS_USART_IRQHandler(void) {
    // read g_copernicusUartHandle.Instance->RDR
    //
    HAL_UART_IRQHandler(&g_copernicusUartHandle);
    g_copernicusUartHandle.Instance->RDR = 0;
    g_copernicusUartHandle.ErrorCode = HAL_UART_ERROR_NONE;
    g_copernicusUartHandle.State = HAL_UART_STATE_BUSY_RX;
}
