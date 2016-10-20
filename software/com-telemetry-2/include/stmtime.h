/*
 * stmtime.h - Definitions for system time control and time-based utility functions
 *
 * This file should have been named "time.h", but that conflicts with the C system-provided
 * time.h header.
 */

#ifndef STMTIME_H_
#define STMTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <periph.h>

// Define to enable the watchdog
#define WATCHDOG_ON

// Feeds the IWDG
static INLINE void feedWatchdog() {
#ifdef WATCHDOG_ON
	IWDG->KR = IWDG_KR_FEED;
#endif
}
// "Reads" the date register to unlock values
#define readDate() ((void)RTC->DR)

// Attention: Variable will overflow every now and then, must be handled
extern volatile uint32_t sysTime;

// Adds or subtracts a specified number of minutes to the given time (up to 1439 minutes)
void addTime(uint32_t *hour, uint32_t *min, int32_t add);
// Calculates the number of days in the given month -- year is 0-99 for 2000-2099
uint32_t daysInMonth(uint32_t month, uint32_t year);
// Blocks for the given number of milliseconds (test only!!)
void __attribute__ ((deprecated)) delay(const uint32_t time);
// Disables the alarm
void disableAlarm(void);
// Gets the current date
void getDate(uint32_t *year, uint32_t *month, uint32_t *day);
// Gets the current day of week where 1 = Monday and 7 = Sunday
uint32_t getDayOfWeek(void);
// Gets the current time
void getTime(uint32_t *hour, uint32_t *min, uint32_t *sec);
// Enables and sets the alarm to the specified HH:MM:SS time, insensitive to day, 24 hour
void setAlarmTo(const uint32_t hour, const uint32_t min, const uint32_t sec);
// Changes the current time in the RTC immediately
void setTime(const uint32_t hour, const uint32_t min, const uint32_t year, const uint32_t mon,
	const uint32_t day);
// Enables the SysTick interrupt for 10ms time base (NOT IRQ safe)
void sysTickEnable(void);
// Disables the SysTick interrupt (NOT IRQ safe)
void sysTickDisable(void);
// Clears and then waits for the RTC register synchronization flag to be set (NOT IRQ safe)
void waitForRSF(void);

#ifdef __cplusplus
}
#endif

#endif
