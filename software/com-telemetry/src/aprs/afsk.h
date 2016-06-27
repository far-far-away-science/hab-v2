#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ax25.h"

typedef struct AfskContext_t
{
    BitstreamSize pos;
    uint16_t leadingWarmUpQuantsLeft;
    bool currentFrequencyIsF1200;
    uint32_t currentF1200Quant;
    uint32_t currentF2200Quant;
    uint32_t currentSymbolQuant;
    bool lastCharacterGenerated;
#include "generated/afsk_context_generated.h"
} AfskContext;

void resetAfskContext(AfskContext* pAfskContext);

/*
 * Should fill full requested length of the buffer. If no data available add Zeroes.
 *
 * returns true if data was filled in
 */
bool encodeAx25MessageAsAfsk(Ax25EncodedMessage* pMessage,
                             AfskContext* pAfskContext,
                             uint16_t* pOutputBuffer,
                             uint32_t outputBufferSize);
