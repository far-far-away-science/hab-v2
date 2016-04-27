#pragma once

#include <stdbool.h>

#include "stm32l0xx_hal_dma.h"
#include "stm32l0xx_hal_uart.h"

HAL_StatusTypeDef EnableUart2ReceiveData(UART_HandleTypeDef* pUart);
void DisableUart2ReceiveData(UART_HandleTypeDef* pUart);

bool UART_GetCharacter(UART_HandleTypeDef* pUart, uint8_t* pChar);
