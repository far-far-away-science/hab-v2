#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "stm32l0xx_it.h"

#include <stdbool.h>

#include <signals/signals.h>

void HardFault_Handler(void)
{
    ERROR_ISR_FAULT(ED_ISRF_HARD);
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    ERROR_ISR_FAULT(ED_ISRF_MEMORY);
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    ERROR_ISR_FAULT(ED_ISRF_BUS);
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    ERROR_ISR_FAULT(ED_ISRF_USAGE);
    while (1)
    {
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
