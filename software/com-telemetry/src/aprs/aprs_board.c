#include "aprs_board_impl.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <aprs/trigonometry/trigonometry.h>

static uint8_t g_aprsPayloadBuffer[APRS_PAYLOAD_BUFFER_MAX_LENGTH];

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

bool encodeAndAppendBits(const uint8_t* pMessageData,
                         uint16_t messageDataSize,
                         STUFFING_TYPE stuffingType,
                         FCS_TYPE fcsType,
                         SHIFT_ONE_LEFT_TYPE shiftOneLeftType,
                         EncodingContext* pEncodingContext,
                         AprsEncodedMessage* pResultAprsEncodedMessage)
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
                if (pResultAprsEncodedMessage->size.chars >= MAX_APRS_MESSAGE_LENGTH)
                {
                    return false;
                }
                // as we are encoding 1 keep current bit as is
                if (pEncodingContext->lastBit)
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
                    ++pEncodingContext->numberOfOnes;
                    
                    if (pEncodingContext->numberOfOnes == 5)
                    {
                        if (pResultAprsEncodedMessage->size.chars >= MAX_APRS_MESSAGE_LENGTH)
                        {
                            return false;
                        }

                        // we need to insert 0 after 5 consecutive ones
                        if (pEncodingContext->lastBit)
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBits));
                            pEncodingContext->lastBit = 0;
                        }
                        else
                        {
                            pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] |= 1 << (pResultAprsEncodedMessage->size.lastCharBits);
                            pEncodingContext->lastBit = 1;
                        }
                        
                        pEncodingContext->numberOfOnes = 0;
                        
                        advanceBitstreamBit(&pResultAprsEncodedMessage->size); // insert zero as we had 5 ones
                    }
                }
            }
            else
            {
                if (pResultAprsEncodedMessage->size.chars >= MAX_APRS_MESSAGE_LENGTH)
                {
                    return false;
                }
                
                // as we are encoding 0 we need to flip bit
                if (pEncodingContext->lastBit)
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] &= ~(1 << (pResultAprsEncodedMessage->size.lastCharBits));
                    pEncodingContext->lastBit = 0;
                }
                else
                {
                    pResultAprsEncodedMessage->buffer[pResultAprsEncodedMessage->size.chars] |= 1 << (pResultAprsEncodedMessage->size.lastCharBits);
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
    if (!pMessage)
    {
        return true;
    }

    return pMessage->size.chars == 0 && pMessage->size.lastCharBits == 0;
}

void resetAfskContext(AfskContext* pAfskContext)
{
    pAfskContext->currentF1200Quant = 0;
    pAfskContext->currentF2200Quant = 0;
    pAfskContext->currentFrequencyIsF1200 = true;
    pAfskContext->currentSymbolQuant = F1200_QUANTS_COUNT_PER_SYMBOL;
    pAfskContext->leadingOneBitsLeft = LEADING_ONES_COUNT_TO_CANCEL_PREVIOUS_PACKET;
    pAfskContext->leadingWarmUpQuantsLeft = LEADING_WARMUP_QUANTS_COUNT;
    pAfskContext->pos.chars = 0;
    pAfskContext->pos.lastCharBits = 0;
}

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, AprsEncodedMessage* pEncdedMessage)
{
    if (!pCallsign || !aprsPayloadBuffer || !aprsPayloadBufferLen || !pEncdedMessage)
    {
        return false;
    }

    EncodingContext encodingCtx = { 0 };
    encodingCtx.lastBit = 1;
    encodingCtx.fcs = FCS_INITIAL_VALUE;

    for (uint8_t i = 0; i < PREFIX_FLAGS_COUNT; ++i)
    {
        encodeAndAppendBits((const uint8_t*) "\x7E", 1, ST_NO_STUFFING, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    }

    // addresses to and from

    encodeAndAppendBits(CALLSIGN_DESTINATION_1.callsign, 6, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits(&CALLSIGN_DESTINATION_1.ssid, 1, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits(pCallsign->callsign, 6, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits(&pCallsign->ssid, 1, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits(CALLSIGN_DESTINATION_2.callsign, 6, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits(&CALLSIGN_DESTINATION_2.ssid, 1, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);

    // control bytes

    encodeAndAppendBits((const uint8_t*) "\x03", 1, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    encodeAndAppendBits((const uint8_t*) "\xF0", 1, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);

    // packet contents

    encodeAndAppendBits(aprsPayloadBuffer, aprsPayloadBufferLen, ST_PERFORM_STUFFING, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);

    // FCS

    encodingCtx.fcs ^= FCS_POST_PROCESSING_XOR_VALUE;
    uint8_t fcsByte = encodingCtx.fcs & 0x00FF; // get low byte
    encodeAndAppendBits(&fcsByte, 1, ST_PERFORM_STUFFING, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    fcsByte = (encodingCtx.fcs >> 8) & 0x00FF; // get high byte
    encodeAndAppendBits(&fcsByte, 1, ST_PERFORM_STUFFING, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);

    // suffix flags

    for (uint8_t i = 0; i < SUFFIX_FLAGS_COUNT; ++i)
    {
        encodeAndAppendBits((const uint8_t*) "\x7E", 1, ST_NO_STUFFING, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pEncdedMessage);
    }

    resetAfskContext(&pEncdedMessage->afskContext);

    return true;
}

uint8_t createGpsAprsPayload(const GpsData* pGpsData, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength)
{
    uint8_t bufferStartIdx = 0;

    //if (pGpsData->gpggaData.latitude.isValid && pGpsData->gpggaData.longitude.isValid)
    {
        if (pGpsData->gpggaData.utcTime.isValid)
        {
            if (bufferStartIdx + 8 > aprsPayloadBufferMaxLength)
            {
                return 0;
            }

            bufferStartIdx += sprintf((char*) &g_aprsPayloadBuffer[bufferStartIdx],
                                      "@%02u%02u%02uz",
                                      pGpsData->gpggaData.utcTime.hours,
                                      pGpsData->gpggaData.utcTime.minutes,
                                      pGpsData->gpggaData.utcTime.seconds / 100);
        }
        else
        {
            if (bufferStartIdx + 1 > aprsPayloadBufferMaxLength)
            {
                return 0;
            }

            g_aprsPayloadBuffer[bufferStartIdx++] = '!';
        }

        if (bufferStartIdx + 20 > aprsPayloadBufferMaxLength)
        {
            return 0;
        }

        const unsigned int latMinutesWhole = pGpsData->gpggaData.latitude.minutes / 1000000;
        const unsigned int latMinutesFraction = (pGpsData->gpggaData.latitude.minutes - latMinutesWhole * 1000000) / 10000;

        const unsigned int lonMinutesWhole = pGpsData->gpggaData.longitude.minutes / 1000000;
        const unsigned int lonMinutesFraction = (pGpsData->gpggaData.longitude.minutes - lonMinutesWhole * 1000000) / 10000;

        bufferStartIdx += sprintf((char*) &g_aprsPayloadBuffer[bufferStartIdx],
                                  "%02u%02u.%02u%1c/%03u%02u.%02u%1cO",
                                  pGpsData->gpggaData.latitude.degrees,
                                  latMinutesWhole,
                                  latMinutesFraction,
                                  pGpsData->gpggaData.latitude.hemisphere,
                                  pGpsData->gpggaData.longitude.degrees,
                                  lonMinutesWhole,
                                  lonMinutesFraction,
                                  pGpsData->gpggaData.longitude.hemisphere);

        if (bufferStartIdx + 7 > aprsPayloadBufferMaxLength)
        {
            return 0;
        }

        bufferStartIdx += sprintf((char*) &g_aprsPayloadBuffer[bufferStartIdx],
                                  ">%03u/%03u",
                                  (unsigned int) (pGpsData->gpvtgData.trueCourseDegrees / 10),
                                  (unsigned int) (pGpsData->gpvtgData.speedKph / 10));

        if (bufferStartIdx + 7 > aprsPayloadBufferMaxLength)
        {
            return 0;
        }

        bufferStartIdx += sprintf((char*) &g_aprsPayloadBuffer[bufferStartIdx],
                                  "@%05um",
                                  (unsigned int) pGpsData->gpggaData.altitudeMslMeters / 10);
    }

    return bufferStartIdx;
}

bool encodeGpsAprsMessage(const Callsign* pCallsign, const GpsData* pGpsData, AprsEncodedMessage* pEncdedMessage)
{
    uint8_t aprsPayloadBufferDataLength = createGpsAprsPayload(pGpsData, g_aprsPayloadBuffer, APRS_PAYLOAD_BUFFER_MAX_LENGTH);
    return encodeAprsMessage(pCallsign, g_aprsPayloadBuffer, aprsPayloadBufferDataLength, pEncdedMessage);
}

uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength)
{
    return 0; // TODO
}

bool encodeTelemetryAprsMessage(const Callsign* pCallsign, const Telemetry* pTelemetry, AprsEncodedMessage* pEncdedMessage)
{
    uint8_t aprsPayloadBufferDataLength = createTelemetryAprsPayload(pTelemetry, g_aprsPayloadBuffer, APRS_PAYLOAD_BUFFER_MAX_LENGTH);
    return encodeAprsMessage(pCallsign, g_aprsPayloadBuffer, aprsPayloadBufferDataLength, pEncdedMessage);
}

float normalizePulseWidth(float width)
{
    if (width < QUANT_MIN_VALUE)
    {
        return QUANT_MIN_VALUE;
    }
    else if (width > QUANT_MAX_VALUE)
    {
        return QUANT_MAX_VALUE;
    }
    return width;
}

bool encodeAprsMessageAsAfsk(AprsEncodedMessage* pMessage, uint16_t* pOutputBuffer, uint32_t outputBufferSize)
{
    for (uint32_t i = 0; i < outputBufferSize; ++i)
    {
        if (pMessage->afskContext.leadingWarmUpQuantsLeft > 0)
        {
            --pMessage->afskContext.leadingWarmUpQuantsLeft;
            // make sure HX1 has a chance to warm up
            pOutputBuffer[i] = QUANT_MIN_VALUE;
        }
        else
        {
            if (pMessage->afskContext.currentSymbolQuant >= F1200_QUANTS_COUNT_PER_SYMBOL)
            {
                pMessage->afskContext.currentSymbolQuant = 0;

                if (pMessage->afskContext.pos.chars >= pMessage->size.chars &&
                    pMessage->afskContext.pos.lastCharBits >= pMessage->size.lastCharBits)
                {
                    // nothing else to send but we can continue putting minimum values to make sure DAC buffer is properly filled in
                    pOutputBuffer[i] = QUANT_MIN_VALUE;
                    continue;
                }
                else if (pMessage->afskContext.leadingOneBitsLeft)
                {
                    // send ones to stabilize HX1 and cancel any previously not-fully received APRS packets
                    pMessage->afskContext.currentFrequencyIsF1200 = true;
                    --pMessage->afskContext.leadingOneBitsLeft;
                }
                else
                {
                    // bit stream is already AFSK encoded so we simply send ones and zeroes as is
                    const bool isOne = pMessage->buffer[pMessage->afskContext.pos.chars] & (1 << pMessage->afskContext.pos.lastCharBits);

                    // make sure new 'zero' bit frequency is 2200
                    if (!isOne && pMessage->afskContext.currentFrequencyIsF1200)
                    {
                        const float triagArg = ANGULAR_FREQUENCY_F1200 * pMessage->afskContext.currentF1200Quant;
                        const float pulseWidth1200 = normalizePulseWidth(AMPLITUDE_SCALED_AND_SHIFTED_SINE(triagArg));
                        const bool pulse1200Positive = COSINE_G_THAN_0(triagArg);

                        if (pulse1200Positive)
                        {
                            pMessage->afskContext.currentF2200Quant = RECIPROCAL_ANGULAR_FREQUENCY_F2200 * INVERSE_SINE(RECIPROCAL_AMPLITUDE_SCALER * (pulseWidth1200 - AMPLITUDE_SHIFT));
                        }
                        else
                        {
                            pMessage->afskContext.currentF2200Quant = HALF_PERIOD_F2200 - RECIPROCAL_ANGULAR_FREQUENCY_F2200 * INVERSE_SINE(RECIPROCAL_AMPLITUDE_SCALER * (pulseWidth1200 - AMPLITUDE_SHIFT));
                        }

                        if (pMessage->afskContext.currentF2200Quant < 0)
                        {
                            pMessage->afskContext.currentF2200Quant += F2200_QUANTS_COUNT_PER_SYMBOL;
                        }

                        pMessage->afskContext.currentFrequencyIsF1200 = false;
                    }
                    // make sure new 'one' bit frequency is 1200
                    else if (isOne && !pMessage->afskContext.currentFrequencyIsF1200)
                    {
                        const float trigArg = ANGULAR_FREQUENCY_F2200 * pMessage->afskContext.currentF2200Quant;
                        const float pulseWidth2200 = normalizePulseWidth(AMPLITUDE_SCALED_AND_SHIFTED_SINE(trigArg));
                        const bool pulse2200Positive = COSINE_G_THAN_0(trigArg);

                        if (pulse2200Positive)
                        {
                            pMessage->afskContext.currentF1200Quant = RECIPROCAL_ANGULAR_FREQUENCY_F1200 * INVERSE_SINE(RECIPROCAL_AMPLITUDE_SCALER * (pulseWidth2200 - AMPLITUDE_SHIFT));
                        }
                        else
                        {
                            pMessage->afskContext.currentF1200Quant = HALF_PERIOD_F1200 - RECIPROCAL_ANGULAR_FREQUENCY_F1200 * INVERSE_SINE(RECIPROCAL_AMPLITUDE_SCALER * (pulseWidth2200 - AMPLITUDE_SHIFT));
                        }

                        if (pMessage->afskContext.currentF1200Quant < 0)
                        {
                            pMessage->afskContext.currentF1200Quant += F1200_QUANTS_COUNT_PER_SYMBOL;
                        }

                        pMessage->afskContext.currentFrequencyIsF1200 = true;
                    }

                    advanceBitstreamBit(&pMessage->afskContext.pos);
                }
            }

            if (pMessage->afskContext.currentFrequencyIsF1200)
            {
                const uint16_t pulseWidth = (uint16_t) AMPLITUDE_SCALED_AND_SHIFTED_SINE(ANGULAR_FREQUENCY_F1200 * pMessage->afskContext.currentF1200Quant);
                pMessage->afskContext.currentF1200Quant += QUANT_STEP_SIZE;
                if (pMessage->afskContext.currentF1200Quant >= F1200_QUANTS_COUNT_PER_SYMBOL)
                {
                    pMessage->afskContext.currentF1200Quant -= F1200_QUANTS_COUNT_PER_SYMBOL;
                }
                pOutputBuffer[i] = pulseWidth;
            }
            else
            {
                const uint16_t pulseWidth = (uint16_t) AMPLITUDE_SCALED_AND_SHIFTED_SINE(ANGULAR_FREQUENCY_F2200 * pMessage->afskContext.currentF2200Quant);
                pMessage->afskContext.currentF2200Quant += QUANT_STEP_SIZE;
                if (pMessage->afskContext.currentF2200Quant >= F2200_QUANTS_COUNT_PER_SYMBOL)
                {
                    pMessage->afskContext.currentF2200Quant -= F2200_QUANTS_COUNT_PER_SYMBOL;
                }
                pOutputBuffer[i] = pulseWidth;
            }

            ++pMessage->afskContext.currentSymbolQuant;
        }
    }

    return pMessage->afskContext.pos.chars < pMessage->size.chars ||
           (pMessage->afskContext.pos.chars == pMessage->size.chars && pMessage->afskContext.pos.lastCharBits < pMessage->size.lastCharBits);
}
