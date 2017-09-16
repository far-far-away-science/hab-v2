/*
 * onewire.c - One-Wire communications for temperature probes
 */

#include <main.h>
#include <onewire.h>
#include <periph.h>
#include <stmtime.h>

#if 0
// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const uint8_t oneWireCRCTable[] = { 0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126,
	32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130,
	220, 35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98, 190, 224,
	2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255, 70, 24, 250, 164, 39, 121,
	155, 197, 132, 218, 56, 102, 229, 187, 89, 7, 219, 133, 103, 57, 186, 228, 6, 88, 25,
	71, 165, 251, 120, 38, 196, 154, 101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69,
	198, 152, 122, 36, 248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231,
	185, 140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205, 17, 79,
	173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80, 175, 241, 19, 77, 206,
	144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238, 50, 108, 142, 208, 83, 13, 239, 177,
	240, 174, 76, 18, 145, 207, 45, 115, 202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180,
	234, 105, 55, 213, 139, 87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170,
	72, 22, 233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168, 116,
	42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
#endif

static struct {
	// Used in the interrupt to determine what to do next
	volatile uint8_t mode;
	// The current byte being shifted in/out
	volatile uint8_t data;
	uint16_t PAD;
} oneWireState;

#if 0
// Computes a Dallas Semiconductor 8 bit CRC using the lookup table
static uint8_t oneWireCRC(const uint8_t *addr, uint32_t len) {
	uint32_t crc = 0;
	while (len--)
		crc = oneWireCRCTable[(crc ^ (uint32_t)*addr++)];
	return (uint8_t)crc;
}
#endif

// Triggers a 1-wire interrupt at the specified time in microseconds
static void oneWireInterruptIn(uint32_t time) {
	TIM6->CR1 = TIM_CR1_URS | TIM_CR1_ARPE;
	TIM6->ARR = time;
	TIM6->EGR = 1U;
	__dsb();
	TIM6->CR1 = TIM_CR1_CEN | TIM_CR1_OPM | TIM_CR1_ARPE | TIM_CR1_URS;
}

// Waits for the outstanding 1-wire operation to complete
static void oneWireWait() {
	do {
		__sleep();
	} while (oneWireState.mode);
}

// Wait for the specified time in microseconds using the 1-wire interrupt (blocking!)
static void oneWireWaitFor(uint32_t time) {
	// Can be any mode that is not already used
	oneWireState.mode = 24U;
	oneWireInterruptIn(time);
	// Until interrupt resets the status to 0 (if this locks up, watchdog will get us)
	oneWireWait();
}

// Receives bytes from the 1-wire bus
void oneWireReadBytes(void *data, uint32_t count) {
	uint8_t *dataOut = (uint8_t *)data;
	while (count--) {
		oneWireState.mode = 223U;
		// Write line high to ensure bus is clear
		ioSetOutput(PIN_OW_TEMP, true);
		oneWireInterruptIn(10U);
		// Wait for receive to complete
		oneWireWait();
		*dataOut++ = oneWireState.data;
	}
}

// Resets the 1-wire bus, returning true if slaves were detected
bool oneWireReset() {
	uint32_t timeout = OW_TIMEOUT;
	// Set pin to floating
	ioSetOutput(PIN_OW_TEMP, true);
	// Wait for a short time for bus to free
	while (--timeout && !ioGetInput(PIN_OW_TEMP));
	if (timeout) {
		ioSetOutput(PIN_OW_TEMP, false);
		// Wait until the line has been held LOW for 480us
		oneWireWaitFor(480U);
		ioSetOutput(PIN_OW_TEMP, true);
		// Slaves hold the line low for 60..240us 15..60us after the reset pulse if present
		// Sample twice - once early (30us) and once late (90us)
		oneWireWaitFor(30U);
		timeout = !ioGetInput(PIN_OW_TEMP);
		oneWireWaitFor(60U);
		if (!timeout)
			timeout = !ioGetInput(PIN_OW_TEMP);
		// Minimum reset time high is 480us, account for overhead
		oneWireWaitFor(380U);
	}
	return timeout != 0U;
}

// Transmits one byte over the 1-wire bus
void oneWireWrite(const uint8_t data) {
	oneWireState.data = data;
	oneWireState.mode = 17U;
	// Write line high to ensure bus is clear
	ioSetOutput(PIN_OW_TEMP, true);
	oneWireInterruptIn(10U);
	// Wait for transmit to complete
	oneWireWait();
}

// Transmits bytes over the 1-wire bus
void oneWireWriteBytes(const void *data, uint32_t count) {
	const uint8_t *dataIn = (uint8_t *)data;
	while (count--)
		oneWireWrite(*dataIn++);
}

// Interrupt used to handle microsecond processing on the 1-wire bus
void ISR_TIM6_DAC() {
	uint32_t state = (uint32_t)oneWireState.mode, interval = 0U;
	const uint32_t data = (uint32_t)oneWireState.data;
	// Shut off the alarm clock
	TIM6->SR = 0U;
	if (state > 222U && state < 255U) {
		// Reading byte: write low for 5us, read after 5us, wait out 50us until next bit
		const uint32_t stateOffset = (state - 223U) & 3U;
		switch (stateOffset) {
		case 1U:
			// 1
			ioSetOutput(PIN_OW_TEMP, true);
			interval = 5U;
			break;
		case 2U:
			// 2, skips 3 back to 0
			oneWireState.data = (uint8_t)((data >> 1) | (ioGetInput(PIN_OW_TEMP) ? 0x80U : 0U));
			interval = 50U;
			state++;
			break;
		default:
			// 0
			ioSetOutput(PIN_OW_TEMP, false);
			interval = 5U;
			break;
		}
		state++;
		// Will wrap around to 0 at the end
	} else if (state > 1U && state < 18U) {
		// Writing byte: if bit is 0 then 60us low 5us high; else 5us low 60us high
		// Even states write the low pulse, odd states the high pulse
		const uint32_t bit = data & 1U;
		if (state & 1U) {
			// State starts at 17 which is odd
			ioSetOutput(PIN_OW_TEMP, false);
			interval = bit ? 5U : 60U;
		} else {
			ioSetOutput(PIN_OW_TEMP, true);
			interval = bit ? 60U : 5U;
			// Shift bit right one (LSB first)
			oneWireState.data = data >> 1;
		}
		state--;
	} else
		// All others
		state = 0U;
	if (interval)
		oneWireInterruptIn(interval);
	oneWireState.mode = (uint8_t)state;
}
