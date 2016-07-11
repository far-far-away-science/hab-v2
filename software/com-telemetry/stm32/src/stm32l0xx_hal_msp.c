#include <stm32l073xx.h>
#include <stm32l0xx_hal.h>

#include "hx1/hx1.h"
#include "test/trace_uart.h"
#include "signals/signals_impl.h"
#include "copernicus/copernicus.h"

#include "mxconstants.h"

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef* pUart)
{
    if (pUart->Instance == COPERNICUS_UART)
    {
        copernicusUartMspInit(pUart);
    }
    else if (pUart->Instance == TRACE_UART)
    {
        traceUartMspInit(pUart);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* pUart)
{
    if (pUart->Instance == COPERNICUS_UART)
    {
        copernicusUartMspDeInit(pUart);
    }
    else if (pUart->Instance == TRACE_UART)
    {
        traceUartMspDeInit(pUart);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == HX1_TIMER)
    {
        hx1TimerMspInit(pTimer);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == HX1_TIMER)
    {
        hx1TimerMspDeInit(pTimer);
    }
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* pDac)
{
    if (pDac->Instance == HX1_DAC)
    {
        hx1DacMspInit(pDac);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* pDac)
{
    if (pDac->Instance == HX1_DAC)
    {
        hx1DacMspDeInit(pDac);
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == SIGNAL_TIMER)
    {
        initializeSignalsTimer(pTimer);
    }
}
