/*
 * bkup.h - Mirrored backup register file definitions; none of these functions are ISR safe
 */

#ifndef BKUP_H_
#define BKUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// The number of backup registers available
#define NUM_BKUP 5U

// Clears all backup mirror registers and the RAM working file to 0x00
void bkupClearAll();
// Reads data from the specified backup register, correcting inconsistencies if they occur
uint16_t bkupRead(const uint32_t index);
// Writes data to the specified backup register
void bkupWrite(const uint32_t index, const uint16_t value);
// Reads one word (32 bits) from EEPROM
uint32_t eeRead(const uint32_t offset);
// Writes one word (32 bits) to EEPROM, this takes quite a while so be careful
void eeWrite(const uint32_t offset, const uint32_t data);

#ifdef __cplusplus
}
#endif

#endif
