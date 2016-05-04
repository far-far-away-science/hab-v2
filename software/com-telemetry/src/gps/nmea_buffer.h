#pragma once

#include <stdint.h>
#include <stdbool.h>

#define NMEA_RING_BUFFER_MAX_MESSAGES_COUNT 3
#define NMEA_MESSAGE_MAX_LEN                128

typedef struct NmeaMessage_t
{
    uint8_t size;
    // signals UART or buffer overflow errors (still want to transmit it but with error flag)
    bool    hasError;
    uint8_t message[NMEA_MESSAGE_MAX_LEN];
} NmeaMessage;

typedef enum NmeaRingBufferStatus_t
{
    NRBS_FULL                      = 0x01,
    NRBS_WAIT_UNTIL_NEXT_MESSAGE   = 0x02,
} NmeaRingBufferStatus;

typedef struct NmeaRingBuffer_t
{
    uint8_t     statusMask;
    uint8_t     startIdx;
    uint8_t     endIdx;
    NmeaMessage buffer[NMEA_RING_BUFFER_MAX_MESSAGES_COUNT];
} NmeaRingBuffer;

uint8_t advanceUint8Index(uint8_t currentValue, uint8_t ringBufferLength);

void nmeaReceiveCharacter(NmeaRingBuffer* pNmeaRingBuffer, uint8_t character, bool errorEncountered);

bool nmeaReadMessage(NmeaRingBuffer* pNmeaRingBuffer, NmeaMessage* pResultBuffer);
