/*
 * spi.c - SPI2DMA driver which uses DMA for SPI to improve performance on big transfers
 */

#include <main.h>
#include <periph.h>
#include <printf.h>

// SPI flags for interrupts to notify when transfers complete
#define SPI_RX_DONE 0x01U
#define SPI_TX_DONE 0x02U
#define SPI_ALL_DONE (SPI_RX_DONE | SPI_TX_DONE)

// SPI interrupt status
volatile uint32_t spiStatus;

// Waits for SPI to be done
static void _spiWaitDone() {
	do {
		// Backup timing here is low priority, as SPI is master clocked and cannot time out
		// IWDG will get us if somehow it is stuck here forever
		__sleep();
	} while ((spiStatus & SPI_ALL_DONE) != SPI_ALL_DONE);
}

// Writes the given value out over SPI and ignores the value read in
void spiWrite(const uint8_t value) {
	spiWriteRead(value);
}

// Writes multiple values out over SPI
void spiWriteBytes(const void *value, uint32_t length) {
	spiWriteReadBytes(NULL, value, length);
}

// Reads a value over SPI while writing the specified value
uint8_t spiWriteRead(const uint8_t value) {
	uint8_t data = 0U;
	spiWriteReadBytes(&data, &value, 1U);
	return data;
}

// Reads multiple values over SPI while writing the specified values
void spiWriteReadBytes(void *inValue, const void *outValue, uint32_t length) {
	const uint32_t def = 0xFFFFFFFFU;
	if (length > 0U) {
		uint32_t outConf = DMA1_Channel3->CCR, inConf = DMA1_Channel2->CCR, temp = 0U;
		const void *outAddr, *inAddr;
		// Start the engines of war
		spiStatus = 0U;
		DMA1->IFCR = DMA_IFCR_CH2 | DMA_IFCR_CH3;
		if (outValue == NULL) {
			// No output data
			outConf &= ~DMA_CCR_MEMINC;
			outAddr = &def;
		} else {
			// Output data
			outConf |= DMA_CCR_MEMINC;
			outAddr = outValue;
		}
		DMA1_Channel3->CMAR = (uint32_t)outAddr;
		DMA1_Channel3->CNDTR = length;
		DMA1_Channel3->CCR = outConf | DMA_CCR_EN;
		// Prepare for battle
		if (inValue == NULL) {
			// No receive data
			inConf &= ~DMA_CCR_MEMINC;
			inAddr = &temp;
		} else {
			// Receive data
			inConf |= DMA_CCR_MEMINC;
			inAddr = inValue;
		}
		DMA1_Channel2->CMAR = (uint32_t)inAddr;
		DMA1_Channel2->CNDTR = length;
		DMA1_Channel2->CCR = inConf | DMA_CCR_EN;
		// Enable DMA
		SPI1->CR2 = SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;
		// Wait until SPI is done
		_spiWaitDone();
		// Then stop the DMA requests
		DMA1_Channel2->CCR = inConf;
		DMA1_Channel3->CCR = outConf;
		SPI1->CR2 = 0U;
	}
}

// SPI DMA interrupt requests
void ISR_DMA1_Channel3_2() {
	const uint32_t isr = DMA1->ISR;
	if (isr & DMA_ISR_TCIF2) {
		spiStatus |= SPI_RX_DONE;
		// Clear flags
		DMA1->IFCR = DMA_IFCR_CH2;
	}
	if (isr & DMA_ISR_TCIF3) {
		spiStatus |= SPI_TX_DONE;
		// Clear flags
		DMA1->IFCR = DMA_IFCR_CH3;
	}
}
