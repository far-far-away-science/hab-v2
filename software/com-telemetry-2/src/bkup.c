/*
 * bkup.c - Mirrored backup register file handling; none of these functions are ISR safe
 */

#include <bkup.h>
#include <periph.h>
#include <stmtime.h>

// EEPROM base address (2 KB)
#define EEPROM_BASE ((const uint8_t *)0x08080000)

// Values kept in RAM for the 3-way redundancy
static volatile uint16_t bkupRAM[NUM_BKUP];

// Clears all backup mirror registers and the RAM working file to 0x00
void bkupClearAll() {
	volatile uint16_t *clr1 = &bkupRAM[0];
	volatile uint32_t *clr2 = &(RTC->BKPR[0]);
	for (uint32_t i = NUM_BKUP; i; i--) {
		*clr1++ = 0U;
		*clr2++ = 0U;
	}
}

// Reads data from the specified backup register, correcting inconsistencies if they occur
uint16_t bkupRead(const uint32_t index) {
	uint32_t ret = 0U;
	if (index < NUM_BKUP) {
		const uint32_t rtcValue = RTC->BKPR[index], rtcLow = rtcValue & 0xFFFFU,
			rtcHigh = rtcValue >> 16;
		ret = (uint32_t)bkupRAM[index];
		// If there is a mismatch
		if (rtcHigh != rtcLow || rtcLow != ret) {
			// Determine where the mess-up occurred
			if (ret == rtcLow || ret == rtcHigh)
				// RTC register was corrupted
				RTC->BKPR[index] = (ret << 16) | ret;
			else if (rtcLow == rtcHigh) {
				// RAM was corrupted
				ret = rtcLow;
				bkupRAM[index] = (uint16_t)ret;
			} else {
				// Not sure who to trust, (arbitrarily) select minimum of rtcHigh and rtcLow
				ret = (rtcHigh > rtcLow) ? rtcLow : rtcHigh;
				RTC->BKPR[index] = (ret << 16) | ret;
				bkupRAM[index] = (uint16_t)ret;
			}
		}
	}
	return ret;
}

// Writes data to the specified backup register
void bkupWrite(const uint32_t index, const uint16_t value) {
	if (index < NUM_BKUP) {
		bkupRAM[index] = value;
		// RTC backup registers must be accessed in word mode only
		RTC->BKPR[index] = ((uint32_t)value << 16) | (uint32_t)value;
	}
}

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
