/*
 * onewire.h - One-Wire communications for temperature probes
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <stdbool.h>
#include <stdint.h>

// The maximum time to wait (I/O check loop iterations, about 1 MHz rate) for the 1-wire bus to
// be freed
#define OW_TIMEOUT 2048U

// Receives bytes from the 1-wire bus
void oneWireReadBytes(void *data, uint32_t count);
// Resets the 1-wire bus, returning true if slaves were detected
bool oneWireReset();
// Searches the 1-wire bus for devices, reporting the number of devices found
uint32_t oneWireSearch();
// Transmits one byte over the 1-wire bus
void oneWireWrite(const uint8_t data);
// Transmits bytes over the 1-wire bus
void oneWireWriteBytes(const void *data, uint32_t count);

#endif
