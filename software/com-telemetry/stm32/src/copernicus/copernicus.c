#include "copernicus.h"

#include <stdbool.h>
#include <mxconstants.h>

#include <stm32l0xx_hal_rcc.h>
#include <stm32l0xx_hal_rcc_ex.h>
#include <stm32l0xx_hal_cortex.h>

#include <signals/signals.h>

#include "../uart/uart.h"

NmeaRingBuffer g_nmeaRingBuffer = { 0 };

static UART_HandleTypeDef g_copernicusUartHandle = { 0 };

void copernicusUartInit(void)
{
    g_copernicusUartHandle.Instance                    = COPERNICUS_UART;
    g_copernicusUartHandle.Init.BaudRate               = 4800;
    g_copernicusUartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
    g_copernicusUartHandle.Init.StopBits               = UART_STOPBITS_1;
    g_copernicusUartHandle.Init.Parity                 = UART_PARITY_NONE;
    g_copernicusUartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_copernicusUartHandle.Init.Mode                   = UART_MODE_RX;
    g_copernicusUartHandle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    g_copernicusUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&g_copernicusUartHandle) != HAL_OK)
    {
        ERROR_UART_COPERNICUS(ED_UC_FAILED_TO_INITIALIZE);
    }

    if (EnableUart2ReceiveData(&g_copernicusUartHandle) != HAL_OK)
    {
        ERROR_UART_COPERNICUS(ED_UC_FAILED_TO_ENABLE_RECEIVE_DATA);
    }
}

void copernicusUartMspInit(UART_HandleTypeDef* pUart)
{
    GPIO_InitTypeDef gpioInitStruct = { 0 };

    COPERNICUS_GPIO_CLK_ENABLE();
    COPERNICUS_UART_CLK_ENABLE();

    gpioInitStruct.Pin = COPERNICUS_UART_TX_Pin | COPERNICUS_UART_RX_Pin;
    gpioInitStruct.Mode = GPIO_MODE_AF_PP;
    gpioInitStruct.Pull = GPIO_PULLUP;
    gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInitStruct.Alternate = COPERNICUS_GPIO_AF4_UART;
    HAL_GPIO_Init(COPERNICUS_GPIO, &gpioInitStruct);

    HAL_NVIC_SetPriority(COPERNICUS_UART_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(COPERNICUS_UART_IRQn);
}

void copernicusUartMspDeInit(UART_HandleTypeDef* pUart)
{
    COPERNICUS_UART_CLK_DISABLE();
    COPERNICUS_UART_FORCE_RESET();
    COPERNICUS_UART_RELEASE_RESET();
    HAL_GPIO_DeInit(COPERNICUS_GPIO, COPERNICUS_UART_TX_Pin | COPERNICUS_UART_RX_Pin);
    HAL_NVIC_DisableIRQ(COPERNICUS_UART_IRQn);
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
            ERROR_UART_COPERNICUS(ED_UC_ERROR_WHILE_RECEIVING_DATA);
            // we don't need to remember the error
            g_copernicusUartHandle.ErrorCode = HAL_UART_ERROR_NONE;
        }
    }
}
