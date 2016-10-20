/*
 * comm.c - Communications functions for USARTs
 */

#include <comm.h>
#include <main.h>
#include <periph.h>
#include <usb_cdcacm.h>

// USART 1
static RingBuffer_TypeDef rx;
static RingBuffer_TypeDef tx;

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

// Reads a byte from the serial port
char serialReadByte() {
	while (ringIsBufferEmpty(&rx)) __sleep();
	return ringPullByte(&rx);
}

// Initializes serial communications
void serialInit() {
	// Turn on USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	rx.head = 0U;
	rx.tail = 0U;
	tx.head = 0U;
	tx.tail = 0U;
	USART1->CR2 = 0U;
	USART1->CR3 = 0U;
	// RM0376 p. 682 (0x116 for PLL)
#ifdef PLL
	USART1->BRR = 0x116U;
#else
	USART1->BRR = 0x8BU;
#endif
	// Eight-N-One, nil parity or flow control
	USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;
}

// Writes a byte to the serial port
void serialWriteByte(const char c) {
	while (ringIsBufferFull(&tx)) __sleep();
	ringQueueByte(&tx, c);
	USART1->CR1 |= USART_CR1_TXEIE;
}

// Buffered character I/O handler for UART port 2
void ISR_USART1() {
	char value;
	if (USART1->ISR & USART_ISR_RXNE) {
		// Read to clear the flag
		value = (char)USART1->RDR;
		if (!ringIsBufferFull(&rx))
			// Buffer it up, if it's not full to the brim
			ringQueueByte(&rx, value);
	}
	if (USART1->ISR & USART_ISR_TXE) {
		if (ringIsBufferEmpty(&tx))
			// Nothing to send, disable interrupt
			USART1->CR1 &= ~USART_CR1_TXEIE;
		else {
			value = ringPullByte(&tx);
			USART1->TDR = value;
		}
	}
}
