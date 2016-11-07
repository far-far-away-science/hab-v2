/*
 * bkup.c - EEPROM backup memory handing
 */

#include <bkup.h>
#include <periph.h>
#include <stmtime.h>

// EEPROM base address (2 KB)
#define EEPROM_BASE ((const uint8_t *)0x08080000)

// Reads one word (32 bits) from EEPROM
uint32_t eeRead(const uint32_t offset) {
	return ((const uint32_t *)EEPROM_BASE)[offset];
}

// Writes one word (32 bits) to EEPROM, this takes quite a while so be careful
void eeWrite(const uint32_t offset, const uint32_t data) {
	__disable_irq();
	{
		// IRQ needs to be masked during unlock sequence
		FLASH->PEKEYR = FLASH_PEKEYR_PEKEY1;
		FLASH->PEKEYR = FLASH_PEKEYR_PEKEY2;
	}
	__enable_irq();
	// EEPROM is now available, but not Program memory (we do not need the Flash!)
	((uint32_t *)EEPROM_BASE)[offset] = data;
	__dsb();
	// Relock EEPROM memory
	FLASH->PECR = FLASH_PECR_PELOCK;
}
