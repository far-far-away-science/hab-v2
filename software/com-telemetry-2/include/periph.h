/*
 * periph.h - Higher-level peripheral manipulation than cortex.h
 */

#ifndef PERIPH_H_
#define PERIPH_H_

#include <cortex.h>
#include <stdarg.h>
#include <stdlib.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// Pin/port configuration for ioSetDirection
// General-purpose input
#define DDR_INPUT 0x00
// Input pull-up
#define DDR_INPUT_PULLUP 0x10
// Input pull-down
#define DDR_INPUT_PULLDOWN 0x20
// Analog input
#define DDR_INPUT_ANALOG 0x03
// General-purpose output
#define DDR_OUTPUT 0x01
// Open-drain output
#define DDR_OUTPUT_OD 0x05
// Open-drain output with pull up
#define DDR_OUTPUT_ODPU 0x15
// Alternate function I/O
#define DDR_AFO 0x02
// Alternate function I/O with pull up
#define DDR_AFO_PU 0x12
// Alternate function open-drain I/O
#define DDR_AFO_OD 0x06
// Alternate function open-drain I/O with pull up
#define DDR_AFO_ODPU 0x16

// Gets the digital value (1 or 0) of a pin configured as a digital input
static INLINE bool ioGetInput(GPIO_TypeDef* port, uint32_t pin) {
	const uint32_t mask = (uint32_t)(0x00000001 << (pin & 0x0F));
	return (port->IDR & mask) != 0;
}

// Sets the digital value (1 or 0) of a pin configured as a digital output
static INLINE void ioSetOutput(GPIO_TypeDef* port, uint32_t pin, bool value) {
	const uint32_t mask = (uint32_t)(0x00000001 << (pin & 0x0F));
	if (value)
		// Atomic bit set
		port->BSRR = mask;
	else
		// Atomic bit reset
		port->BRR = mask;
}

// Configures the AFIO assignment for the specified pin (assignment table is in part datasheet)
static INLINE void ioSetAlternateFunction(GPIO_TypeDef *port, uint32_t pin, uint8_t afio) {
	const uint32_t idx = (pin & 0x07) << 2, bank = (pin < 8) ? 0 : 1;
	// Load the AFIO bits into the low or high port selection register
	port->AFR[bank] = (port->AFR[bank] & (uint32_t)~(0xF << idx)) | ((uint32_t)(afio &
		0x07) << idx);
}

// Configures the pin as an input or output with a variety of settings
static INLINE void ioSetDirection(GPIO_TypeDef* port, uint32_t pin, uint32_t type) {
	// Force pin in range
	pin &= 0x0F;
	const uint32_t pin2 = pin << 1;
	// Mode has the low 2 bits
	port->MODER = (port->MODER & (uint32_t)~(0x3 << pin2)) | (uint32_t)((type & 0x03) << pin2);
	// Set open drain bit according to 4s position
	if (type & 0x04)
		port->OTYPER |= (uint32_t)(1 << pin);
	else
		port->OTYPER &= (uint32_t)~(1 << pin);
	// PU/PD has the highest 2 bits
	port->PUPDR = (port->PUPDR & (uint32_t)~(0x3 << pin2)) | (uint32_t)(((type & 0x30) >>
		4) << pin2);
}

// Starts an ADC conversion
void adcStart(void);
// Writes the given value out over SPI and ignores the value read in
void spiWrite(const uint8_t value);
// Writes multiple values out over SPI
void spiWriteBytes(const void *value, uint32_t length);
// Reads a value over SPI while writing the specified value
uint8_t spiWriteRead(const uint8_t value);
// Reads multiple values over SPI while writing the specified values
void spiWriteReadBytes(void *inValue, const void *outValue, uint32_t length);
// Reads the specified number of data bytes from the specified address
bool i2cRead(uint8_t addr, void *data, uint32_t count);
// Reads the specified number of data bytes from a given register on the specified address,
// using the I2C "Repeated Start" convention
bool i2cReadRegister(uint8_t addr, uint8_t reg, void *value, uint32_t count);
// Writes the specified number of data bytes to the specified address
bool i2cWrite(uint8_t addr, const void *data, uint32_t count);
// Writes a data byte to the specified register on the specified address
bool i2cWriteRegister(uint8_t addr, uint8_t reg, uint8_t value);
// Shuts down the SD card
void sdShutdown(void);
// Switches the system clock to the PLL (only used if #define PLL)
void switchToPLL(void);

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
