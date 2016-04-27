#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "stm32l0xx_hal_gpio_ex.h"

#include "stm32l0xx_hal_uart.h"
#include "stm32l0xx_hal_uart_ex.h"

#include "uart.h"

UART_HandleTypeDef g_copernicusUartHandle;

void ErrorHandler(void);

void ConfigurateMcu();

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_STLink_Init(void);
void Copernicus_Uart_Init(void);

int main(void) {
    ConfigurateMcu();

    for (;;)
    {
    }
}

void ConfigurateMcu(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_STLink_Init();
    Copernicus_Uart_Init();
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
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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
