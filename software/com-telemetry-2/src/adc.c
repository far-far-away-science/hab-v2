/*
 * adc.c - Analog-to-digital converter functions
 */

#include <main.h>
#include <periph.h>

// Addresses of the internal temperature calibration data
// 30 C, 3 V (0x0295 = 661)
#define ADC_TS_CAL1 ((volatile uint16_t *)0x1FF8007A)
// 130 C, 3 V (0x0379 = 889)
#define ADC_TS_CAL2 ((volatile uint16_t *)0x1FF8007E)

// Results from the ADC will end up here
//  Errata 2.2 requires converting a useless channel first, but only affects Z series
// Max usage: HAB V2 (Battery, CPU Temperature, 2x Camera photocell, 2x Analog temperature)
volatile uint16_t adcResults[6];

// Converts the CPU temperature using the calibration values to a value in degrees C * 10
int32_t adcConvertCPUTemp(uint16_t value) {
	// Account for 16x oversampling
	const int32_t cal1 = ((int32_t)*ADC_TS_CAL1) << 4, cal2 = ((int32_t)*ADC_TS_CAL2) << 4,
		diff = cal2 - cal1;
	return ((int32_t)value - cal1) * 1000 / diff + 300;
}

// Starts an ADC conversion
void adcStart() {
	DMA1_Channel1->CMAR = (uint32_t)&adcResults[0];
	DMA1_Channel1->CNDTR = sizeof(adcResults) / sizeof(uint16_t);
	DMA1_Channel1->CCR |= DMA_CCR_EN;
	ADC1->CR = ADC_CR_ADSTART | ADC_CR_ADEN;
}

// ADC interrupt
void ISR_ADC_COMP() {
	const uint32_t isr = ADC1->ISR;
	if (isr & ADC_ISR_EOCAL) {
		// Calibration was completed, shut off the alarm clock
		ADC1->ISR = ADC_ISR_EOCAL;
		// RM0376 p. 288
		ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
	}
}

// ADC transfer complete interrupt
void ISR_DMA1_Channel1() {
	// Shut off the alarm clock... again
	DMA1->IFCR = DMA_IFCR_CH1;
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
}
