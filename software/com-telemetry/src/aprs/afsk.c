#include "afsk.h"

#include "generated/afsk.h"

void resetAfskContext(AfskContext* pAfskContext)
{
    pAfskContext->currentF1200Quant = 0;
    pAfskContext->currentF2200Quant = 0;
    pAfskContext->currentFrequencyIsF1200 = true;
    pAfskContext->currentSymbolQuant = QUANTS_COUNT_PER_SYMBOL_F1200;
    pAfskContext->leadingWarmUpQuantsLeft = LEADING_WARMUP_QUANTS_COUNT;
    pAfskContext->lastCharacterGenerated = false;
    pAfskContext->pos.chars = 0;
    pAfskContext->pos.lastCharBits = 0;
    RESET_CONTEXT_GENERATED_PART(pAfskContext);
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

bool encodeAx25MessageAsAfsk(Ax25EncodedMessage* pMessage,
                             AfskContext* pAfskContext,
                             uint16_t* pOutputBuffer,
                             uint32_t outputBufferSize)
{
    uint32_t i = 0;
    bool someStuffIsLeftToGenerate = true;

    for (; i < outputBufferSize && pAfskContext->leadingWarmUpQuantsLeft > 0; ++i, --pAfskContext->leadingWarmUpQuantsLeft)
    {
        // make sure HX1 has a chance to warm up
        pOutputBuffer[i] = QUANT_MIN_VALUE;
    }

    for (; i < outputBufferSize; ++i)
    {
        if (pAfskContext->currentSymbolQuant >= QUANTS_COUNT_PER_SYMBOL)
        {
            pAfskContext->currentSymbolQuant = 0;

            if (pAfskContext->pos.chars >= pMessage->size.chars &&
                pAfskContext->pos.lastCharBits >= pMessage->size.lastCharBits)
            {
                if (pAfskContext->lastCharacterGenerated == false)
                {
                    // make sure we finish current symbol correctly by generating one more quant
                    pAfskContext->lastCharacterGenerated = true;
                }
                else
                {
                    someStuffIsLeftToGenerate = false;
                    // nothing else to send but we can continue putting minimum values to make sure DAC buffer is properly filled in
                    pOutputBuffer[i] = QUANT_MIN_VALUE;
                    continue;
                }
            }
            else
            {
                // bit stream is already AFSK encoded so we simply send ones and zeroes as is
                const bool isOne = pMessage->buffer[pAfskContext->pos.chars] & (1 << pAfskContext->pos.lastCharBits);

                // make sure new 'zero' bit frequency is 2200
                if (!isOne && pAfskContext->currentFrequencyIsF1200)
                {
                    pAfskContext->currentF2200Quant = CALCULATE_F2200_QUANT_IDX_FROM_F1200_QUANT_IDX(pAfskContext);
                    pAfskContext->currentFrequencyIsF1200 = false;
                }
                // make sure new 'one' bit frequency is 1200
                else if (isOne && !pAfskContext->currentFrequencyIsF1200)
                {
                    pAfskContext->currentF1200Quant = CALCULATE_F1200_QUANT_IDX_FROM_F2200_QUANT_IDX(pAfskContext);
                    pAfskContext->currentFrequencyIsF1200 = true;
                }

                advanceBitstreamBit(&pAfskContext->pos);
            }
        }

        if (pAfskContext->currentFrequencyIsF1200)
        {
            pOutputBuffer[i] = CALCULATE_F1200_AMPLITUDE_FROM_QUANT_IDX(pAfskContext);
            pAfskContext->currentF1200Quant += QUANT_STEP_SIZE;
            if (pAfskContext->currentF1200Quant >= QUANTS_COUNT_PER_SYMBOL_F1200)
            {
                pAfskContext->currentF1200Quant -= QUANTS_COUNT_PER_SYMBOL_F1200;
            }
        }
        else
        {
            pOutputBuffer[i] = CALCULATE_F2200_AMPLITUDE_FROM_QUANT_IDX(pAfskContext);
            pAfskContext->currentF2200Quant += QUANT_STEP_SIZE;
            if (pAfskContext->currentF2200Quant >= QUANTS_COUNT_PER_SYMBOL_F2200)
            {
                pAfskContext->currentF2200Quant -= QUANTS_COUNT_PER_SYMBOL_F2200;
            }
        }

        ++pAfskContext->currentSymbolQuant;
    }

    return someStuffIsLeftToGenerate;
}
