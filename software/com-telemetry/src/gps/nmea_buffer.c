#include <signals/signals.h>
#include "nmea_buffer.h"

#include <string.h>


uint8_t advanceUint8Index(uint8_t currentValue, uint8_t ringBufferLength)
{
    ++currentValue;
    if (currentValue >= ringBufferLength)
    {
        currentValue = 0;
    }
    return currentValue;
}

bool nmeaReadMessage(NmeaRingBuffer* pNmeaRingBuffer, NmeaMessage* pResultBuffer)
{
    // if buffer is empty and interrupt kicks in after we check '!pChannelData->readBuffer.isFull' we are still fine
    // as buffer can only get fuller by interrupt
    if (!(pNmeaRingBuffer->statusMask & NRBS_FULL) && pNmeaRingBuffer->endIdx == pNmeaRingBuffer->startIdx)
    {
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
    if ((pNmeaRingBuffer->statusMask & NRBS_FULL) && pNmeaRingBuffer->startIdx != pNmeaRingBuffer->endIdx)
    {
        // other 'thread' advanced start index (doesn't matter much if we miss this event and think that buffer is
        // full for the next iteration)
        pNmeaRingBuffer->statusMask &= ~NRBS_FULL;
    }

    if (!(pNmeaRingBuffer->statusMask & NRBS_FULL))
    {
        if (character == '$')
        {
            pNmeaRingBuffer->statusMask &= ~NRBS_WAIT_UNTIL_NEXT_MESSAGE; // we are at the beginning of next message
        }

        if (!(pNmeaRingBuffer->statusMask & NRBS_WAIT_UNTIL_NEXT_MESSAGE))
        {
            const uint8_t charWriteIdx = pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size;
            const bool thereIsSpaceForNewCharacter = charWriteIdx < NMEA_MESSAGE_MAX_LEN;

            bool moveToNewMessage = false;
            bool placeCurrentCharToNewMessage = false;

            if (!thereIsSpaceForNewCharacter)
            {
                moveToNewMessage = true;
                pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].hasError = true;
                pNmeaRingBuffer->statusMask |= NRBS_WAIT_UNTIL_NEXT_MESSAGE; // message buffer overflow, skip until next message
            }
            else if (character != '$' && charWriteIdx == 0)
            {
                pNmeaRingBuffer->statusMask |= NRBS_WAIT_UNTIL_NEXT_MESSAGE; // unexpected start character
            }
            else if (character == '$' && charWriteIdx != 0)
            {
                moveToNewMessage = true;
                placeCurrentCharToNewMessage = true;
            }
            else
            {
                pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].hasError |= errorEncountered;
                pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].message[charWriteIdx] = character;
                ++pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size;
                moveToNewMessage = character == '\x0A'; // we are done with the current message if we met LF
            }

            if (moveToNewMessage)
            {
                // start index can only get away and cannot get past end index so we are fine here
                pNmeaRingBuffer->endIdx = advanceUint8Index(pNmeaRingBuffer->endIdx, NMEA_RING_BUFFER_MAX_MESSAGES_COUNT);
                // if start index moves on we are golden and can continue writing
                if (pNmeaRingBuffer->endIdx == pNmeaRingBuffer->startIdx)
                {
                    pNmeaRingBuffer->statusMask |= NRBS_FULL;
                }
                else
                {
                    if (placeCurrentCharToNewMessage)
                    {
                        pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].hasError = errorEncountered;
                        pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].message[0] = character;
                        pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size = 1;
                    }
                    else
                    {
                        pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].hasError = false;
                        pNmeaRingBuffer->buffer[pNmeaRingBuffer->endIdx].size = 0;
                    }
                }
            }
        }
    }
    else
    {
        ERROR_NMEA_BUFFER_OVERFLOW();
    }
}
