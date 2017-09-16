/*
 * comm.h - Definitions for character I/O and communications functions
 */

#ifndef COMM_H_
#define COMM_H_

#include <periph.h>
#include <stdbool.h>
#include <stdint.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// ---- Ring buffer definitions ----
// Size of buffer, in bytes, for the USART ring buffers
// Can be at most 65536 before overflowing the space in a word; must be a power of two
#define USART_BUFFER_SIZE 0x80

// Structure containing a ring buffer
typedef struct {
	uint16_t head;
	uint16_t tail;
	char buffer[USART_BUFFER_SIZE];
} RingBuffer_TypeDef;

// Checks to see if the ring buffer is empty (head = tail)
static INLINE bool ringIsBufferEmpty(volatile RingBuffer_TypeDef* buffer) {
	return buffer->head == buffer->tail;
}

// Checks to see if the ring buffer is full (tail + 1 = head)
static INLINE bool ringIsBufferFull(volatile RingBuffer_TypeDef* buffer) {
	return ((buffer->tail + 1) & (USART_BUFFER_SIZE - 1)) == buffer->head;
}

// Removes a byte from the head of the given buffer
char ringPullByte(volatile RingBuffer_TypeDef* buffer);
// Queues a byte onto the tail of the given buffer
void ringQueueByte(volatile RingBuffer_TypeDef* buffer, char value);
// Initializes serial communications
void serialInit();
// Reads a byte from the serial port; only use when serialReady() is true!
char serialReadByte();
// Checks to see if data is available on the Pi serial port
bool serialReady();
// To achieve low power consumption while still handling LSE failures, allow the LPUART baud
// rate to be adjusted at runtime according to the source clock
void serialSetLPBaud(bool hsi16);
// Writes a byte to the serial port
void serialWriteByte(const char c);

// Writes one character to the specified stream and returns the input value
#define fputc(_value, _stream) serialWriteByte(_value)

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
