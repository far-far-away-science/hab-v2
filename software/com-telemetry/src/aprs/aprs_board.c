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

bool sendAprsMessage(const GpsData* pGpsData)
{
    return false; // TODO
}

bool sendTelemetryMessage(const Telemetry* pTelemetry)
{
    return false; // TODO
}

void advanceBitstreamBit(BitstreamPos* pResultBitstreamSize)
{
    if (pResultBitstreamSize->bitstreamCharBitIdx >= 7)
    {
        ++pResultBitstreamSize->bitstreamCharIdx;
        pResultBitstreamSize->bitstreamCharBitIdx = 0;
    }
    else
    {
        ++pResultBitstreamSize->bitstreamCharBitIdx;
    }
}

bool encodeAndAppendBits(uint8_t* pBitstreamBuffer,
                         uint16_t maxBitstreamBufferLen,
                         EncodingData* pEncodingData,
                         const uint8_t* pMessageData,
                         uint16_t messageDataSize,
                         STUFFING_TYPE stuffingType,
                         FCS_TYPE fcsType,
                         SHIFT_ONE_LEFT_TYPE shiftOneLeftType)
{
    if (!pBitstreamBuffer || !pEncodingData || maxBitstreamBufferLen < messageDataSize)
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

            if (fcsType == FCS_CALCULATE)
            {
                const uint16_t shiftBit = pEncodingData->fcs & 0x0001;
                pEncodingData->fcs = pEncodingData->fcs >> 1;
                if (shiftBit != ((currentByte >> iBit) & 0x01))
                {
                    pEncodingData->fcs ^= FCS_POLYNOMIAL;
                }
            }

            if (currentBit)
            {
                if (pEncodingData->bitstreamSize.bitstreamCharIdx >= maxBitstreamBufferLen)
                {
                    return false;
                }
                // as we are encoding 1 keep current bit as is
                if (pEncodingData->lastBit)
                {
                    pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] |= 1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx);
                }
                else
                {
                    pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] &= ~(1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx));
                }

                advanceBitstreamBit(&pEncodingData->bitstreamSize);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    ++pEncodingData->numberOfOnes;
                    
                    if (pEncodingData->numberOfOnes == 5)
                    {
                        if (pEncodingData->bitstreamSize.bitstreamCharIdx >= maxBitstreamBufferLen)
                        {
                            return false;
                        }

                        // we need to insert 0 after 5 consecutive ones
                        if (pEncodingData->lastBit)
                        {
                            pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] &= ~(1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx));
                            pEncodingData->lastBit = 0;
                        }
                        else
                        {
                            pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] |= 1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx);
                            pEncodingData->lastBit = 1;
                        }
                        
                        pEncodingData->numberOfOnes = 0;
                        
                        advanceBitstreamBit(&pEncodingData->bitstreamSize); // insert zero as we had 5 ones
                    }
                }
            }
            else
            {
                if (pEncodingData->bitstreamSize.bitstreamCharIdx >= maxBitstreamBufferLen)
                {
                    return false;
                }
                
                // as we are encoding 0 we need to flip bit
                if (pEncodingData->lastBit)
                {
                    pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] &= ~(1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx));
                    pEncodingData->lastBit = 0;
                }
                else
                {
                    pBitstreamBuffer[pEncodingData->bitstreamSize.bitstreamCharIdx] |= 1 << (pEncodingData->bitstreamSize.bitstreamCharBitIdx);
                    pEncodingData->lastBit = 1;
                }

                advanceBitstreamBit(&pEncodingData->bitstreamSize);

                if (stuffingType == ST_PERFORM_STUFFING)
                {
                    pEncodingData->numberOfOnes = 0;
                }
            }
        }
    }

    if (stuffingType == ST_NO_STUFFING)
    {
        // reset ones as we didn't do any stuffing while sending this data
        pEncodingData->numberOfOnes = 0;
    }

    return true;
}

bool fillInAmplitudesBuffer(uint16_t* pBuffer, uint32_t bufferSize)
{
    return true; // TODO KPL fill in
}
