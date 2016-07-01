#pragma once

#include <stdint.h>

void resetFcs(void);
void calculateFcs(uint8_t data);
uint16_t getCalculatedFcs();
