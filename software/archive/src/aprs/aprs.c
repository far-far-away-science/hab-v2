#include "aprs_impl.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <crc/crc.h>

#include <signals/signals.h>

#define CRC_POLYNOMIAL                0x1021
#define CRC_POST_PROCESSING_XOR_VALUE 0xFFFF

static uint16_t g_telemetryMessageId;
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

bool encodeAprsMessage(const Callsign* pCallsign, const uint8_t* aprsPayloadBuffer, uint8_t aprsPayloadBufferLen, Ax25EncodedMessage* pAx25EncodedMessage)
{
    if (!pCallsign || !aprsPayloadBuffer || !aprsPayloadBufferLen || !pAx25EncodedMessage)
    {
        return false;
    }

    memset(pAx25EncodedMessage, 0, sizeof(Ax25EncodedMessage));

    resetCrc(CRC_POLYNOMIAL);

    Ax25EncodingContext encodingCtx = { 0 };

    encodeAndAppendPrefixAsAx25(&encodingCtx, pAx25EncodedMessage);

    // addresses to and from

    encodeAndAppendDataAsAx25(CALLSIGN_DESTINATION_1.callsign, 6, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25(&CALLSIGN_DESTINATION_1.ssid, 1, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25(pCallsign->callsign, 6, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25(&pCallsign->ssid, 1, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25(CALLSIGN_DESTINATION_2.callsign, 6, FCS_CALCULATE, SHIFT_ONE_LEFT, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25(&CALLSIGN_DESTINATION_2.ssid, 1, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);

    // control bytes

    encodeAndAppendDataAsAx25((const uint8_t*) "\x03", 1, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);
    encodeAndAppendDataAsAx25((const uint8_t*) "\xF0", 1, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);

    // packet contents

    encodeAndAppendDataAsAx25(aprsPayloadBuffer, aprsPayloadBufferLen, FCS_CALCULATE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);

    // FCS

    uint16_t fcs = getCalculatedCrc() ^ CRC_POST_PROCESSING_XOR_VALUE;
    uint8_t fcsByte = fcs & 0x00FF; // get low byte
    encodeAndAppendDataAsAx25(&fcsByte, 1, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);
    fcsByte = (fcs >> 8) & 0x00FF; // get high byte
    encodeAndAppendDataAsAx25(&fcsByte, 1, FCS_NONE, SHIFT_ONE_LEFT_NO, &encodingCtx, pAx25EncodedMessage);

    disableCrc();

    encodeAndAppendSuffixAsAx25(&encodingCtx, pAx25EncodedMessage);

    return true;
}

bool encodeNmeaAprsMessage(const Callsign* pCallsign, const NmeaMessage* pNmeaMessage, Ax25EncodedMessage* pAx25EncodedMessage)
{
    const uint8_t payloadSize = pNmeaMessage->size > APRS_PAYLOAD_BUFFER_MAX_LENGTH ? APRS_PAYLOAD_BUFFER_MAX_LENGTH : pNmeaMessage->size;
    memcpy(g_aprsPayloadBuffer, pNmeaMessage->message, payloadSize);
    return encodeAprsMessage(pCallsign, g_aprsPayloadBuffer, payloadSize, pAx25EncodedMessage);
}

uint8_t threeDigitInt2str(uint16_t value, uint8_t* pBuffer)
{
    if (value < 10)
    {
        pBuffer[0] = '0';
        pBuffer[1] = '0';
        pBuffer[2] = '0' + value;
    }
    else if (value < 100)
    {
        pBuffer[0] = '0';
        pBuffer[1] = '0' + (value / 10);
        pBuffer[2] = '0' + (value % 10);
    }
    else if (value < 1000)
    {
        pBuffer[0] = '0' + (value / 100);
        value = value % 100;
        pBuffer[1] = '0' + (value / 10);
        pBuffer[2] = '0' + (value % 10);
    }
    else
    {
        pBuffer[0] = '9';
        pBuffer[1] = '9';
        pBuffer[2] = '9';
    }
    return 3;
}

uint8_t uint8ToBinaryStr(uint8_t value, uint8_t* pBuffer)
{
    for (int8_t iBit = 0x80, idx = 0; iBit > 0; iBit >>= 1, ++idx)
    {
        if ((value & iBit) != 0)
        {
            pBuffer[idx] = '1';
        }
        else
        {
            pBuffer[idx] = '0';
        }
    }
    return 8;
}

uint8_t createTelemetryAprsPayload(const Telemetry* pTelemetry, uint8_t* pAprsPayloadBuffer, uint8_t aprsPayloadBufferMaxLength)
{
    if (aprsPayloadBufferMaxLength < 34)
    {
        return 0;
    }

    uint8_t currentIdx = 0;

    if (g_telemetryMessageId >= 1000)
    {
        g_telemetryMessageId = 0;
    }

    pAprsPayloadBuffer[currentIdx++] = 'T';
    pAprsPayloadBuffer[currentIdx++] = '#';
    currentIdx += threeDigitInt2str(g_telemetryMessageId++, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';
    currentIdx += threeDigitInt2str(pTelemetry->batteryVoltage, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';
    currentIdx += threeDigitInt2str(pTelemetry->batteryTemperature, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';
    currentIdx += threeDigitInt2str(pTelemetry->cpuTemperature, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';
    currentIdx += threeDigitInt2str(pTelemetry->gpsChipTemperature, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';
    currentIdx += threeDigitInt2str(pTelemetry->maxAccelerationOnAnyAxis, &pAprsPayloadBuffer[currentIdx]);
    pAprsPayloadBuffer[currentIdx++] = ',';

    const uint8_t errorMask = getErrorsMask();
    currentIdx += uint8ToBinaryStr(errorMask, &pAprsPayloadBuffer[currentIdx]);

    return currentIdx;
}

bool encodeTelemetryAprsMessage(const Callsign* pCallsign, const Telemetry* pTelemetry, Ax25EncodedMessage* pAx25EncodedMessage)
{
    uint8_t aprsPayloadBufferDataLength = createTelemetryAprsPayload(pTelemetry, g_aprsPayloadBuffer, APRS_PAYLOAD_BUFFER_MAX_LENGTH);
    return encodeAprsMessage(pCallsign, g_aprsPayloadBuffer, aprsPayloadBufferDataLength, pAx25EncodedMessage);
}
