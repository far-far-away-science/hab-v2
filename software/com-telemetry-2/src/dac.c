/*
 * dac.c - Audio output over DAC for APRS/HX1
 */

#include <main.h>
#include <dac.h>
#include "fat_sd/ff.h"
#include <periph.h>

// Useful macro for reading from files
#define F_READ(_file, _dest, _read) f_read((_file), &(_dest), sizeof(_dest), &(_read))
// Scales the audio to size and re-centers to match amplifier reference to fix click and pop
// Need to convert a signed 16-bit sample to the DAC levels (11 bits)
#define SCALE_AUDIO(x) ((uint16_t)(((int32_t)(x) + 32775) >> 4) + 128U)
// Wave buffer size, 2 buffers are allocated so array is twice this
#define WAVE_BUFFER 256
// Bitstream buffer size
#define APRS_BITSTREAM_SIZE 384

struct {
	// Offset into the bitstream
	uint32_t bit;
	// Phase counter for the waves
	uint32_t phase;
} audioState;

// WAVE file buffers
static uint8_t bitstream[APRS_BITSTREAM_SIZE];
static uint16_t waveData[WAVE_BUFFER << 1];

// Generates an AFSK bitstream into the bitstream array, returning the number of bits in
// this stream
static uint32_t generateBitStream(uint8_t *buffer, uint32_t size) {
	uint32_t bits = 32U;
	uint8_t *bs = &bitstream[0];
	// Start with a minimum of 15 ones with no stuff, we use 24 (3 whole bytes)
	*bs++ = 0xFFU;
	*bs++ = 0xFFU;
	*bs++ = 0xFFU;
	// Frame separator = 0x7E
	*bs++ = 0x7EU;
}

// Loads the buffer with wave data
static uint32_t loadBuffer(uint16_t *buffer, uint32_t size) {
	// Temporary buffer
	int16_t temp[WAVE_BUFFER], *ptr = temp;
	if (size > 0) {
		// If at EOF, fill buffer with zeroes
		for (uint32_t i = size; i < WAVE_BUFFER; i++)
			temp[i] = 0U;
		// Scale audio to stay out of the saturation regions and convert signed to unsigned
		ptr = temp;
		for (uint32_t i = WAVE_BUFFER; i; i--)
			*buffer++ = SCALE_AUDIO(*ptr++);
	}
	return size;
}

// Starts up audio by preparing the DAC/timer and buffer (does not precharge the caps!)
static INLINE void setupAudio(const uint32_t size) {
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
	uint32_t size = 0U;
	if (flags & FLAG_HX1_FRONT)
		// Front half of buffer needs filling
		size = loadBuffer(&waveData[0], size);
	else if (flags & FLAG_HX1_BACK)
		// Back half of buffer needs filling
		size = loadBuffer(&waveData[WAVE_BUFFER], size);
	// If audio is complete, shut down
	if (size == 0U)
		audioStop();
	return size == 0U;
}

// Shuts down the audio pins
void audioShutdown(void) {
	// Disable the HX1
	ioSetOutput(PIN_HX1_EN, false);
	// Turn off the DAC
	DAC->CR &= ~DAC_CR_EN1;
	// Stop the SD card
	sdShutdown();
}

// Finishes audio by stopping the DAC/timer (does not shutdown amp!)
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
}
