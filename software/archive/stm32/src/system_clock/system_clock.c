#include "system_clock.h"

#include <stm32l0xx_hal_pwr.h>
#include <stm32l0xx_hal_rcc.h>
#include <stm32l0xx_hal_flash.h>
#include <stm32l0xx_hal_cortex.h>

void configSystemClock(void)
{
    RCC_OscInitTypeDef rccOscInitStruct = { 0 };

    rccOscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI |
                                           RCC_OSCILLATORTYPE_LSE;
    rccOscInitStruct.HSIState            = RCC_HSI_ON;
    rccOscInitStruct.LSEState            = RCC_LSE_ON;
    rccOscInitStruct.HSICalibrationValue = 16;
    rccOscInitStruct.PLL.PLLState        = RCC_PLL_NONE;

    HAL_RCC_OscConfig(&rccOscInitStruct);

    RCC_ClkInitTypeDef rccClkInitStruct = { 0 };

    rccClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK |
                                      RCC_CLOCKTYPE_SYSCLK |
                                      RCC_CLOCKTYPE_PCLK1 |
                                      RCC_CLOCKTYPE_PCLK2;
    rccClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    rccClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    rccClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    rccClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&rccClkInitStruct, FLASH_LATENCY_0);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    __HAL_RCC_PWR_CLK_DISABLE();

    /* LSE is configured to provide clock to the LPUART */
    RCC_PeriphCLKInitTypeDef periphClkInit = {0};

    periphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_LPUART1;
    periphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&periphClkInit);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
