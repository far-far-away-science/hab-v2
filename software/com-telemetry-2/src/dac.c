/*
 * dac.c - Audio output over DAC for APRS/HX1
 */

#include <main.h>
#include <dac.h>
#include <periph.h>

// Because the DMA channel 4-7 interrupts are shared, the implementation needs to notify I2C
// when the DMA is done
extern volatile uint32_t i2cState;

// Scales the audio to size and re-centers to match amplifier reference to fix click and pop
// Need to convert a signed 16-bit sample to the DAC levels (11 bits)
#define SCALE_AUDIO(x) ((uint16_t)((int32_t)(x) + 32768) >> 4)
// Wave buffer size, 2 buffers are allocated so array is twice this
#define WAVE_BUFFER 256
// Bitstream buffer size
#define APRS_BITSTREAM_SIZE 384

volatile struct {
	// Offset into the bitstream
	uint32_t bit;
	// Phase counter for the waves
	uint32_t phase;
	// Total number of bits left in the bitstream
	uint32_t size;
	// Samples left in the current bit time
	int32_t time;
} audioState;

// Bit stuffing counter
static struct {
	uint32_t bitOnes;
	uint32_t lastBit;
} bitStuff;
// Bitstream buffer (packed bits, LSB first)
// Note that the FCS is MSB first and needs to be reversed before going into here!
static uint8_t bitstream[APRS_BITSTREAM_SIZE];
// Buffer for DAC
static uint16_t waveData[WAVE_BUFFER << 1];

// Adds one bit to the current bitstream position, updating the pointer if necessary
static uint8_t* addBit(uint8_t *bs, const uint32_t bitCount, const uint32_t bit) {
	const uint32_t place = bitCount & 7U;
	if (bit)
		*bs |= 1U << place;
	// If 7th bit was added, roll over
	if (place == 7U)
		bs++;
	return bs;
}

// Adds the specified byte to the global bitstream at the specified position
static uint32_t addBits(uint32_t bitCount, uint8_t data, bool stuff) {
	uint32_t ones = bitStuff.bitOnes, last = bitStuff.lastBit;
	uint8_t *bs = &bitstream[bitCount >> 3];
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
	bitStuff.bitOnes = ones;
	bitStuff.lastBit = last;
	return bitCount;
}

// Generates an AFSK bitstream into the bitstream array, returning the number of bits in
// this stream (size must be greater than zero)
static uint32_t generateBitStream(const uint8_t *buffer, uint32_t size) {
	uint32_t bits = 24U, *quickZero = (uint32_t *)&bitstream[0], fcs;
	uint8_t *bs = &bitstream[0], value;
	bitStuff.bitOnes = 0U;
	bitStuff.lastBit = 1U;
	// Strict aliasing warning! But it zeroes the array way faster!
	for (uint32_t i = (APRS_BITSTREAM_SIZE >> 2); i; i--)
		*quickZero++ = 0U;
	// Initialize CRC
	CRC->CR |= CRC_CR_RESET;
	CRC->POL = 0x1021U;
	// Start with a minimum of 15 ones with no stuff, we use 24 (3 whole bytes)
	*bs++ = 0xFFU;
	*bs++ = 0xFFU;
	*bs++ = 0xFFU;
	// Frame separator = 0x7E
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
	bits = addBits(bits, 0x7EU, false);
	return bits;
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

// Loads the buffer with wave data
static uint32_t loadBuffer(uint16_t *buffer, uint32_t size) {
	uint32_t phase = audioState.phase, bit = audioState.bit, left = WAVE_BUFFER;
	int32_t time = audioState.time;
	do {
		uint32_t advance = (bitstream[bit >> 3] & (1U << (bit & 0x07U))) ? PHASE_12 : PHASE_22;
		// Run out the current bit time
		while (time > 0 && left > 0U) {
			// Scale audio to stay out of the saturation regions and convert signed to unsigned
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
	audioState.bit = bit;
	audioState.phase = phase;
	audioState.time = time;
	// Finish buffer with 0x0 if necessary
	for (; left; left--)
		*buffer++ = SCALE_AUDIO(0);
	return size;
}

// Starts up audio by preparing the DAC/timer and buffer (does not precharge the caps!)
static INLINE void setupAudio() {
	uint16_t *ptr = &waveData[0];
	// Prepare first buffer
	for (uint32_t i = (WAVE_BUFFER << 1); i; i--)
		*ptr++ = SCALE_AUDIO(0);
	// Initialize DMA
	DMA1_Channel4->CNDTR = WAVE_BUFFER << 1;
	DMA1_Channel4->CMAR = (uint32_t)&waveData[0];
	DMA1_Channel4->CCR |= DMA_CCR_EN;
	// Start DAC, generate a CCR4 event right away to force load the first DAC value
	DAC->CR = (DAC->CR & ~DAC_CR_TSEL1_BITS) | DAC_CR_TSEL1_TIM2;
	TIM2->CNT = 0U;
	TIM2->EGR = TIM_EGR_CC4G;
	TIM2->CR1 |= TIM_CR1_CEN;
	// Clear phase timers
	audioState.phase = 0U;
	audioState.bit = 0U;
	// Enable the HX1
	ioSetOutput(PIN_HX1_EN, true);
	sysFlags &= ~FLAG_HX1_ANY;
}

// Starts precharge of the audio pins
void audioInit(void) {
	// Precharge audio pin to zero level
	DAC->CR = (DAC->CR & ~DAC_CR_TSEL1_BITS) | (DAC_CR_TSEL1_SOFT | DAC_CR_EN1);
	DAC->DHR12R1 = SCALE_AUDIO(0);
	__dsb();
	DAC->SWTRIGR = DAC_SWTRIGR_SWTRIG;
}

// Processes an audio interrupt in the main loop, returns TRUE iff audio has finished
bool audioInterrupt(const uint32_t flags) {
	uint32_t size = audioState.size;
	if (flags & FLAG_HX1_FRONT)
		// Front half of buffer needs filling
		size = loadBuffer(&waveData[0], size);
	else if (flags & FLAG_HX1_BACK)
		// Back half of buffer needs filling
		size = loadBuffer(&waveData[WAVE_BUFFER], size);
	// If audio is complete, shut down
	if (size == 0U)
		audioStop();
	audioState.size = size;
	return size == 0U;
}

// Plays the specified message as an APRS bit stream over the DAC port to the HX1
uint32_t audioPlay(const void *data, uint32_t len) {
	uint32_t bits = 0U;
	if (len > 0U) {
		audioInit();
		bits = generateBitStream(data, len);
		// Select first bit
		audioState.size = bits;
		audioState.time = BIT_TIME;
		setupAudio();
	}
	return bits;
}

// Shuts down the audio pins
void audioShutdown(void) {
	// Disable the HX1
	ioSetOutput(PIN_HX1_EN, false);
	// Turn off the DAC
	DAC->CR &= ~DAC_CR_EN1;
}

// Finishes audio by stopping the DAC/timer (does not shutdown HX1!)
void audioStop(void) {
	// Tear down DMA, TIM2
	TIM2->CR1 &= ~TIM_CR1_CEN;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
}

// Audio DMA requests
void ISR_DMA1_Channel7_4() {
	const uint32_t isr = DMA1->ISR, af = sysFlags;
	if (isr & DMA_ISR_TCIF4) {
		sysFlags = af | FLAG_HX1_BACK;
		// Clear flags
		DMA1->IFCR = DMA_ISR_TCIF4;
	}
	if (isr & DMA_ISR_HTIF4) {
		sysFlags = af | FLAG_HX1_FRONT;
		// Clear flags
		DMA1->IFCR = DMA_ISR_HTIF4;
	}
	if (isr & DMA_ISR_TCIF7) {
		i2cState |= I2C_DATA_DONE;
		// Clear flags
		DMA1->IFCR = DMA_ISR_TCIF7;
	}
}
