/*
 * comm.c - Communications functions for USARTs
 */

#include <comm.h>
#include <main.h>
#include <nmea.h>
#include <periph.h>

// USART 1
#if defined(DEBUG_UART) || !defined(DUAL_GPS)
static RingBuffer_TypeDef rx;
#endif
static RingBuffer_TypeDef tx;

// Determine USART1 baud
#ifdef DUAL_GPS
#define BAUD_USART1 BAUD_GPS2
#else
#define BAUD_USART1 BAUD_PI
#endif

// Removes a byte from the head of the given buffer
char ringPullByte(volatile RingBuffer_TypeDef* buffer) {
	uint16_t head = buffer->head; char value;
	value = buffer->buffer[head];
	buffer->head = (head + 1) & (USART_BUFFER_SIZE - 1);
	return value;
}

// Queues a byte onto the tail of the given buffer
void ringQueueByte(volatile RingBuffer_TypeDef* buffer, char value) {
	uint16_t tail = buffer->tail;
	buffer->buffer[tail] = value;
	tail = (tail + 1) & (USART_BUFFER_SIZE - 1);
	buffer->tail = tail;
}

#if defined(DEBUG_UART) || !defined(DUAL_GPS)
// Reads a byte from the serial port; only use when serialReady() is true!
char serialReadByte() {
	return ringPullByte(&rx);
}

// Checks to see if data is available on the Pi serial port
bool serialReady() {
	return !ringIsBufferEmpty(&rx);
}
#endif

// Initializes serial communications
void serialInit() {
	uint32_t temp;
	// Turn on USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
#ifdef DEBUG_UART
	// Turn on USART2 and LPUART clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_LPUART1EN;
#else
	// Turn on LPUART clock
	RCC->APB1ENR |= RCC_APB1ENR_LPUART1EN;
#endif
	// Reset USART1
	temp = RCC->APB2RSTR & ~RCC_APB2RSTR_USART1RST;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_USART1RST;
	__dsb();
	RCC->APB2RSTR = temp;
#ifdef DEBUG_UART
	// Reset USART2 and LPUART
	temp = RCC->APB1RSTR & ~(RCC_APB1RSTR_USART2RST | RCC_APB1RSTR_LPUART1RST);
	RCC->APB1RSTR = temp | (RCC_APB1RSTR_USART2RST | RCC_APB1RSTR_LPUART1RST);
	__dsb();
	RCC->APB1RSTR = temp;
#else
	// Reset LPUART
	temp = RCC->APB1RSTR & ~RCC_APB1RSTR_LPUART1RST;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_LPUART1RST;
	__dsb();
	RCC->APB1RSTR = temp;
#endif
	// Clear buffers
#ifndef DUAL_GPS
	rx.head = 0U;
	rx.tail = 0U;
#endif
	tx.head = 0U;
	tx.tail = 0U;
	USART1->CR2 = 0U;
	USART1->CR3 = 0U;
	// RM0367 p. 763
#ifdef HS32
	USART1->BRR = (32000000U + (BAUD_USART1 >> 1)) / BAUD_USART1;
#else
	USART1->BRR = (16000000U + (BAUD_USART1 >> 1)) / BAUD_USART1;
#endif
	// Eight-N-One, nil parity or flow control
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;
#ifdef DEBUG_UART
	// For debugging only
#ifdef HS32
	USART2->BRR = 0x116U;
#else
	USART2->BRR = 0x8BU;
#endif
	// Eight-N-One, nil parity or flow control
	USART2->CR1 = USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;
#endif
	// Interrupt will wake from stop mode if UESM is set and RXNEIE is set
	LPUART1->CR3 = 0U;
	LPUART1->CR2 = 0U;
#ifdef LSE
	// Initially start on the LSE unless it failed to initialize
	serialSetLPBaud((sysFlags & FLAG_LSI) != 0U);
#else
	// Use the HSI instead if LSE is not enabled
	serialSetLPBaud(true);
#endif
}

// To achieve low power consumption while still handling LSE failures, allow the LPUART baud
// rate to be adjusted at runtime according to the source clock
void serialSetLPBaud(bool hsi16) {
	// Turn off LPUART1
	LPUART1->CR1 = 0U;
	if (hsi16) {
		RCC->CSR = (RCC->CSR & ~RCC_CCIPR_LPUART1SEL_LSE) | RCC_CCIPR_LPUART1SEL_HSI;
		__dsb();
		// Please do this at compile time...
		LPUART1->BRR = (uint32_t)((256ULL * 16000000ULL + (uint64_t)(BAUD_GPS1 >> 1)) /
			(uint64_t)BAUD_GPS1);
	} else {
		RCC->CSR = (RCC->CSR & ~RCC_CCIPR_LPUART1SEL_HSI) | RCC_CCIPR_LPUART1SEL_LSE;
		__dsb();
		LPUART1->BRR = (256U * 32768U + (BAUD_GPS1 >> 1)) / BAUD_GPS1;
	}
	// Clear LPUART overrun flag
	LPUART1->ICR = USART_ISR_ORE;
	// Eight-N-One, nil parity or flow control, no transmitter
	LPUART1->CR1 = USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE | USART_CR1_UESM;
}

// Writes a byte to the serial port
void serialWriteByte(const char c) {
#ifdef DEBUG_UART
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = c;
#else
	while (ringIsBufferFull(&tx)) __sleep();
	ringQueueByte(&tx, c);
	USART1->CR1 |= USART_CR1_TXEIE;
#endif
}

// Buffered character I/O handler for UART port 1
void ISR_USART1() {
	char value;
	if (USART1->ISR & USART_ISR_RXNE) {
		// Read to clear the flag
		value = (char)USART1->RDR;
#ifdef DUAL_GPS
		if (gpsCharHandler(value, 1U))
			sysFlags |= FLAG_GPS_2_READY;
#else
		if (!ringIsBufferFull(&rx))
			// Buffer it up, if it's not full to the brim
			ringQueueByte(&rx, value);
#endif
	}
#ifdef DEBUG_UART
	if (USART1->ISR & USART_ISR_TXE)
		// Nothing to send, disable interrupt
		USART1->CR1 &= ~USART_CR1_TXEIE;
#else
	if (USART1->ISR & USART_ISR_TXE) {
		if (ringIsBufferEmpty(&tx))
			// Nothing to send, disable interrupt
			USART1->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = ringPullByte(&tx);
			USART1->TDR = value;
		}
	}
#endif
}

#ifdef DEBUG_UART
// Buffered character I/O handler for UART port 2
void ISR_USART2() {
	char value;
	if (USART2->ISR & USART_ISR_RXNE) {
		// Read to clear the flag
		value = (char)USART2->RDR;
		if (!ringIsBufferFull(&rx))
			// Buffer it up, if it's not full to the brim
			ringQueueByte(&rx, value);
	}
	if (USART2->ISR & USART_ISR_TXE) {
		if (ringIsBufferEmpty(&tx))
			// Nothing to send, disable interrupt
			USART2->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = ringPullByte(&tx);
			USART2->TDR = value;
		}
	}
}
#endif

// Character I/O handler for low-power UART
void ISR_RNG_LPUART1() {
	const uint32_t isr = LPUART1->ISR;
	if (isr & USART_ISR_RXNE) {
		// Read and handle character
		char value = (char)(LPUART1->RDR);
		if (gpsCharHandler(value, 0U))
			sysFlags |= FLAG_GPS_1_READY;
	}
	if (isr & USART_ISR_ORE)
		// Clear and ignore
		LPUART1->ICR = USART_ISR_ORE;
}
