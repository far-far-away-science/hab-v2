/*
 * main.c - Telemetry main code
 */

#include <main.h>
#include <bkup.h>
#include <adxl345.h>
#include <bme280.h>
#include <printf.h>
#include <aprs.h>
#include <periph.h>
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
	UTCTime time;
	TelemetryData data;
	// Bring up sensors, green for success, red for failure
	if (bme280Init() && accelInit())
		setLED(0U, 65535U, 0U);
	else
		setLED(65535U, 0U, 0U);
	accelResume();
	// Set time to 12:00:00 PM
	time.hour = 12U;
	time.minute = 0U;
	time.second = 0U;
	// Fill in fake telemetry
	data.sequence = 1U;
	data.powerLevel = 90U;
	data.ambientTemp = 750U;
	data.cpuTemp = 800U;
	while (1) {
		uint32_t flags;
		__disable_irq();
		{
			// Clear the flags with no risk of interrupt contention
			flags = sysFlags;
			sysFlags = flags & ~(FLAG_ADC_READY | FLAG_SYSTICK | FLAG_HX1_ANY | FLAG_RTC_1S);
		}
		__enable_irq();
		// Check the flags
		if ((flags & FLAG_RTC_1S) && (RTC->TR & RTC_TR_SU) % 2U == 0U) {
			// "send" APRS data
			setLED(0U, 0U, 65535U);
			aprsSend(&time, NULL, &data);
		}
		if ((flags & FLAG_HX1_ANY) != 0U && audioInterrupt(flags)) {
			audioShutdown();
			setLED(0U, 0U, 0U);
		}
		// Feed the watchdog
		feedWatchdog();
		__sleep();
	}
	return 0;
}

// RTC interrupt
void ISR_RTC() {
	const uint32_t isr = RTC->ISR;
	// Clear EXTI17 flag since that is what actually ran this IRQ
	EXTI->PR = RTC_BIT;
#if 1
	if (isr & RTC_ISR_ALRBF) {
		// Clear Alarm B flag
		RTC->ISR = ~RTC_ISR_ALRBF;
		sysFlags |= FLAG_RTC_1S;
	}
#else
	if (isr & RTC_ISR_WUTF) {
		// Clear wakeup timer flag
		RTC->ISR = ~RTC_ISR_WUTF;
		sysFlags |= FLAG_RTC_1S;
	}
#endif
}

// Fires every 10 ms to update system timer
void ISR_SysTick() {
	sysTime++;
	sysFlags |= FLAG_SYSTICK;
}
