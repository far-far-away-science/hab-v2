#pragma once

#include <stdint.h>

void resetFcs(void);
void calculateFcs(uint8_t newByte);
uint16_t getCalculatedFcs();
