/*
 * ax25.c - Bridge between APRS protocol and raw DAC commands by generating the bitstream
 * required for audio interrupts
 */

#include <main.h>
#include <ax25.h>
#include <dac.h>
#include <periph.h>

// Bit stuffing counter
static struct {
	uint16_t bitOnes;
	uint16_t lastBit;
} bitStuff;

// Handles transform of the actual bit stream into audio samples
static volatile struct {
	// Offset into the bitstream
	uint32_t bit;
	// Phase counter for the waves
	uint32_t phase;
	// Total number of bits left in the bitstream
	uint32_t size;
	// Samples left in the current bit time
	int32_t time;
} audioState;

// Bitstream buffer (packed bits, LSB first)
// Note that the FCS is MSB first and needs to be reversed before going into here!
static uint32_t bitstream[APRS_BITSTREAM_SIZE];

// Adds one bit to the current bitstream position, updating the pointer if necessary
static uint32_t* addBit(uint32_t *bs, const uint32_t bitCount, const uint32_t bit) {
	const uint32_t place = bitCount & 31U;
	if (bit)
		*bs |= 1U << place;
	// If 31st bit was added, roll over
	if (place == 31U)
		bs++;
	return bs;
}

// Adds the specified byte to the global bitstream at the specified position
static uint32_t addBits(uint32_t bitCount, uint8_t data, bool stuff) {
	uint32_t ones = (uint32_t)bitStuff.bitOnes, last = (uint32_t)bitStuff.lastBit;
	uint32_t *bs = &bitstream[bitCount >> 5];
	for (uint32_t i = 8U; i; i--) {
		// LSB First
		if (data & 0x01U) {
			ones++;
			// Ones = repeat previous bit
			if (ones >= 5U) {
				// Stuff a zero after 5 consecutive ones if bit stuffing enabled
				// Even if disabled, keep count of ones if stuffing gets turned on later
				ones = 0U;
				if (stuff) {
					bs = addBit(bs, bitCount++, last);
					// Swap previous bit (one is added first, then the zero)
					last = !last;
				}
			}
		} else {
			// Zero, swap previous bit and reset the bit stuff counter
			ones = 0U;
			last = !last;
		}
		bs = addBit(bs, bitCount++, last);
		data >>= 1;
	}
	// Store state for next bits
	bitStuff.bitOnes = (uint16_t)ones;
	bitStuff.lastBit = (uint16_t)last;
	return bitCount;
}

// Generates an AFSK AX.25 bitstream into the bitstream array, returning the number of bits in
// this stream (size must be greater than zero)
uint32_t ax25GenerateBits(const uint8_t *buffer, uint32_t size) {
	uint32_t bits = 32U, fcs, value;
	uint32_t *bs = &bitstream[0], *bsZero = bs;
	bitStuff.bitOnes = 0U;
	bitStuff.lastBit = 1U;
	for (uint32_t i = APRS_BITSTREAM_SIZE; i; i--)
		*bsZero++ = 0U;
	// Initialize CRC
	CRC->CR |= CRC_CR_RESET;
	CRC->POL = 0x1021U;
	// Start with a minimum of 15 ones with no stuff, we use 16 (2 whole bytes) LSB first
	// Then put a sequence of 0x7E bytes (frame separator)
	*bs++ = 0xFFFFFFFFU;
	for (uint32_t i = 6U; i; i--)
		bits = addBits(bits, 0x7EU, false);
	// Data
	do {
		value = *buffer++;
		CRC->DR8 = value;
		bits = addBits(bits, value, true);
	} while (--size);
	// FCS (note that the CRC system swapped the bits for us, so send "LSB First")
	fcs = ~(CRC->DR16);
	bits = addBits(bits, (uint8_t)fcs, true);
	bits = addBits(bits, (uint8_t)(fcs >> 8), true);
	// Frame separator
	bits = addBits(bits, 0x7EU, false);
	bits = addBits(bits, 0xFFU, false);
	// Ones at the end
	return addBits(bits, 0xFFU, false);
}

#ifdef DEBUG_APRS
#include "comm.h"

// Dumps the bitstream to printf for debugging
void audioDebugStream() {
	uint8_t *bs = &bitstream[0];
	uint32_t size = audioState.size, i, byt;
	while (size) {
		byt = (uint32_t)*bs++;
		// Print the byte LSB first, trailing zeroes do not matter
		for (i = 8U; i && size; i--) {
			serialWriteByte((byt & 1U) ? '1' : '0');
			byt >>= 1;
			// Decrement the bit just printed
			size--;
		}
	}
}
#endif

// Initializes the AX25 audio encoder to generate wave samples for the specified number of bits
// Use the output from ax25GenerateBits
void ax25Init(uint32_t bits) {
	audioState.phase = 0U;
	audioState.bit = 0U;
	// Select first bit
	audioState.size = bits;
	audioState.time = BIT_TIME;
}

// Loads the specified buffer with wave samples based on the current AX.25 bit stream position
//  Up to WAVE_BUFFER bits are loaded, subject to limits on the number of bits left to send
//  Returns true if more samples are pending (the entire array is filled even if only one sample
//  needs to be sent) or false if nothing needs to be done and transmission is over
bool ax25LoadBuffer(uint16_t *buffer) {
	uint32_t left = WAVE_BUFFER, size = audioState.size, phase = audioState.phase;
	if (size == 0U)
		// If zero, use phase to count down 2 periods
		phase--;
	else {
		uint32_t bit = audioState.bit, advance;
		int32_t time = audioState.time;
		do {
			advance = (bitstream[bit >> 5] & (1U << (bit & 31U))) ? PHASE_12 : PHASE_22;
			// Run out the current bit time
			while (time > 0 && left > 0U) {
				// Scale audio to stay out of the saturation regions and convert signed to
				// unsigned
				int32_t sample = sinfp(phase);
				*buffer++ = SCALE_AUDIO(sample);
				phase += advance;
				// Wrap around
				if (phase > B_RAD)
					phase -= B_RAD;
				time -= (1 << F_S);
				left--;
			}
			// If we are at a bit time end, hold the phase constant and move bit up one
			if (time <= 0) {
				time += BIT_TIME;
				bit++;
				size--;
			}
		} while (size > 0U && left > 0U);
		// If out of time, reset phase to two to count out the buffer end
		if (size == 0U)
			phase = 2U;
		// Store updated sample generation for the next buffer fill
		audioState.bit = bit;
		audioState.time = time;
		audioState.size = size;
	}
	// Finish buffer with 0x0 if necessary
	for (; left; left--)
		*buffer++ = SCALE_AUDIO(0);
	audioState.phase = phase;
	return size == 0U && phase == 0U;
}
