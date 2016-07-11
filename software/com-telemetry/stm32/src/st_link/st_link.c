#include "st_link.h"

#include <stm32l0xx_hal_rcc.h>
#include <stm32l0xx_hal_gpio.h>
#include <stm32l0xx_hal_gpio_ex.h>

void stLinkInit(void)
{
    GPIO_InitTypeDef gpioInitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpioInitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    gpioInitStruct.Mode      = GPIO_MODE_AF_PP;
    gpioInitStruct.Pull      = GPIO_NOPULL;
    gpioInitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioInitStruct.Alternate = GPIO_AF4_USART2;

    HAL_GPIO_Init(GPIOA, &gpioInitStruct);
}
