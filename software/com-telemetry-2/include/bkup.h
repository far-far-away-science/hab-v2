/*
 * bkup.h - EEPROM backup memory definitions
 */

#ifndef BKUP_H_
#define BKUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Reads one word (32 bits) from EEPROM
uint32_t eeRead(const uint32_t offset);
// Writes one word (32 bits) to EEPROM, this takes quite a while so be careful
void eeWrite(const uint32_t offset, const uint32_t data);

#ifdef __cplusplus
}
#endif

#endif
