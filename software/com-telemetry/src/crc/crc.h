#pragma once

#include <stdint.h>

void resetCrc(void);
void calculateCrc(uint8_t data);
uint16_t getCalculatedCrc();
