/*
 * main.c - Telemetry main code
 */

#include <main.h>
#include <periph.h>
#include <bkup.h>
#include <adxl345.h>
#include <bme280.h>
#include <printf.h>
#include <aprs.h>
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

// Populates the APRS packet with the correct telemetry information
static INLINE void populatePacket(UTCTime *time, TelemetryData *data) {
	uint32_t seq = data->sequence, power;
	int32_t temp;
	// Calculate time
	getTime(&(time->hour), &(time->minute), &(time->second));
	readDate();
	// Data from ADC
	//  Temperature using conversion formula
	temp = adcConvertCPUTemp(adcResults[ADC_IDX_TEMP_I]) + APRS_TEMP_OFFSET;
	if (temp < 0)
		temp = 0;
	if (temp > 999)
		temp = 999;
	data->cpuTemp = (uint32_t)temp;
	//  Battery: 430K / 100K divider makes ADC value 0.189 * true voltage
	//  4095 = 3.3V so voltage in decivolts (is this a thing?) = x * 174.9 / 4095
	//  Account for 16x oversampling by shifting an additional 4 places
	power = ((uint32_t)adcResults[ADC_IDX_BAT] * 175U) >> 16;
	if (power > 999U)
		power = 999U;
	data->powerLevel = power;
	//  Ambient temperature: to be gathered from One-Wire sensor
	data->ambientTemp[0] = 500U;
	data->ambientTemp[1] = 500U;
	// Add one to sequence number
	if (seq >= 999U)
		seq = 1U;
	else
		seq++;
	data->sequence = seq;
}

// Initializes the packet to be sent over APRS
static INLINE void initPacket(TelemetryData *data) {
	data->sequence = 0U;
}

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
	bool sendAPRS = false;
	// Bring up sensors, green for success, red for failure
	if (bme280Init() && accelInit())
		setLED(0U, 65535U, 0U);
	else
		setLED(65535U, 0U, 0U);
	accelResume();
	initPacket(&data);
	while (1) {
		uint32_t flags;
		__disable_irq();
		{
			// Clear the flags with no risk of interrupt contention
			flags = sysFlags;
			sysFlags = flags & ~(FLAG_ADC_READY | FLAG_SYSTICK | FLAG_HX1_ANY | FLAG_RTC_30S |
				FLAG_RTC_1S);
		}
		__enable_irq();
		// Check the flags
		if ((flags & FLAG_HX1_ANY) != 0U && audioInterrupt(flags)) {
			// APRS data complete
			audioShutdown();
			setLED(0U, 0U, 0U);
		}
		if ((flags & FLAG_SYSTICK) != 0U && sendAPRS) {
			// HX1 is ready, compose and send APRS data
			sysTickDisable();
			sendAPRS = false;
			populatePacket(&time, &data);
			aprsSend(&time, NULL, &data);
		}
		if (flags & FLAG_RTC_30S) {
			setLED(0U, 0U, 65535U);
			// Power up the HX1
			audioInit();
			adcStart();
			// Start APRS once HX1 is ready
			sendAPRS = true;
			sysTickEnable();
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
	// Clear EXTI17 and EXTI20 flag since that is what actually ran this IRQ
	EXTI->PR = RTC_BIT;
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
