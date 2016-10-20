/*
 * adc.c - Analog-to-digital converter functions
 */

#include <main.h>
#include <periph.h>

// Results from the ADC will end up here
// Due to errata 2.2 we should convert a useless channel first
volatile uint16_t adcResults[2];

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
