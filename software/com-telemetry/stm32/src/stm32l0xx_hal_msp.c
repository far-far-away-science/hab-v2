#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"

#include "test/trace_uart.h"
#include "signals/signals_impl.h"
#include "copernicus/copernicus.h"

#include "mxconstants.h"

/*
 * Initializes the Global MSP.
 */
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
        HX1_TIMER_CLK_ENABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == HX1_TIMER)
    {
        HX1_TIMER_FORCE_RESET();
        HX1_TIMER_RELEASE_RESET();
    }
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* pDac)
{
    if (pDac->Instance == HX1_DAC)
    {
        HX1_GPIO_CLK_ENABLE();
        HX1_DAC_CLK_ENABLE();
        HX1_DMA_CLK_ENABLE();

        GPIO_InitTypeDef gpioInitStruct = { 0 };

        gpioInitStruct.Pin  = HX1_GPIO_PIN;
        gpioInitStruct.Mode = GPIO_MODE_ANALOG;
        gpioInitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(HX1_GPIO_PORT, &gpioInitStruct);

        // it's important that this variable should be static
        static DMA_HandleTypeDef dmaDac1 = { 0 };
        dmaDac1.Instance                 = HX1_DMA_INSTANCE;
        dmaDac1.Init.Request             = DMA_REQUEST_9;
        dmaDac1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        dmaDac1.Init.PeriphInc           = DMA_PINC_DISABLE;
        dmaDac1.Init.MemInc              = DMA_MINC_ENABLE;
        dmaDac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        dmaDac1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        dmaDac1.Init.Mode                = DMA_CIRCULAR;
        dmaDac1.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
        HAL_DMA_Init(&dmaDac1);

        __HAL_LINKDMA(pDac, DMA_Handle1, dmaDac1);

        HAL_NVIC_SetPriority(HX1_DMA_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(HX1_DMA_IRQn);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* pDac)
{
    if (pDac->Instance == HX1_DAC)
    {
        HX1_DAC_FORCE_RESET();
        HX1_DAC_RELEASE_RESET();
        HAL_GPIO_DeInit(HX1_GPIO_PORT, HX1_GPIO_PIN);
        HAL_DMA_DeInit(pDac->DMA_Handle1);
        HAL_NVIC_DisableIRQ(HX1_DMA_IRQn);
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == SIGNAL_TIMER)
    {
        initializeSignalsTimer(pTimer);
    }
}
