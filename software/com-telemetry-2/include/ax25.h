/*
 * ax25.h - Bridge between APRS protocol and raw DAC commands by generating the bitstream
 * required for audio interrupts
 */

#ifndef AX25_H_
#define AX25_H_

#include <stdint.h>

// Bitstream buffer size
#define APRS_BITSTREAM_SIZE 96

// Generates an AFSK AX.25 bitstream into the bitstream array, returning the number of bits in
// this stream (size must be greater than zero)
uint32_t ax25GenerateBits(const uint8_t *buffer, uint32_t size);
// Initializes the AX25 audio encoder to generate wave samples for the specifeid number of bits
// Use the output from ax25GenerateBits
void ax25Init(uint32_t bits);
// Loads the specified buffer with wave samples based on the current AX.25 bit stream position
bool ax25LoadBuffer(uint16_t *buffer);

#endif
