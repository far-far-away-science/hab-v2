/*
 * comm.c - Communications functions for USARTs
 */

#include <comm.h>
#include <main.h>
#include <periph.h>

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
	uint32_t temp;
	// Turn on USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
#ifdef DEBUG_UART
	// Turn on USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
#endif
	// Reset USART1
	temp = RCC->APB2RSTR & ~RCC_APB2RSTR_USART1RST;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_USART1RST;
	__dsb();
	RCC->APB2RSTR = temp;
#ifdef DEBUG_UART
	// Reset USART2
	temp = RCC->APB1RSTR & ~RCC_APB1RSTR_USART2RST;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_USART2RST;
	__dsb();
	RCC->APB1RSTR = temp;
#endif
	// Clear buffers
	rx.head = 0U;
	rx.tail = 0U;
	tx.head = 0U;
	tx.tail = 0U;
	USART1->CR2 = 0U;
	USART1->CR3 = 0U;
	// RM0376 p. 682 (0x116 for PLL)
#ifdef HS32
	USART1->BRR = 0x116U;
#else
	USART1->BRR = 0x8BU;
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
