/*
 * i2c.c - I2C (TWI) implementation for STM32L0 series chips
 */

#include <periph.h>

// I2C flags for interrupts to notify when transfers complete
#define I2C_DONE 0x01U
#define I2C_ERROR 0x02U

volatile uint32_t i2cState;

// Waits for I2C to complete, returning true if OK or false if errored out
static bool _i2cWait() {
	i2cState = 0U;
	do {
		__sleep();
	} while (i2cState == 0U);
	return (i2cState & I2C_DONE) ? true : false;
}

// Writes data to the I2C interface, sending STOP if restart is false
static bool _i2cWrite(uint8_t addr, const void *data, uint32_t count, bool restart) {
	count &= 0xFFU;
	// Program slave address and write mode
	const uint32_t cr2 = ((uint32_t)addr) | (restart ? 0U : I2C_CR2_AUTOEND) | (count << 16);
	// Turn on TX DMA
	DMA1_Channel6->CMAR = (uint32_t)data;
	DMA1_Channel6->CNDTR = count;
	DMA1_Channel6->CCR |= DMA_CCR_EN;
	// Send the START condition
	I2C1->CR2 = cr2;
	I2C1->CR2 = cr2 | I2C_CR2_START;
	if (restart)
		// In case of back to back restarted writes, this needs to be enabled after start sent
		I2C1->CR1 |= I2C_CR1_TCIE;
	// Wait for it, then kill DMA
	const bool ok = _i2cWait();
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	return ok;
}

// Reads the specified number of data bytes from the specified address
bool i2cRead(uint8_t addr, void *data, uint32_t count) {
	count &= 0xFFU;
	// Program slave address and read mode
	const uint32_t cr2 = ((uint32_t)addr) | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | (count << 16);
	// Turn on RX DMA
	DMA1_Channel7->CMAR = (uint32_t)data;
	DMA1_Channel7->CNDTR = count;
	DMA1_Channel7->CCR |= DMA_CCR_EN;
	// Send the START condition
	I2C1->CR2 = cr2;
	I2C1->CR2 = cr2 | I2C_CR2_START;
	// Wait for it, then kill DMA
	const bool ok = _i2cWait();
	DMA1_Channel7->CCR &= ~DMA_CCR_EN;
	return ok;
}

// Reads the specified number of data bytes from a given register on the specified address,
// using the I2C "Repeated Start" convention
bool i2cReadRegister(uint8_t addr, uint8_t reg, void *value, uint32_t count) {
	// Write out the location we want and read in data
	return _i2cWrite(addr, &reg, 1, true) && i2cRead(addr, value, count);
}

// Writes the specified number of data bytes to the specified address
bool i2cWrite(uint8_t addr, const void *data, uint32_t count) {
	return _i2cWrite(addr, data, count, false);
}

// Writes a data byte to the specified register on the specified address
bool i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t value) {
	uint8_t data[2];
	// Write register, value
	data[0] = reg;
	data[1] = value;
	// Write information
	return i2cWrite(addr, data, 2);
}

// I2C1 event and error interrupt
void ISR_I2C1() {
	const uint32_t cr = I2C1->CR1, isr = I2C1->ISR;
	if ((isr & I2C_ISR_TC) || (isr & I2C_ISR_STOPF)) {
		i2cState |= I2C_DONE;
		// Transmission complete, ready for RESTART or continue
		I2C1->CR1 = cr & ~I2C_CR1_TCIE;
		I2C1->ICR = I2C_ICR_STOPCF;
	} else {
		i2cState |= I2C_ERROR;
		// Turn I2C off and on again to reset BUSY flag
		I2C1->CR1 = cr & ~I2C_CR1_PE;
		__dsb();
		I2C1->CR1 = cr;
	}
}
