#pragma once

// Initializes SPI
void spiInit(void);
// Reads multiple values over SPI while writing the specified values
void spiWriteReadBytes(void *inValue, const void *outValue, uint32_t length);
