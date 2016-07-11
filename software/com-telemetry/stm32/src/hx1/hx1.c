#include "hx1.h"

#include <stm32l0xx_hal_rcc.h>
#include <stm32l0xx_hal_gpio.h>
#include <stm32l0xx_hal_dac_ex.h>
#include <stm32l0xx_hal_cortex.h>

#include <aprs/aprs.h>
#include <aprs/afsk.h>
#include <aprs/generated/afsk.h>

#include <signals/signals.h>

#include "../mxconstants.h"

#define HALF_BUFFER_LENGTH 128
#define FULL_BUFFER_LENGTH ((HALF_BUFFER_LENGTH) * 2)

Ax25EncodedMessage g_ax25EncodedAprsMessage = { 0 };

static bool g_aprsMessageTransmitting = false;

static AfskContext g_afskContext = { 0 };
static DAC_HandleTypeDef g_hx1DacHandle = { 0 };
static DAC_ChannelConfTypeDef g_hx1DacConfig = { 0 };

static uint16_t g_DacBuffer[FULL_BUFFER_LENGTH] = { 0 };

void hx1GpioInit(void);
void hx1DacInit(void);
void hx1TimerInit(void);

bool isAprsMessageTransmitting(void)
{
    return g_aprsMessageTransmitting;
}

void enableHx1(void)
{
    hx1GpioInit();
    hx1DacInit();
    hx1TimerInit();

    HAL_GPIO_WritePin(HX1_ENABLE_GPIO_Port, HX1_ENABLE_Pin, GPIO_PIN_SET);
}

void hx1GpioInit(void)
{
    HX1_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef gpioInitStruct = { 0 };

    gpioInitStruct.Pin   = HX1_ENABLE_Pin;
    gpioInitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInitStruct.Pull  = GPIO_NOPULL;
    gpioInitStruct.Speed = GPIO_SPEED_LOW;

    HAL_GPIO_Init(HX1_ENABLE_GPIO_Port, &gpioInitStruct);
    HAL_GPIO_WritePin(HX1_ENABLE_GPIO_Port, HX1_ENABLE_Pin, GPIO_PIN_RESET);
}

void hx1DacInit(void)
{
    g_hx1DacHandle.Instance = HX1_DAC;
}

void hx1DacMspInit(DAC_HandleTypeDef* pDac)
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

void hx1DacMspDeInit(DAC_HandleTypeDef* pDac)
{
    HX1_DAC_FORCE_RESET();
    HX1_DAC_RELEASE_RESET();
    HAL_GPIO_DeInit(HX1_GPIO_PORT, HX1_GPIO_PIN);
    HAL_DMA_DeInit(pDac->DMA_Handle1);
    HAL_NVIC_DisableIRQ(HX1_DMA_IRQn);
}

void hx1TimerInit(void)
{
    const uint32_t period = (uint32_t) (SystemCoreClock / APRS_SIGNAL_GENERATION_FREQUENCY) - 1;

    static TIM_HandleTypeDef hx1TimerHandle;

    hx1TimerHandle.Instance           = HX1_TIMER;
    hx1TimerHandle.Init.Period        = period - 1;
    hx1TimerHandle.Init.Prescaler     = 0;
    hx1TimerHandle.Init.ClockDivision = 0;
    hx1TimerHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&hx1TimerHandle);

    TIM_MasterConfigTypeDef masterConfig = { 0 };
    masterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    masterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&hx1TimerHandle, &masterConfig);

    HAL_TIM_Base_Start(&hx1TimerHandle);
}

void hx1TimerMspInit(TIM_HandleTypeDef* pTimer)
{
    HX1_TIMER_CLK_ENABLE();
}

void hx1TimerMspDeInit(TIM_HandleTypeDef* pTimer)
{
    HX1_TIMER_FORCE_RESET();
    HX1_TIMER_RELEASE_RESET();
}

void stopDacAndDisableHx1(void)
{
    HAL_DAC_DeInit(&g_hx1DacHandle);
    HAL_GPIO_WritePin(HX1_ENABLE_GPIO_Port, HX1_ENABLE_Pin, GPIO_PIN_RESET);
    g_aprsMessageTransmitting = false;
}

void transmitAprsMessage(void)
{
    stopDacAndDisableHx1();

    if (isAx25MessageEmtpy(&g_ax25EncodedAprsMessage))
    {
        return;
    }

    if (HAL_DAC_Init(&g_hx1DacHandle) != HAL_OK)
    {
        ERROR_HX1(ED_HX1_FAILED_TO_INITIALIZE_DAC);
    }

    g_hx1DacConfig.DAC_Trigger      = HX1_DAC_TRIGGER;
    g_hx1DacConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&g_hx1DacHandle, &g_hx1DacConfig, HX1_DAC_CHANNEL) != HAL_OK)
    {
        ERROR_HX1(ED_HX1_FAILED_TO_CONFIGURE_DAC_CHANNEL);
    }

    resetAfskContext(&g_afskContext);

    if (encodeAx25MessageAsAfsk(&g_ax25EncodedAprsMessage, &g_afskContext, g_DacBuffer, FULL_BUFFER_LENGTH))
    {
        enableHx1();
        if (HAL_DAC_Start_DMA(&g_hx1DacHandle, HX1_DAC_CHANNEL, (uint32_t*) g_DacBuffer, FULL_BUFFER_LENGTH, HX1_DAC_ALIGN) != HAL_OK)
        {
            stopDacAndDisableHx1();
            ERROR_HX1(ED_HX1_FAILED_TO_START_DAC_DMA);
        }
        g_aprsMessageTransmitting = true;
    }
    else
    {
        stopDacAndDisableHx1();
    }
}

void HX1_DMA_IRQHandler(void)
{
    HAL_DMA_IRQHandler(g_hx1DacHandle.DMA_Handle1);
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* pDac)
{
    if (g_aprsMessageTransmitting)
    {
        // fill in 1st half of the buffer
        g_aprsMessageTransmitting = encodeAx25MessageAsAfsk(&g_ax25EncodedAprsMessage, &g_afskContext, g_DacBuffer, HALF_BUFFER_LENGTH);
        // continue transmission as we filled 2nd half of the buffer (this is 1/2 completion event after all)
    }
    else
    {
        stopDacAndDisableHx1();
    }
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* pDdac)
{
    if (g_aprsMessageTransmitting)
    {
        // fill in 2nd half of the buffer
        g_aprsMessageTransmitting = encodeAx25MessageAsAfsk(&g_ax25EncodedAprsMessage, &g_afskContext, g_DacBuffer + HALF_BUFFER_LENGTH, HALF_BUFFER_LENGTH);
        // continue transmission as we filled 1st half of the buffer
    }
    else
    {
        stopDacAndDisableHx1();
    }
}
