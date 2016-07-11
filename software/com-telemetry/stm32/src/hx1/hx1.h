#pragma once

#include <stdbool.h>
#include <aprs/ax25.h>

#include <stm32l0xx_hal_dma.h>
#include <stm32l0xx_hal_tim.h>
#include <stm32l0xx_hal_dac.h>

extern Ax25EncodedMessage g_ax25EncodedAprsMessage;

void hx1TimerMspInit(TIM_HandleTypeDef* pTimer);
void hx1TimerMspDeInit(TIM_HandleTypeDef* pTimer);

void hx1DacMspInit(DAC_HandleTypeDef* pDac);
void hx1DacMspDeInit(DAC_HandleTypeDef* pDac);

void transmitAprsMessage(void);
bool isAprsMessageTransmitting(void);
