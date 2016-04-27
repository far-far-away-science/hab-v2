#include "nmeaBuffer.h"

#include <string.h>

#include "errors.h"

uint8_t advanceUint8Index(uint8_t currentValue, uint8_t maxLen) {
    ++currentValue;
    if (currentValue >= maxLen) {
        currentValue = 0;
    }
    return currentValue;
}

bool readMessage(NmeaRingBuffer* pNmeaRingBuffer, NmeaMessage* pResultBuffer) {
    // if buffer is empty and interrupt kicks in after we check '!pChannelData->readBuffer.isFull' we are still fine
    // as buffer can only get fuller by interrupt
    if (!(pNmeaRingBuffer->statusMask & NRBS_FULL) && pNmeaRingBuffer->endIdx == pNmeaRingBuffer->startIdx) {
        return false;
    }

    // start index can only be changed by 'thread' other than UART interrupt handler so we fine here as well
    memcpy(pResultBuffer, &pNmeaRingBuffer->buffer[pNmeaRingBuffer->startIdx], sizeof(NmeaMessage));
    pNmeaRingBuffer->buffer[pNmeaRingBuffer->startIdx].size = 0;
    pNmeaRingBuffer->startIdx = advanceUint8Index(pNmeaRingBuffer->startIdx, NMEA_RING_BUFFER_MAX_MESSAGES_COUNT);

    return pResultBuffer->size > 0;
}

void nmeaReceiveCharacter(NmeaRingBuffer* pNmeaRingBuffer, uint8_t character, bool errorEncountered)
{
    if ((pNmeaRingBuffer->statusMask & NRBS_FULL) && pNmeaRingBuffer->startIdx != pNmeaRingBuffer->endIdx) {
        // other 'thread' advanced start index (doesn't matter much if we miss this event and think that buffer is
        // full for the next iteration)
        pNmeaRingBuffer->statusMask &= ~NRBS_FULL;
    }

    if (!(pNmeaRingBuffer->statusMask & NRBS_FULL) && !(pNmeaRingBuffer->statusMask & NRBS_WAIT_UNTIL_NEXT_MESSAGE)) {
        const uint8_t charWriteIdx = pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size;
        const bool thereIsSpaceForNewCharacter = charWriteIdx < NMEA_MESSAGE_MAX_LEN;

        bool moveToNewMessage = false;
        bool placeCurrentCharToNewMessage = false;

        if (character == '$' && charWriteIdx != 0) {
            moveToNewMessage = true;
            placeCurrentCharToNewMessage = true;
            pNmeaRingBuffer->statusMask &= ~NRBS_WAIT_UNTIL_NEXT_MESSAGE; // we already at the beginning of next message
        } else if (!thereIsSpaceForNewCharacter) {
            moveToNewMessage = true;
            pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].hasError = true;
            pNmeaRingBuffer->statusMask |= NRBS_WAIT_UNTIL_NEXT_MESSAGE; // message buffer overflow, skip until next message
        } else {
            pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].message[charWriteIdx] = character;
            ++pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size;
        }

        if (moveToNewMessage) {
            // start index can only get away and cannot get past end index so we are fine here
            pNmeaRingBuffer->endIdx = advanceUint8Index(pNmeaRingBuffer->endIdx, NMEA_RING_BUFFER_MAX_MESSAGES_COUNT);
            // if start index moves on we are golden and can continue writing
            if (pNmeaRingBuffer->endIdx == pNmeaRingBuffer->startIdx) {
                pNmeaRingBuffer->statusMask |= NRBS_FULL;
            } else {
                if (placeCurrentCharToNewMessage) {
                    pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].message[0] = character;
                    pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size = 1;
                } else {
                    pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size = 0;
                }
            }
        }
    } else {
        ERROR_NMEA_BUFFER_OVERFLOW();
    }

    if ((pNmeaRingBuffer->statusMask & NRBS_WAIT_UNTIL_NEXT_MESSAGE) &&
        (pNmeaRingBuffer->statusMask & NRBS_PREVIOUS_CHARACTER_WAS_CR) &&
        character == '\x0A')
    {
        pNmeaRingBuffer->statusMask &= ~NRBS_WAIT_UNTIL_NEXT_MESSAGE;
    }

    if (character == '\x0D') {
        pNmeaRingBuffer->statusMask |= NRBS_PREVIOUS_CHARACTER_WAS_CR;
    } else {
        pNmeaRingBuffer->statusMask &= ~NRBS_PREVIOUS_CHARACTER_WAS_CR;
    }
}
