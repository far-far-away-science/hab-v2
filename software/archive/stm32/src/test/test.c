#include <test/test.h>

#include <stm32l0xx_hal.h>

uint32_t getTimeMilliseconds(void)
{
    return HAL_GetTick();
}
