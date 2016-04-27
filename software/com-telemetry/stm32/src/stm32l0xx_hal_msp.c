#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"

/*
 * Initializes the Global MSP.
 */
void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == COPERNICUS_UART) {
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };

        COPERNICUS_GPIO_CLK_ENABLE();
        COPERNICUS_UART_CLK_ENABLE();

        GPIO_InitStruct.Pin = COPERNICUS_UART_TX_Pin | COPERNICUS_UART_RX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = COPERNICUS_GPIO_AF4_UART;
        HAL_GPIO_Init(COPERNICUS_GPIO, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(COPERNICUS_UART_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(COPERNICUS_UART_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == COPERNICUS_UART) {
        COPERNICUS_UART_CLK_DISABLE();
        COPERNICUS_UART_FORCE_RESET();
        COPERNICUS_UART_RELEASE_RESET();
        HAL_GPIO_DeInit(COPERNICUS_GPIO, COPERNICUS_UART_TX_Pin | COPERNICUS_UART_RX_Pin);
        HAL_NVIC_DisableIRQ(COPERNICUS_UART_IRQn);
    }
}
