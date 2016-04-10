#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "stm32l0xx_hal_gpio_ex.h"

#include "stm32l0xx_hal_uart.h"
#include "stm32l0xx_hal_uart_ex.h"

UART_HandleTypeDef g_copernicusUartHandle;

void ErrorHandler(void);

void ConfigurateMcu();

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void USART2_Init(void);

int main(void) {
    ConfigurateMcu();

    uint8_t rxBuffer[100] = { 0 };

    if(HAL_UART_Receive_IT(&g_copernicusUartHandle, rxBuffer, 20) != HAL_OK)
    {
      ErrorHandler();
    }

    for (;;)
    {
    }
}

void ErrorHandler(void)
{
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_SET); // TODO need a bit better error handling
}

void ConfigurateMcu(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    USART2_Init();
}

/*
 * System Clock Configuration
 */
void SystemClock_Config(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_5;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1 |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    __GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin   = ERROR_LD_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(ERROR_LD_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_RESET);
}

void USART2_Init(void) {
    g_copernicusUartHandle.Instance = COPERNICUS_USART;
    g_copernicusUartHandle.Init.BaudRate = 4800;
    g_copernicusUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    g_copernicusUartHandle.Init.StopBits = UART_STOPBITS_1;
    g_copernicusUartHandle.Init.Parity = UART_PARITY_NONE;
    g_copernicusUartHandle.Init.Mode = UART_MODE_TX_RX;

    if (HAL_UART_Init(&g_copernicusUartHandle) != HAL_OK) {
        ErrorHandler();
    }
}
