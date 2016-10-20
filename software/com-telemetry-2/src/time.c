/*
 * time.c - System time control and time-based utility functions
 */

#include <main.h>
#include <display.h>
#include <stmtime.h>
#include <periph.h>

// SysTick nesting counter to allow multiple sources to use the timer simultaneously
// The enable and disable functions only run in thread mode so there are no updating issues
static volatile uint32_t sysTickNesting;

// Converts an hour:minute:second to the RTC bitfield for alarm and time registers
static uint32_t hmsToRTCField(const uint32_t hour, const uint32_t min, const uint32_t sec) {
	return (((hour / 10U) & 0x3U) << RTC_TR_HT_S) | ((hour % 10U) << RTC_TR_HU_S) |
		(((min / 10U) & 0x7U) << RTC_TR_MNT_S) | ((min % 10U) << RTC_TR_MNU_S) |
		(((sec / 10U) & 0x7U) << RTC_TR_ST_S) | ((sec % 10U) << RTC_TR_SU_S);
}

// Converts a y/m/d to the RTC bitfield for date registers
static uint32_t ymdToRTCField(const uint32_t year, const uint32_t mon, const uint32_t day) {
	// Year can only be 2000 or up on this part, year is assumed as difference from 2000
	return (((year / 10U) & 0x3U) << RTC_DR_YT_S) | ((year % 10U) << RTC_DR_YU_S) |
		((mon > 9U) ? RTC_DR_MT : 0) | ((mon % 10U) << RTC_DR_MU_S) |
		(((day / 10U) & 0x3U) << RTC_DR_DT_S) | ((day % 10U) << RTC_DR_DU_S);
}

// Finds the day of the week for the specified date
static uint32_t dayOfWeek(const uint32_t year, const uint32_t mon, uint32_t day) {
	uint32_t leapYears = (year + 3U) >> 2;
	// 1/1/2000 was a Saturday = 5 (Monday = 0, Sunday = 6)
	// Every year adds a day, every leap year adds 2 days
	day += 4U + leapYears + year;
	// Brute force method of handling the months
	for (uint32_t mm = 1; mm < mon; mm++)
		day += daysInMonth(mm, year);
	// RTC uses 1-7 with Monday = 1, Sunday = 7
	return (day % 7U) + 1U;
}

// Adds or subtracts a specified number of minutes to the given time (up to 1439 minutes)
void addTime(uint32_t *hour, uint32_t *min, int32_t add) {
	uint32_t futureMin = *min;
	if (add >= 0)
		// Adding to the time
		futureMin += (uint32_t)add;
	else
		// Subtracting from the time
		futureMin += (uint32_t)(1440 + add);
	*hour = (*hour + (futureMin / 60U)) % 24U;
	// Leap second not a problem, DST?
	*min = futureMin % 60U;
}

// Calculates the number of days in the given month -- year is 0-99 for 2000-2099
uint32_t daysInMonth(uint32_t month, uint32_t year) {
	switch (month) {
	case 2U:
		// February has 28 days, except in years divisible by 4
		// Years divisible by 100 are not leap years, but years divisible by 400 are, so 2000
		// (0) is a leap year, the only such year divisible by 100 in our domain
		return (year & 0x03U) ? 28U : 29U;
	case 4U:
	case 6U:
	case 9U:
	case 11U:
		// 30 days hath September April June and November
		return 30U;
	default:
		// January, March, May, July, August, October, December
		return 31U;
	}
}

// Disables the alarm
void disableAlarm(void) {
	// Enable RTC writes
	RTC->WPR = RTC_WPR_INIT1;
	RTC->WPR = RTC_WPR_INIT2;
	__dsb();
	// Disable alarm A
	RTC->CR &= ~RTC_CR_ALRAE;
	// Clear alarm A flag
	RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_ALRAF;
	// Disable RTC writes
	RTC->WPR = RTC_WPR_LOCK;
}

// Gets the current date
void getDate(uint32_t *year, uint32_t *month, uint32_t *day) {
	const uint32_t now = RTC->DR;
	// Calculate date
	if (year != NULL)
		*year = ((now & RTC_DR_YT) >> RTC_DR_YT_S) * 10U + ((now & RTC_DR_YU) >> RTC_DR_YU_S);
	if (month != NULL)
		*month = ((now & RTC_DR_MT) >> RTC_DR_MT_S) * 10U + ((now & RTC_DR_MU) >> RTC_DR_MU_S);
	if (day != NULL)
		*day = ((now & RTC_DR_DT) >> RTC_DR_DT_S) * 10U + (now & RTC_DR_DU);
}

// Gets the current day of week where 1 = Monday and 7 = Sunday
uint32_t getDayOfWeek(void) {
	return (RTC->DR & RTC_DR_WDU) >> RTC_DR_WDU_S;
}

// Gets the current time
void getTime(uint32_t *hour, uint32_t *min, uint32_t *sec) {
	const uint32_t now = RTC->TR;
	// Calculate time
	if (hour != NULL)
		*hour = ((now & RTC_TR_HT) >> RTC_TR_HT_S) * 10U + ((now & RTC_TR_HU) >> RTC_TR_HU_S);
	if (min != NULL)
		*min = ((now & RTC_TR_MNT) >> RTC_TR_MNT_S) * 10U + ((now & RTC_TR_MNU) >>
			RTC_TR_MNU_S);
	if (sec != NULL)
		*sec = ((now & RTC_TR_ST) >> RTC_TR_ST_S) * 10U + (now & RTC_TR_SU);
}

// Enables and sets the alarm to the specified HH:MM:SS time, insensitive to day, 24 hour
void setAlarmTo(const uint32_t hour, const uint32_t min, const uint32_t sec) {
	uint32_t timeout;
	// Enable RTC writes
	RTC->WPR = RTC_WPR_INIT1;
	RTC->WPR = RTC_WPR_INIT2;
	__dsb();
	// Disable alarm A to allow writes
	RTC->CR &= ~RTC_CR_ALRAE;
	// Wait for alarm to be available to write
	for (timeout = 0x100U; timeout && !(RTC->ISR & RTC_ISR_ALRAWF); timeout--);
	if (timeout) {
		// Able to write to alarm time, program it to ignore day/date but match the rest
		// Time register and alarm register use the same bit masks!
		RTC->ALRMAR = RTC_ALRMAR_MSK4 | hmsToRTCField(hour, min, sec);
		RTC->ALRMASSR = 0U;
	}
	// Clear alarm A flag and re-enable it
	RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_ALRAF;
	RTC->CR |= RTC_CR_ALRAE;
	// Disable RTC writes
	RTC->WPR = RTC_WPR_LOCK;
}

// Changes the current time in the RTC immediately
void setTime(const uint32_t hour, const uint32_t min, const uint32_t year, const uint32_t mon,
		const uint32_t day) {
	uint32_t sec;
	// Enter initialization mode
	RTC->WPR = RTC_WPR_INIT1;
	RTC->WPR = RTC_WPR_INIT2;
	__dsb();
	RTC->ISR = RTC_ISR_NOP | RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));
	// Update time and calculate the new day of week
	getTime(NULL, NULL, &sec);
	RTC->TR = hmsToRTCField(hour, min, sec);
	RTC->DR = ymdToRTCField(year, mon, day) | (dayOfWeek(year, mon, day) << RTC_DR_WDU_S);
	// Exit initialization mode and enable write protection
	RTC->ISR = RTC_ISR_NOP;
	RTC->WPR = RTC_WPR_LOCK;
}

// Enables the SysTick interrupt for 10ms time base
void sysTickEnable(void) {
	const uint32_t nest = sysTickNesting;
	if (!nest)
		SysTick->CTRL = SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;
	sysTickNesting = nest + 1U;
}

// Disables the SysTick interrupt
void sysTickDisable(void) {
	const uint32_t nest = sysTickNesting;
	if (nest)
		sysTickNesting = nest - 1U;
	if (nest < 2U)
		SysTick->CTRL = 0U;
}

// Clears and then waits for the RTC register synchronization flag to be set
void waitForRSF(void) {
	// Enable RTC writes
	RTC->WPR = RTC_WPR_INIT1;
	RTC->WPR = RTC_WPR_INIT2;
	__dsb();
	// Clear RSF, watchdog will bark if this gets stuck
	RTC->ISR = RTC_ISR_NOP & ~RTC_ISR_RSF;
	while (!(RTC->ISR & RTC_ISR_RSF));
	// Disable RTC writes
	RTC->WPR = RTC_WPR_LOCK;
}
