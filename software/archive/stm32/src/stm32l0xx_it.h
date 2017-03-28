#pragma once

#include "mxconstants.h"

void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

void SysTick_Handler(void);
