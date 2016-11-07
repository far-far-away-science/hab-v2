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
// Audio frequency = 32 KHz

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
