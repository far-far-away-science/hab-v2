#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "stm32l0xx_hal_gpio_ex.h"

#include "stm32l0xx_hal_dac.h"
#include "stm32l0xx_hal_dac_ex.h"

#include "stm32l0xx_hal_uart.h"
#include "stm32l0xx_hal_uart_ex.h"

#include "uart.h"
#include "test/test.h"
#include "gps/nmea_buffer.h"
#include "aprs/aprs_board.h"

#define HALF_BUFFER_LENGTH 16
#define FULL_BUFFER_LENGTH ((HALF_BUFFER_LENGTH) * 2)

UART_HandleTypeDef g_copernicusUartHandle;
TIM_HandleTypeDef g_hx1TimerHandle;
DAC_HandleTypeDef g_hx1DacHandle;
DAC_ChannelConfTypeDef g_hx1DacConfig;
uint16_t g_DacBuffer[FULL_BUFFER_LENGTH] = { 0 };

NmeaRingBuffer g_nmeaRingBuffer;

void ErrorHandler(void);

void ConfigurateMcu();

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_STLink_Init(void);
void Copernicus_Uart_Init(void);

void HX1_Timer_Init(void);
void HX1_Dac_Init(void);

void TransmitAprsMessage(void);

int main(void) {
    ConfigurateMcu();

#ifdef TEST
    EXECUTE_TESTS();

    for (;;)
    {
    }
#else
    TransmitAprsMessage();
    for (;;)
    {
    }
#endif
}

void ConfigurateMcu(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_STLink_Init();
    Copernicus_Uart_Init();
    HX1_Dac_Init();
    HX1_Timer_Init();
}

void ErrorHandler(void)
{
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_SET);
    for (;;)
    {
    }
}

/*
 * System Clock Configuration
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI |
                                            RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1 |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    __HAL_RCC_PWR_CLK_DISABLE();

    /* LSE is configured to provide clock to the LPUART */
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin   = ERROR_LD_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(ERROR_LD_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_RESET);
}

void MX_STLink_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Copernicus_Uart_Init(void) {
    g_copernicusUartHandle.Instance                    = COPERNICUS_UART;
    g_copernicusUartHandle.Init.BaudRate               = 4800;
    g_copernicusUartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
    g_copernicusUartHandle.Init.StopBits               = UART_STOPBITS_1;
    g_copernicusUartHandle.Init.Parity                 = UART_PARITY_NONE;
    g_copernicusUartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_copernicusUartHandle.Init.Mode                   = UART_MODE_RX;
    g_copernicusUartHandle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    g_copernicusUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&g_copernicusUartHandle) != HAL_OK) {
        ErrorHandler();
    }

    if (EnableUart2ReceiveData(&g_copernicusUartHandle) != HAL_OK) {
        ErrorHandler();
    }
}

void HX1_Timer_Init(void) {
    const uint32_t period = APRS_SIGNAL_GENERATION_FREQUENCY;
    const uint32_t prescalerValue = (uint32_t) (SystemCoreClock / period) - 1;

    g_hx1TimerHandle.Instance           = HX1_TIMER;
    g_hx1TimerHandle.Init.Period        = period - 1;
    g_hx1TimerHandle.Init.Prescaler     = prescalerValue;
    g_hx1TimerHandle.Init.ClockDivision = 0;
    g_hx1TimerHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&g_hx1TimerHandle);

    TIM_MasterConfigTypeDef masterConfig = { 0 };
    masterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    masterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&g_hx1TimerHandle, &masterConfig);

    HAL_TIM_Base_Start(&g_hx1TimerHandle);
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* pDac) {
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* pDdac) {
}

void HX1_Dac_Init(void) {
    g_hx1DacHandle.Instance = HX1_DAC;
}

void TransmitAprsMessage(void) {
    HAL_DAC_DeInit(&g_hx1DacHandle);

    if (HAL_DAC_Init(&g_hx1DacHandle) != HAL_OK) {
        ErrorHandler();
    }

    g_hx1DacConfig.DAC_Trigger      = HX1_DAC_TRIGGER;
    g_hx1DacConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&g_hx1DacHandle, &g_hx1DacConfig, HX1_DAC_CHANNEL) != HAL_OK) {
        ErrorHandler();
    }

    if (fillInAmplitudesBuffer(g_DacBuffer, FULL_BUFFER_LENGTH)) {
        if (HAL_DAC_Start_DMA(&g_hx1DacHandle, HX1_DAC_CHANNEL, (uint32_t*) g_DacBuffer, FULL_BUFFER_LENGTH, HX1_DAC_ALIGN) != HAL_OK) {
            ErrorHandler();
        }
    }
}
