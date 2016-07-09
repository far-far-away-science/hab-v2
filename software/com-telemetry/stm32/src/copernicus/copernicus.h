#pragma once

#include <stm32l0xx_hal_dma.h>
#include <stm32l0xx_hal_uart.h>

#include <gps/nmea_buffer.h>

extern NmeaRingBuffer g_nmeaRingBuffer;

void copernicusUartInit(void);
void copernicusUartMspInit(UART_HandleTypeDef* pUart);
void copernicusUartMspDeInit(UART_HandleTypeDef* pUart);
