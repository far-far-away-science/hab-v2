/*
 * util.c - Utility functions (declared in periph.h or main.h) which do not belong in other
 * files
 */

#include <main.h>
#include <periph.h>
#include <stmtime.h>

// System state register
volatile uint32_t sysFlags;

// Clears the specified flags with no risk of interrupt contention
// Only to be used outside of interrupts
uint32_t clearFlags(const uint32_t set) {
	uint32_t flags;
	__disable_irq();
	{
		flags = sysFlags;
		sysFlags = flags & ~set;
	}
	__enable_irq();
	return flags;
}

// Sets the specified flags with no risk of interrupt contention
// Only to be used outside of interrupts
uint32_t setFlags(const uint32_t set) {
	uint32_t flags;
	__disable_irq();
	{
		flags = sysFlags;
		sysFlags = flags | set;
	}
	__enable_irq();
	return flags;
}

// Sets the LED color values
void setLED(uint32_t red, uint32_t green, uint32_t blue) {
#ifdef PHOENIX
#ifdef PHOENIX_LED_PWM
	// TIM22 analog control
	TIM22->CCR2 = red;
#else
	// Digital control only
	ioSetOutput(PIN_LED_R, red < 0x8000U);
#endif
	ioSetOutput(PIN_LED_G, green < 0x8000U);
	ioSetOutput(PIN_LED_B, blue < 0x8000U);
#else
	TIM3->CCR2 = green & 0xFFFFU;
	TIM3->CCR3 = red & 0xFFFFU;
	TIM3->CCR4 = blue & 0xFFFFU;
#endif
}

// RTC interrupt
void ISR_RTC() {
	const uint32_t isr = RTC->ISR;
	// Clear EXTI17 and EXTI20 flag since that is what actually ran this IRQ
	EXTI->PR = RTC_BIT;
	if (isr & RTC_ISR_ALRAF) {
		// Clear Alarm A flag
		RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_ALRAF;
		sysFlags |= FLAG_RTC_ALARM;
	}
	if (isr & RTC_ISR_ALRBF) {
		// Clear Alarm B flag
		RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_ALRBF;
		sysFlags |= FLAG_RTC_1S;
	}
	if (isr & RTC_ISR_WUTF) {
		// Clear wakeup timer flag
		RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_WUTF;
		sysFlags |= FLAG_RTC_30S;
	}
}

// Fires every 10 ms to update system timer
void ISR_SysTick() {
	sysTime++;
	sysFlags |= FLAG_SYSTICK;
}

// Fires when the button is pushed
void ISR_EXTI15_4() {
	EXTI->PR = BUTTON_BIT;
	sysFlags |= FLAG_BUTTON;
}
