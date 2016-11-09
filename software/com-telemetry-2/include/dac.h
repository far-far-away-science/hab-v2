/*
 * audio.h - Audio output over DAC for APRS/HX1
 */

#ifndef DAC_H_
#define DAC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// Core clock = 16 MHz
// Audio frequency = 64 KHz
// 1200 Hz (53.3) = 0, 2200 Hz (29) = 1

// Uses fixed point arithmetic with this many fractional points (not for I/O of cos and sin)
#define F_S 4
// Binary radian implementation of cos and sin uses 2^16 per 2pi
#define B_RAD 65536
// How many brads one 1/64KHz step takes when running at 1200 Hz
#define PHASE_12 ((B_RAD * 1200U + (AUDIO_FREQ >> 1)) / AUDIO_FREQ)
// How many brads one 1/64KHz step takes when running at 2200 Hz
#define PHASE_22 ((B_RAD * 2200U + (AUDIO_FREQ >> 1)) / AUDIO_FREQ)

// CRC polynomial for transmission with AX.25 data
#define CRC_POLYNOMIAL 0x1021U

// Starts precharge of the audio pins
void audioInit(void);
// Processes an audio interrupt in the main loop, returns TRUE iff audio has finished
bool audioInterrupt(const uint32_t flags);
// Finishes audio by closing the file and stopping the DAC/timer (does not shutdown amp!)
void audioStop(void);
// Shuts down the audio pins
void audioShutdown(void);

#ifdef __cplusplus
}
#endif

#endif
