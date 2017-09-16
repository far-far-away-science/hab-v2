/*
 * dac.c - Audio output over DAC for APRS/HX1
 */

#include <main.h>
#include <ax25.h>
#include <dac.h>
#include <periph.h>

// Because the DMA channel 4-7 interrupts are shared, the implementation needs to notify I2C
// when the DMA is done
extern volatile uint32_t i2cState;

// Buffer for DAC
static uint16_t waveData[WAVE_BUFFER << 1];

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
	sysFlags &= ~FLAG_HX1_ANY;
}

// Starts precharge of the audio pins
void audioInit(void) {
	// Precharge audio pin to zero level
	DAC->CR = (DAC->CR & ~DAC_CR_TSEL1_BITS) | (DAC_CR_TSEL1_SOFT | DAC_CR_EN1);
	DAC->DHR12R1 = SCALE_AUDIO(0);
	__dsb();
	DAC->SWTRIGR = DAC_SWTRIGR_SWTRIG;
	// Enable the HX1
	ioSetOutput(PIN_HX1_EN, true);
}

// Processes an audio interrupt in the main loop, returns TRUE iff audio has finished
bool audioInterrupt(const uint32_t flags) {
	bool done = true;
	if (flags & FLAG_HX1_FRONT)
		// Front half of buffer needs filling
		done = ax25LoadBuffer(&waveData[0]);
	else if (flags & FLAG_HX1_BACK)
		// Back half of buffer needs filling
		done = ax25LoadBuffer(&waveData[WAVE_BUFFER]);
	return done;
}

// Plays the specified message as an APRS bit stream over the DAC port to the HX1
uint32_t audioPlay(const void *data, uint32_t len) {
	uint32_t bits = 0U;
	if (len > 0U) {
		bits = ax25GenerateBits(data, len);
		ax25Init(bits);
		setupAudio();
	}
	return bits;
}

// Shuts down the audio pins
void audioShutdown(void) {
	// Tear down DMA, TIM2
	TIM2->CR1 &= ~TIM_CR1_CEN;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	// Disable the HX1
	ioSetOutput(PIN_HX1_EN, false);
	// Turn off the DAC
	DAC->CR &= ~DAC_CR_EN1;
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
