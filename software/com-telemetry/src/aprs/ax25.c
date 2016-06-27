#include "ax25.h"

#include <fcs/fcs.h>

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

bool encodeAndAppendDataAsAx25(const uint8_t* pMessageData,
                               uint16_t messageDataSize,
                               STUFFING_TYPE stuffingType,
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
            calculateFcs(currentByte);
        }

        for (uint8_t iBit = 0; iBit < 8; ++iBit)
        {
            const uint8_t currentBit = currentByte & (1 << iBit);

            if (currentBit)
            {
                if (pResultAprsEncodedMessage->size.chars >= MAX_AX25_MESSAGE_LENGTH)
                {
                    return false;
                }
                // as we are encoding 1 keep current bit as is
                if (pAx25EncodingContext->lastBit)
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] |= 1 << (pResultAprsEncodedMessage->size.lastCharBits);
                }
                else
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBits));
                }

                advanceBitstreamBit(&pResultAprsEncodedMessage->size);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    ++pAx25EncodingContext->numberOfOnes;

                    if (pAx25EncodingContext->numberOfOnes == 5)
                    {
                        if (pResultAprsEncodedMessage->size.chars >= MAX_AX25_MESSAGE_LENGTH)
                        {
                            return false;
                        }

                        // we need to insert 0 after 5 consecutive ones
                        if (pAx25EncodingContext->lastBit)
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBits));
                            pAx25EncodingContext->lastBit = 0;
                        }
                        else
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] |= 1 << (pResultAprsEncodedMessage->size.lastCharBits);
                            pAx25EncodingContext->lastBit = 1;
                        }

                        pAx25EncodingContext->numberOfOnes = 0;

                        advanceBitstreamBit(&pResultAprsEncodedMessage->size); // insert zero as we had 5 ones
                    }
                }
            }
            else
            {
                if (pResultAprsEncodedMessage->size.chars >= MAX_AX25_MESSAGE_LENGTH)
                {
                    return false;
                }

                // as we are encoding 0 we need to flip bit
                if (pAx25EncodingContext->lastBit)
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBits));
                    pAx25EncodingContext->lastBit = 0;
                }
                else
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] |= 1 << (pResultAprsEncodedMessage->size.lastCharBits);
                    pAx25EncodingContext->lastBit = 1;
                }

                advanceBitstreamBit(&pResultAprsEncodedMessage->size);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    pAx25EncodingContext->numberOfOnes = 0;
                }
            }
        }
    }

    if (stuffingType == ST_NO_STUFFING)
    {
        // reset ones as we didn't do any stuffing while sending this data
        pAx25EncodingContext->numberOfOnes = 0;
    }

    return true;
}
