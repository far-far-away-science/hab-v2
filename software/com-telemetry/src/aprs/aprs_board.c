#include "aprs_board_impl.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

// TODO replace with tables (we have plenty of data memory so we can make large tables for high precision)
#define SINE(v)            sinf(v)
#define COSINE_G_THAN_0(v) (cosf(v) >= 0)
#define INVERSE_SINE(v)    asinf(v)

const Callsign CALLSIGN_SOURCE = 
{
    {"HABHAB"},
    '\xF6' // 111 1011 0
           //          ^ not a last address
           //     ^^^^ SSID (11 - balloon)
           // ^^^ some reserved values and command/response
};

const Callsign CALLSIGN_DESTINATION_1 = 
{
    {"WIDE1 "},
    '\xE2' // 111 0001 0
           //          ^ not a last address
           //     ^^^^ SSID (1 - wide1-1)
           // ^^^ some reserved values and command/response
};

const Callsign CALLSIGN_DESTINATION_2 = 
{
    {"WIDE2 "},
    '\xE5' // 111 0010 1
           //          ^ last address
           //     ^^^^ SSID (2 - wide2-2)
           // ^^^ some reserved values and command/response
};

void advanceBitstreamBit(BitstreamSize* pResultBitstreamSize)
{
    if (pResultBitstreamSize->lastCharBitsCount >= 7)
    {
        ++pResultBitstreamSize->charsCount;
        pResultBitstreamSize->lastCharBitsCount = 0;
    }
    else
    {
        ++pResultBitstreamSize->lastCharBitsCount;
    }
}

bool encodeAndAppendBits(const uint8_t* pMessageData,
                         uint16_t messageDataSize,
                         STUFFING_TYPE stuffingType,
                         FCS_TYPE fcsType,
                         SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                         AprsEncodedMessage* pResultAprsEncodedMessage,
                         EncodingContext* pEncodingContext)
{
    if (!pResultAprsEncodedMessage || !pEncodingContext || MAX_APRS_MESSAGE_LENGTH < messageDataSize)
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

        for (uint8_t iBit = 0; iBit < 8; ++iBit)
        {
            const uint8_t currentBit = currentByte & (1 << iBit);

            // TODO
            // add FCS calculation based on tables
            // to improve speed (need to migrate to GCC due to 
            // 32Kb application size limit in Keil)
            // STM32 has CRC hardware can reuse that

            if (fcsType == FCS_CALCULATE)
            {
                const uint16_t shiftBit = pEncodingContext->fcs & 0x0001;
                pEncodingContext->fcs = pEncodingContext->fcs >> 1;
                if (shiftBit != ((currentByte >> iBit) & 0x01))
                {
                    pEncodingContext->fcs ^= FCS_POLYNOMIAL;
                }
            }

            if (currentBit)
            {
                if (pResultAprsEncodedMessage->size.charsCount >= MAX_APRS_MESSAGE_LENGTH)
                {
                    return false;
                }
                // as we are encoding 1 keep current bit as is
                if (pEncodingContext->lastBit)
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] |= 1 << (pResultAprsEncodedMessage->size.lastCharBitsCount);
                }
                else
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBitsCount));
                }

                advanceBitstreamBit(&pResultAprsEncodedMessage->size);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    ++pEncodingContext->numberOfOnes;
                    
                    if (pEncodingContext->numberOfOnes == 5)
                    {
                        if (pResultAprsEncodedMessage->size.charsCount >= MAX_APRS_MESSAGE_LENGTH)
                        {
                            return false;
                        }

                        // we need to insert 0 after 5 consecutive ones
                        if (pEncodingContext->lastBit)
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBitsCount));
                            pEncodingContext->lastBit = 0;
                        }
                        else
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] |= 1 << (pResultAprsEncodedMessage->size.lastCharBitsCount);
                            pEncodingContext->lastBit = 1;
                        }
                        
                        pEncodingContext->numberOfOnes = 0;
                        
                        advanceBitstreamBit(&pResultAprsEncodedMessage->size); // insert zero as we had 5 ones
                    }
                }
            }
            else
            {
                if (pResultAprsEncodedMessage->size.charsCount >= MAX_APRS_MESSAGE_LENGTH)
                {
                    return false;
                }
                
                // as we are encoding 0 we need to flip bit
                if (pEncodingContext->lastBit)
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBitsCount));
                    pEncodingContext->lastBit = 0;
                }
                else
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.charsCount] |= 1 << (pResultAprsEncodedMessage->size.lastCharBitsCount);
                    pEncodingContext->lastBit = 1;
                }

                advanceBitstreamBit(&pResultAprsEncodedMessage->size);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    pEncodingContext->numberOfOnes = 0;
                }
            }
        }
    }

    if (stuffingType == ST_NO_STUFFING)
    {
        // reset ones as we didn't do any stuffing while sending this data
        pEncodingContext->numberOfOnes = 0;
    }

    return true;
}

bool isAprsMessageEmtpy(const AprsEncodedMessage* pMessage)
{
    return true; // TODO
}

bool encodeGpsAprsMessage(const Callsign callsign, const GpsData* pGpsData, AprsEncodedMessage* pEncdedMessage)
{
    return false; // TODO
}

bool encodeTelemetryAprsMessage(const Callsign callsign, const Telemetry* pTelemetry, AprsEncodedMessage* pEncdedMessage)
{
    return false; // TODO
}

bool fillInAmplitudesBuffer(const AprsEncodedMessage* pMessage, uint16_t* pOutputBuffer, uint32_t outputBufferSize)
{
    return false; // TODO
}
