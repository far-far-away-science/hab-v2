#include "fcs.h"

#define FCS_POLYNOMIAL                0x8408
#define FCS_INITIAL_VALUE             0xFFFF
#define FCS_POST_PROCESSING_XOR_VALUE 0xFFFF

static uint16_t g_currentFcs;

// TODO replace with silicone version

void resetFcs(void)
{
    g_currentFcs = FCS_INITIAL_VALUE;
}

void calculateFcs(uint8_t newByte)
{
    for (uint8_t iBit = 0; iBit < 8; ++iBit)
    {
        const uint16_t shiftBit = g_currentFcs & 0x0001;
        g_currentFcs = g_currentFcs >> 1;
        if (shiftBit != ((newByte >> iBit) & 0x01))
        {
            g_currentFcs ^= FCS_POLYNOMIAL;
        }
    }
}

uint16_t getCalculatedFcs()
{
    return g_currentFcs ^ FCS_POST_PROCESSING_XOR_VALUE;
}
