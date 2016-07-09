#include "signals_impl.h"

#include "mxconstants.h"

#include <stm32l0xx_hal_tim.h>

ErrorsDetails g_errorsDetails = { 0 };

static uint8_t g_errorsMask = 0;
static TIM_HandleTypeDef g_timHandle = { 0 };
static TIM_OC_InitTypeDef g_timOutputConfig = { 0 };

#define PERIOD_VALUE 2
#define PULSE_VALUE  1

void initializeSignals(void)
{
    const uint32_t prescalerValue = (uint32_t) (SystemCoreClock / 10000) - 1;

    g_timHandle.Instance           = SIGNAL_TIMER;
    g_timHandle.Init.Prescaler     = prescalerValue;
    g_timHandle.Init.Period        = PERIOD_VALUE;
    g_timHandle.Init.ClockDivision = 0;
    g_timHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

    if (HAL_TIM_PWM_Init(&g_timHandle) != HAL_OK)
    {
        ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_INITIALIZE);
    }

    g_timOutputConfig.OCMode     = TIM_OCMODE_PWM1;
    g_timOutputConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    g_timOutputConfig.OCFastMode = TIM_OCFAST_DISABLE;

    g_timOutputConfig.Pulse = PULSE_VALUE;
    if (HAL_TIM_PWM_ConfigChannel(&g_timHandle, &g_timOutputConfig, SIGNAL_TIMER_CHANNEL_RED) != HAL_OK)
    {
        ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_CONFIGURE_CHANNEL_RED);
    }

    g_timOutputConfig.Pulse = PULSE_VALUE;
    if (HAL_TIM_PWM_ConfigChannel(&g_timHandle, &g_timOutputConfig, SIGNAL_TIMER_CHANNEL_GREEN) != HAL_OK)
    {
        ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_CONFIGURE_CHANNEL_GREEN);
    }

    g_timOutputConfig.Pulse = PULSE_VALUE;
    if (HAL_TIM_PWM_ConfigChannel(&g_timHandle, &g_timOutputConfig, SIGNAL_TIMER_CHANNEL_BLUE) != HAL_OK)
    {
        ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_CONFIGURE_CHANNEL_BLUE);
    }
}

uint8_t getErrorsMask()
{
    return g_errorsMask;
}

void addError(uint8_t errorId, uint32_t* pErrorDetails, uint32_t errorDetailId)
{
    g_errorsMask |= errorId;
    *pErrorDetails |= errorDetailId;
}

void addErrorWithoutDetails(uint8_t errorId)
{
    g_errorsMask |= errorId;
}

void resetErrors()
{
    g_errorsMask = 0;
}

void signalError(bool signal)
{
    if (signal)
    {
        if (HAL_TIM_PWM_Start(&g_timHandle, SIGNAL_TIMER_CHANNEL_RED) != HAL_OK)
        {
            ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_SIGNAL_RED);
        }
    }
    else
    {
        HAL_TIM_PWM_Stop(&g_timHandle, SIGNAL_TIMER_CHANNEL_RED);
    }
}

void signalInitialized(bool signal)
{
    if (signal)
    {
        if (HAL_TIM_PWM_Start(&g_timHandle, SIGNAL_TIMER_CHANNEL_GREEN) != HAL_OK)
        {
            ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_SIGNAL_GREEN);
        }
    }
    else
    {
        HAL_TIM_PWM_Stop(&g_timHandle, SIGNAL_TIMER_CHANNEL_GREEN);
    }
}

void signalTransmitting(bool signal)
{
    if (signal)
    {
        if (HAL_TIM_PWM_Start(&g_timHandle, SIGNAL_TIMER_CHANNEL_BLUE) != HAL_OK)
        {
            ERROR_SIGNA_TIMER(ED_ST_FAILED_TO_SIGNAL_BLUE);
        }
    }
    else
    {
        HAL_TIM_PWM_Stop(&g_timHandle, SIGNAL_TIMER_CHANNEL_BLUE);
    }
}

void initializeSignalsTimer(TIM_HandleTypeDef* pTimer)
{
    if (pTimer->Instance == SIGNAL_TIMER)
    {
        SIGNAL_TIMER_CLK_ENABLE();
        SIGNAL_TIMER_CHANNEL_GPIO_PORT();

        GPIO_InitTypeDef gpioInitStruct = { 0 };

        gpioInitStruct.Mode  = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull  = GPIO_PULLUP;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        gpioInitStruct.Alternate = SIGNAL_GPIO_AF_CHANNEL_RED;
        gpioInitStruct.Pin = SIGNAL_GPIO_PIN_RED;
        HAL_GPIO_Init(SIGNAL_GPIO_CHANNEL_RED, &gpioInitStruct);

        gpioInitStruct.Alternate = SIGNAL_GPIO_AF_CHANNEL_GREEN;
        gpioInitStruct.Pin = SIGNAL_GPIO_PIN_GREEN;
        HAL_GPIO_Init(SIGNAL_GPIO_CHANNEL_GREEN, &gpioInitStruct);

        gpioInitStruct.Alternate = SIGNAL_GPIO_AF_CHANNEL_BLUE;
        gpioInitStruct.Pin = SIGNAL_GPIO_PIN_BLUE;
        HAL_GPIO_Init(SIGNAL_GPIO_CHANNEL_BLUE, &gpioInitStruct);
    }
}
