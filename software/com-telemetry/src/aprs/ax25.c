#include "ax25.h"

#include <crc/crc.h>

#include "generated/ax25.h"

bool isAx25MessageEmtpy(const Ax25EncodedMessage* pMessage)
{
    if (!pMessage)
    {
        return true;
    }

    return pMessage->size.chars == 0 && pMessage->size.lastCharBits == 0;
}

void advanceBitstreamBit(BitstreamSize* pResultBitstreamSize)
{
    if (pResultBitstreamSize->lastCharBits >= 7)
    {
        ++pResultBitstreamSize->chars;
        pResultBitstreamSize->lastCharBits = 0;
    }
    else
    {
        ++pResultBitstreamSize->lastCharBits;
    }
}

bool encodeAndAppendPrefixAsAx25(Ax25EncodingContext* pAx25EncodingContext, Ax25EncodedMessage* pResultAprsEncodedMessage)
{
    pAx25EncodingContext->lastBit = 1;
    pAx25EncodingContext->numberOfOnes = 0;

    for (uint8_t i = 0; i < LEADING_FF_BYTES_COUNT_TO_CANCEL_PREVIOUS_PACKET; ++i)
    {
        pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = 0xFF;
    }

    for (uint8_t i = 0; i < PREFIX_FLAGS_COUNT; ++i)
    {
        pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ONE;
    }

    return true;
}

bool encodeAndAppendDataAsAx25(const uint8_t* pMessageData,
                               uint16_t messageDataSize,
                               FCS_TYPE fcsType,
                               SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                               Ax25EncodingContext* pAx25EncodingContext,
                               Ax25EncodedMessage* pResultAprsEncodedMessage)
{
    if (!pResultAprsEncodedMessage || !pAx25EncodingContext || MAX_AX25_MESSAGE_LENGTH < messageDataSize)
    {
        return false;
    }
    if (messageDataSize == 0)
    {
        return true;
    }
    if (!pMessageData)
    {
        return false;
    }

    for (uint16_t iByte = 0; iByte < messageDataSize; ++iByte)
    {
        uint8_t currentByte = pMessageData[iByte];

        if (shiftOneLeftType == SHIFT_ONE_LEFT)
        {
            currentByte <<= 1;
        }

        if (fcsType == FCS_CALCULATE)
        {
            calculateCrc(currentByte);
        }

        uint16_t value;
        const AX25EncodedData* pData = GET_AX25_ENCODED_DATA_FOR_BYTE(pAx25EncodingContext->numberOfOnes, currentByte);

        if (pAx25EncodingContext->lastBit)
        {
            value = GET_VALUE_IF_LAST_BIT_IS_ONE(pData);
        }
        else
        {
            value = GET_VALUE_IF_LAST_BIT_IS_ZERO(pData);
        }

        const uint8_t newLastBit = GET_LAST_BIT(value, pData);

        if (pResultAprsEncodedMessage->size.lastCharBits == 0)
        {
            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = (uint8_t) value;
            if (pData->dataBitsCount > 8)
            {
                pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] = (uint8_t) (value >> 8);
                pResultAprsEncodedMessage->size.lastCharBits = pData->dataBitsCount - 8;
            }
        }
        else
        {
            // old lastCharBits could have been 7 so if new data length is 10 bits max bits count is 17 (which is 2 whole bytes and one extra bit)

            const uint8_t byteRemainderData = (uint8_t) (value << pResultAprsEncodedMessage->size.lastCharBits);

            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] |= byteRemainderData;

            uint8_t remainingBits = pData->dataBitsCount - (8 - pResultAprsEncodedMessage->size.lastCharBits);

            value >>= (8 - pResultAprsEncodedMessage->size.lastCharBits);

            // now we need to fill whole bytes data

            while (remainingBits >= 8)
            {
                pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = (uint8_t) value;
                value >>= 8;
                remainingBits -= 8;
            }

            // now fill remainder if any

            if (remainingBits == 0)
            {
                pResultAprsEncodedMessage->size.lastCharBits = 0;
            }
            else
            {
                pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] = (uint8_t) value;
                pResultAprsEncodedMessage->size.lastCharBits = remainingBits;
            }
        }

        pAx25EncodingContext->numberOfOnes = pData->newNumberOfOnes;
        pAx25EncodingContext->lastBit = newLastBit;
    }

    return true;
}

bool encodeAndAppendSuffixAsAx25(Ax25EncodingContext* pAx25EncodingContext, Ax25EncodedMessage* pResultAprsEncodedMessage)
{
    const uint8_t frameStop = pAx25EncodingContext->lastBit == 0 ? FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ZERO :
                                                                   FRAME_SEPARATOR_GIVEN_THAT_PREVIOUS_BIT_WAS_ONE;

    if (pResultAprsEncodedMessage->size.lastCharBits == 0)
    {
        for (uint8_t i = 0; i < SUFFIX_FLAGS_COUNT; ++i)
        {
            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = frameStop;
        }
    }
    else
    {
        const uint8_t firstByteRemainder = frameStop << pResultAprsEncodedMessage->size.lastCharBits;
        const uint8_t otherBytesData = firstByteRemainder | (frameStop >> (8 - pResultAprsEncodedMessage->size.lastCharBits));

        pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] |= firstByteRemainder;

        for (uint8_t i = 1; i < SUFFIX_FLAGS_COUNT; ++i)
        {
            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars++] = otherBytesData;
        }
    }

    return true;
}
