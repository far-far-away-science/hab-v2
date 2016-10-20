/*
 * audio.h - Wave audio definitions
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Wave structures
// Each block follows this format
typedef struct {
	union {
		char ascii[4]; // "RIFF", "fmt ", "data"
		uint32_t word; // for faster compare
	} desc;
	uint32_t size;     // bytes, LE, file size - 8
} WaveHeader;

// Format chunk gives codec, bit width, etc. Size is nominally 16
typedef struct {
	uint16_t codec;    // codec used: 1 = PCM, 2 = ADPCM...
	uint16_t channels; // 1 = mono, 2 = stereo
	uint32_t rate;     // sample rate in bits/second, 44100 typical, this code uses 32000
	uint32_t datarate; // bytes per second = rate * block align
	uint16_t align;    // bytes per slice = bits per sample * number of channels / 8
	uint16_t bps;      // bits per sample (per channel)
	uint16_t extra;    // # of extra bytes to follow
} WaveChunkFmt;

// There are other chunks: list, silence, etc.

// Triggers the audio beeper at the given duty cycle (1-256 for best results); if zero is
// provided, the beeper is shut off
void audioBeep(const uint32_t duty);
// Starts precharge of the audio pins
void audioInit(void);
// Processes an audio interrupt in the main loop, returns TRUE iff audio has finished
bool audioInterrupt(const uint32_t flags);
// Plays a wave file from the SD card
bool audioPlay(const char *file);
// Finishes audio by closing the file and stopping the DAC/timer (does not shutdown amp!)
void audioStop(void);
// Shuts down the audio pins
void audioShutdown(void);

#ifdef __cplusplus
}
#endif

#endif
