#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

#include <gps/nmea_buffer.h>
#include <signals/signals.h>
#include "uart.h"

extern UART_HandleTypeDef g_copernicusUartHandle;
extern DAC_HandleTypeDef g_hx1DacHandle;
extern NmeaRingBuffer g_nmeaRingBuffer;

void HardFault_Handler(void)
{
    signalError(true);
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    signalError(true);
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    signalError(true);
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    signalError(true);
    while (1)
    {
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void COPERNICUS_LPUART_IRQHandler(void)
{
    uint8_t character;

    if (UART_GetCharacter(&g_copernicusUartHandle, &character))
    {
        if (g_copernicusUartHandle.ErrorCode & HAL_UART_ERROR_ORE)\
        {
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

void HX1_DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(g_hx1DacHandle.DMA_Handle1);
}
