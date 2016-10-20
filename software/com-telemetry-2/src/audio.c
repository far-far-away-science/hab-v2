/*
 * audio.c - Wave audio functions
 */

#include <main.h>
#include <audio.h>
#include "fat_sd/ff.h"
#include <periph.h>

// Useful macro for reading from files
#define F_READ(_file, _dest, _read) f_read((_file), &(_dest), sizeof(_dest), &(_read))
// Scales the audio to size and re-centers to match amplifier reference to fix click and pop
// Need to convert a signed 16-bit sample to the DAC levels (11 bits)
#define SCALE_AUDIO(x) ((uint16_t)(((int32_t)(x) + 32775) >> 4) + 128U)
// Wave buffer size, 2 buffers are allocated so array is twice this
#define WAVE_BUFFER 256

// SD card file system and state of the audio system
static struct {
	// SD file system
	FATFS fs;
	// WAVE file open
	FIL wave;
	// Number of samples left
	uint32_t size;
} audioState;
// WAVE file buffers
static uint16_t waveData[WAVE_BUFFER << 1];

// Loads the buffer with wave data
static uint32_t loadBuffer(FIL *wave, uint16_t *buffer, uint32_t size) {
	// Temporary buffer
	int16_t temp[WAVE_BUFFER], *ptr = temp;
	if (size > 0) {
		uint32_t left = WAVE_BUFFER << 1;
		UINT read;
		// If file is about to end, read only that amount
		if (size < (WAVE_BUFFER << 1))
			left = size;
		// If at EOF, fill buffer with zeroes
		for (uint32_t i = size; i < WAVE_BUFFER; i++)
			temp[i] = 0U;
		while (f_read(wave, ptr, left, &read) == FR_OK && left > 0U && size > 0U) {
			// Record data read
			left -= read;
			size -= read;
			ptr += read;
		}
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
	audioState.size = size;
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
	// Enable amplifier if not already enabled
	ioSetOutput(PIN_AMP_SD, true);
	sysFlags &= ~FLAG_AUDIO_ANY;
}

// Decodes all blocks in the WAVE file, playing the first DATA block found
static bool startDecodeWaveFile(uint32_t offset, const uint32_t wavesize) {
	UINT read;
	WaveHeader head;
	FIL *wave = &audioState.wave;
	bool foundChunk = FALSE;
	// Read each following chunk
	while (!foundChunk && offset < wavesize && f_lseek(wave, offset) == FR_OK &&
			F_READ(wave, head, read) == FR_OK) {
		// Ignore the chunks LIST (0x5453494C), ID3 (0x20336469)
		if (head.desc.word == 0x61746164U) {
			const uint32_t size = head.size;
			// Data chunk, play only if long enough to fill 1 full wave buffer
			if (size > (WAVE_BUFFER << 2)) {
				setupAudio(size);
				foundChunk = TRUE;
			}
		}
		// Do not round the size up to the nearest word!
		// Some sites say that the whole file must be word aligned, but they lie, Audacity
		// will happily export a track with an odd number of bytes but no padding
		offset += sizeof(head) + head.size;
	}
	if (!foundChunk)
		// No more chunks, close file and return
		f_close(&audioState.wave);
	return foundChunk;
}

// Parses the WAVE file
static bool parseWaveFile() {
	UINT read;
	WaveHeader head;
	WaveChunkFmt fmt;
	FIL *wave = &audioState.wave;
	// Read the WAVE header, check that it starts with RIFF (LE!)
	if (F_READ(wave, head, read) == FR_OK && head.desc.word == 0x46464952U) {
		uint32_t ignore, offset = sizeof(head) + sizeof(uint32_t);
		const uint32_t wavesize = head.size + sizeof(head);
		// Calculate number of bytes left in the file, skip the "WAVE"
		f_read(wave, &ignore, sizeof(uint32_t), &read);
		// Read the fmt chunk, should always be first
		if (F_READ(wave, head, read) == FR_OK && head.desc.word == 0x20746D66U &&
				F_READ(wave, fmt, read) == FR_OK) {
			// Skip the bytes thus read
			offset += sizeof(head) + head.size;
			// Only proceed if the file is 16-bit signed PCM 32.000 KHz sampling rate
			if (fmt.codec == 1U && fmt.channels == 1U && fmt.bps == 16U && fmt.rate == 32000U)
				return startDecodeWaveFile(offset, wavesize);
		}
	}
	return false;
}

// Triggers the audio beeper at the given duty cycle (1-256 for best results); if zero is
// provided, the beeper is shut off
// TODO Volume is uselessly low, even with transistor -- try a self oscillating PC buzzer
void audioBeep(const uint32_t duty) {
	// Target 6.00 KHz ~= 16000000 / 2667
	uint32_t maxDuty = 0xA6BU;
	if (duty == 0U)
		maxDuty = 0xFFFFU;
	// These are preloaded so the current cycle is unaffected
	TIM22->CCR2 = duty & 0xFFU;
	TIM22->ARR = maxDuty;
}

// Starts precharge of the audio pins
void audioInit(void) {
	// Change pin to analog output mode
	ioSetDirection(PIN_DAC, DDR_INPUT_ANALOG);
	// Precharge audio pin to zero level
	DAC->CR = (DAC->CR & ~DAC_CR_TSEL1_BITS) | (DAC_CR_TSEL1_SOFT | DAC_CR_EN1);
	DAC->DHR12R1 = SCALE_AUDIO(0);
	__dsb();
	DAC->SWTRIGR = DAC_SWTRIGR_SWTRIG;
}

// Processes an audio interrupt in the main loop, returns TRUE iff audio has finished
bool audioInterrupt(const uint32_t flags) {
	uint32_t size = audioState.size;
	// Flags must be cleared immediately to avoid missing alarm/RTC interrupts
	if (flags & FLAG_AUDIO_FRONT)
		// Front half of buffer needs filling
		size = loadBuffer(&audioState.wave, &waveData[0], size);
	else if (flags & FLAG_AUDIO_BACK)
		// Back half of buffer needs filling
		size = loadBuffer(&audioState.wave, &waveData[WAVE_BUFFER], size);
	audioState.size = size;
	// If audio is complete, shut down
	if (size == 0U)
		audioStop();
	return size == 0U;
}

// Plays a wave file from the SD card
#define READ_FLAGS (FA_READ | FA_OPEN_EXISTING)
bool audioPlay(const char *file) {
	FIL *wave = &audioState.wave;
	bool started = false;
	// Open SD card and retrieve the desired audio file
	if (f_mount(0, &audioState.fs) == FR_OK && f_open(wave, file, READ_FLAGS) == FR_OK) {
		if (parseWaveFile())
			started = true;
		else
			// Close on error, it is now async so we cannot do it unconditionally
			f_close(wave);
	}
	return started;
}

// Shuts down the audio pins
void audioShutdown(void) {
	// Disable the amplifier
	ioSetOutput(PIN_AMP_SD, false);
	// Turn off the DAC
	DAC->CR &= ~DAC_CR_EN1;
	// Drive output "high"
	ioSetDirection(PIN_DAC, DDR_INPUT_PULLUP);
	// Stop the SD card
	sdShutdown();
}

// Finishes audio by closing the file and stopping the DAC/timer (does not shutdown amp!)
void audioStop(void) {
	// Tear down DAC, DMA, TIM6
	TIM2->CR1 &= ~TIM_CR1_CEN;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	f_close(&audioState.wave);
}

// Audio DMA requests
void ISR_DMA1_Channel7_4() {
	const uint32_t isr = DMA1->ISR, af = sysFlags;
	if (isr & DMA_ISR_TCIF4) {
		sysFlags = af | FLAG_AUDIO_BACK;
		// Clear flags
		DMA1->IFCR = DMA_ISR_TCIF4;
	}
	if (isr & DMA_ISR_HTIF4) {
		sysFlags = af | FLAG_AUDIO_FRONT;
		// Clear flags
		DMA1->IFCR = DMA_ISR_HTIF4;
	}
}
