/*
 * main.c - Telemetry main code
 */

#include <bkup.h>
#include <main.h>
#include <dac.h>
#include <periph.h>
#include <stdlib.h>
#include <string.h>
#include <stmtime.h>

// System state register
volatile uint32_t sysFlags;

#ifdef HABV2
// Selects the specified SPI device from SPI_DEVICE_* in main.h
void spiSelect(uint32_t device) {
	ioSetOutput(PIN_LATCH_A0, (device & 0x1U) != 0U);
	ioSetOutput(PIN_LATCH_A1, (device & 0x2U) != 0U);
	ioSetOutput(PIN_LATCH_A2, (device & 0x4U) != 0U);
	// Pulse the latch enable to lock in the pin selection
	ioSetOutput(PIN_LATCH_CS, false);
	__dsb();
	ioSetOutput(PIN_LATCH_CS, true);
}
#endif

// Sets the LED color values
void setLED(uint32_t red, uint32_t green, uint32_t blue) {
#ifdef PHOENIX
	ioSetOutput(PIN_LED_R, red < 0x8000U);
	ioSetOutput(PIN_LED_G, green < 0x8000U);
	ioSetOutput(PIN_LED_B, blue < 0x8000U);
#else
	TIM3->CCR2 = green & 0xFFFFU;
	TIM3->CCR3 = red & 0xFFFFU;
	TIM3->CCR4 = blue & 0xFFFFU;
#endif
}

// Main program
int main(void) {
	while (1) {
	}
	return 0;
}

// Fires every 10 ms to update system timer
void ISR_SysTick() {
	sysTime++;
}
