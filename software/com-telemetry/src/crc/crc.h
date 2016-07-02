#pragma once

#include <stdint.h>

void resetCrc(uint32_t polynomial);
void calculateCrc(uint8_t data);
uint16_t getCalculatedCrc();
void disableCrc();
