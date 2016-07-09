#pragma once

#include <signals/signals.h>

#include <mxconstants.h>

#include <stm32l073xx.h>
#include <stm32l0xx_hal.h>
#include <stm32l0xx_hal_conf.h>

void initializeSignalsTimer(TIM_HandleTypeDef* pTimer);
