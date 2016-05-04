#include "nmea_messages_impl.h"

#include <math.h>
#include <ctype.h>
#include <stdlib.h>

bool isSeparator(uint8_t c)
{
    return c == ',' || c == '*';
}

bool canUInt32Overflow(uint32_t previousValue, uint8_t newDigit)
{
    if ((previousValue > NMEA_MAX_UINT32_DIV_10) ||
        (previousValue == NMEA_MAX_UINT32_DIV_10 && newDigit > 5))
    {
        return true;
    }
    else
    {
        return false;
    }
}

NMEA_PARSING_RESULT findNextTokenStart(NmeaParsingContext* pContext)
{
    for (;; ++pContext->tokenStartIdx)
    {
        if (pContext->tokenStartIdx >= pContext->pMessage->size)
        {
            return NPR_UNEXPECTED_END_OF_MESSAGE;
        }

        const uint8_t c = pContext->pMessage->message[pContext->tokenStartIdx];

        if (isSeparator(c))
        {
            ++pContext->tokenStartIdx; // move to start of the next token
            return NPR_VALID;
        }
    }

    return NPR_UNEXPECTED_END_OF_MESSAGE;
}

NMEA_PARSING_RESULT parseUInt32FixedPoint(NmeaParsingContext* pContext, uint8_t minNumberOfWholeDigits, uint8_t fractionalDigitsCount, uint32_t* pResult)
{
    bool ignoreRemaningCharacters = false;

    NMEA_PARSING_RESULT result = NPR_VALID;

    uint32_t number = 0;
    uint32_t numberOfDigitsProcessed = 0;
    uint32_t numberOfCharactersProcessed = 0;
    uint32_t decimalPointPosition = NMEA_NO_POINT;

    for (;; ++pContext->tokenStartIdx, ++numberOfCharactersProcessed)
    {
        if (pContext->tokenStartIdx >= pContext->pMessage->size)
        {
            result = NPR_UNEXPECTED_END_OF_MESSAGE;
            break;
        }

        const uint8_t c = pContext->pMessage->message[pContext->tokenStartIdx];

        if (isSeparator(c))
        {
            ++pContext->tokenStartIdx; // move to start of the next token
            break;
        }
        else if (!ignoreRemaningCharacters)
        {
            if (c == '.')
            {
                const bool isSecondPointDetected = decimalPointPosition != NMEA_NO_POINT;

                if (isSecondPointDetected)
                {
                    ignoreRemaningCharacters = true; // ignore fractional digits as precision isn't that big of a deal
                    result = NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
                }
                else
                {
                    decimalPointPosition = numberOfCharactersProcessed;
                }
            }
            else if (!isdigit(c))
            {
                ignoreRemaningCharacters = true;
                result = NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            else
            {
                const uint8_t d = c - '0';

                if (canUInt32Overflow(number, d))
                {
                    result = NPR_OVERFLOW;
                    ignoreRemaningCharacters = true;
                }
                else
                {
                    number = number * 10 + d;
                    ++numberOfDigitsProcessed;

                    const bool isPointEncountered = decimalPointPosition != NMEA_NO_POINT;

                    if (isPointEncountered)
                    {
                        const bool collectedEnoughFractionalDigits = (numberOfCharactersProcessed - decimalPointPosition) >= fractionalDigitsCount;

                        if (collectedEnoughFractionalDigits)
                        {
                            ignoreRemaningCharacters = true; // safely ignore remaining digits
                        }
                    }
                }
            }
        }
    }

    const bool isPointEncountered = decimalPointPosition != NMEA_NO_POINT;

    if (NPR_IS_VALID(result))
    {
        if (isPointEncountered)
        {
            if (decimalPointPosition < minNumberOfWholeDigits)
            {
                result = NPR_NOT_ENOUGH_DIGITS;
            }
        }
        else if (numberOfDigitsProcessed < minNumberOfWholeDigits)
        {
            result = NPR_NOT_ENOUGH_DIGITS;
        }
    }

    if (NPR_IS_VALID(result) && number)
    {
        // as it's a fixed point number we need to fill up missing fractional digits

        const uint32_t encounteredFractionalDigitCount = isPointEncountered ? numberOfCharactersProcessed - decimalPointPosition - 1 : 0;

        for (uint32_t i = encounteredFractionalDigitCount; i < fractionalDigitsCount; ++i)
        {
            if (canUInt32Overflow(number, 0))
            {
                result = NPR_OVERFLOW;
                break;
            }
            number = number * 10;
        }
    }

    if (NPR_IS_VALID(result))
    {
        *pResult = number;
        return numberOfDigitsProcessed == 0 ? NPR_EMPTY_VALUE : NPR_VALID;
    }
    else
    {
        *pResult = 0;
        return result;
    }
}

NMEA_PARSING_RESULT parseUInt16FixedPoint(NmeaParsingContext* pContext, uint8_t minNumberOfWholeDigits, uint8_t fractionalDigitsCount, uint16_t* pResult)
{
    uint32_t result;

    const NMEA_PARSING_RESULT parsingResult = parseUInt32FixedPoint(pContext, minNumberOfWholeDigits, fractionalDigitsCount, &result);

    if (NPR_IS_INVALID(parsingResult))
    {
        *pResult = 0;
        return parsingResult;
    }
    else
    {
        if (result > 65535)
        {
            *pResult = 0;
            return NPR_OVERFLOW;
        }
        else
        {
            *pResult = (uint16_t) result;
            return parsingResult;
        }
    }
}

NMEA_PARSING_RESULT parseUInt8(NmeaParsingContext* pContext, uint32_t maxNumberOfCharactersToConsider, uint8_t* pResult)
{
    bool ignoreRemaningCharacters = false;

    uint16_t number = 0;
    uint32_t numberOfCharactersProcessed = 0;

    NMEA_PARSING_RESULT result = NPR_VALID;

    for (;; ++pContext->tokenStartIdx, ++numberOfCharactersProcessed)
    {
        if (pContext->tokenStartIdx >= pContext->pMessage->size)
        {
            result = NPR_UNEXPECTED_END_OF_MESSAGE;
            break;
        }
        else if (numberOfCharactersProcessed >= maxNumberOfCharactersToConsider)
        {
            break;
        }

        const uint8_t c = pContext->pMessage->message[pContext->tokenStartIdx];

        if (isSeparator(c))
        {
            if (maxNumberOfCharactersToConsider != NMEA_UNLIMITED_NUMBER_OF_CHARACTERS)
            {
                result = NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED;
            }
            ++pContext->tokenStartIdx; // move to start of the next token
            break;
        }
        else if (!ignoreRemaningCharacters)
        {
            if (!isdigit(c))
            {
                ignoreRemaningCharacters = true;
                result = NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            else
            {
                number = number * 10 + (c - '0');

                if (number > 255)
                {
                    result = NPR_OVERFLOW;
                    ignoreRemaningCharacters = true;
                }
            }
        }
    }

    if (NPR_IS_VALID(result))
    {
        *pResult = (uint8_t) number;
        return result;
    }
    else
    {
        *pResult = 0;
        return result;
    }
}

NMEA_PARSING_RESULT parseHemisphere(NmeaParsingContext* pContext, HEMISPHERE* pHemisphere)
{
    if (pContext->tokenStartIdx >= pContext->pMessage->size)
    {
        return NPR_UNEXPECTED_END_OF_MESSAGE;
    }

    NMEA_PARSING_RESULT result = NPR_VALID;
    uint8_t c = pContext->pMessage->message[pContext->tokenStartIdx];

    switch (c)
    {
        case 'n':
        case 'N':
        {
            *pHemisphere = H_NORTH;
            break;
        }
        case 's':
        case 'S':
        {
            *pHemisphere = H_SOUTH;
            break;
        }
        case 'e':
        case 'E':
        {
            *pHemisphere = H_EAST;
            break;
        }
        case 'w':
        case 'W':
        {
            *pHemisphere = H_WEST;
            break;
        }
        case ',':
        {
            *pHemisphere = H_UNKNOWN;
            ++pContext->tokenStartIdx;
            return NPR_EMPTY_VALUE;
        }
        default:
        {
            result = NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
        }
    }

    ++pContext->tokenStartIdx;

    if (pContext->tokenStartIdx >= pContext->pMessage->size)
    {
        return NPR_UNEXPECTED_END_OF_MESSAGE;
    }

    c = pContext->pMessage->message[pContext->tokenStartIdx];

    if (c == ',')
    {
        ++pContext->tokenStartIdx;
        return result;
    }
    else
    {
        findNextTokenStart(pContext);
        return NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
    }
}

NMEA_PARSING_RESULT parseGpsTime(NmeaParsingContext* pContext, GpsTime* pTime)
{
    pTime->isValid = false;

    if (pContext->tokenStartIdx >= pContext->pMessage->size)
    {
        return NPR_UNEXPECTED_END_OF_MESSAGE;
    }
    if (pContext->pMessage->message[pContext->tokenStartIdx] == ',')
    {
        ++pContext->tokenStartIdx;
        return NPR_EMPTY_VALUE;
    }

    NMEA_PARSING_RESULT result;

    if (NPR_IS_INVALID(result = parseUInt8(pContext, 2, &pTime->hours)))
    {
        if (result != NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED)
        {
            findNextTokenStart(pContext); // skip to next token
        }
        else
        {
            result = NPR_NOT_ENOUGH_DIGITS;
        }
        return result;
    }
    if (NPR_IS_INVALID(result = parseUInt8(pContext, 2, &pTime->minutes)))
    {
        if (result != NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED)
        {
            findNextTokenStart(pContext); // skip to next token
        }
        else
        {
            result = NPR_NOT_ENOUGH_DIGITS;
        }
        return result;
    }
    if (NPR_IS_INVALID(result = parseUInt16FixedPoint(pContext, 2, 2, &pTime->seconds)))
    {
        return result;
    }
    else if (result == NPR_EMPTY_VALUE)
    {
        return NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED;
    }

    if (pTime->hours > 23 ||
        pTime->minutes > 59 ||
        pTime->seconds > 5999)
    {
        return NPR_OVERFLOW;
    }

    pTime->isValid = true;

    return NPR_VALID;
}

NMEA_PARSING_RESULT parseAngularCoordinate(NmeaParsingContext* pContext, AngularCoordinateType angularCoordinateType, AngularCoordinate* pCoordinate)
{
    pCoordinate->isValid = false;

    if (pContext->tokenStartIdx >= pContext->pMessage->size)
    {
        return NPR_UNEXPECTED_END_OF_MESSAGE;
    }

    NMEA_PARSING_RESULT resultCoordinate;

    if (pContext->pMessage->message[pContext->tokenStartIdx] == ',')
    {
        ++pContext->tokenStartIdx;
        resultCoordinate = NPR_EMPTY_VALUE;
    }
    else
    {
        if (NPR_IS_INVALID(resultCoordinate = parseUInt8(pContext, angularCoordinateType, &pCoordinate->degrees)))
        {
            if (resultCoordinate != NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED)
            {
                findNextTokenStart(pContext); // skip current angular coordinate
            }
            else
            {
                resultCoordinate = NPR_NOT_ENOUGH_DIGITS;
            }
            findNextTokenStart(pContext); // skip hemisphere
            return resultCoordinate;
        }
        if (NPR_IS_INVALID(resultCoordinate = parseUInt32FixedPoint(pContext, 2, 6, &pCoordinate->minutes)))
        {
            findNextTokenStart(pContext); // skip hemisphere
            return resultCoordinate;
        }
    }

    NMEA_PARSING_RESULT resultHemisphere;

    if (NPR_IS_INVALID(resultHemisphere = parseHemisphere(pContext, &pCoordinate->hemisphere)))
    {
        return resultHemisphere;
    }

    if ((resultCoordinate == NPR_EMPTY_VALUE || resultHemisphere == NPR_EMPTY_VALUE))
    {
        return resultCoordinate == resultHemisphere ? NPR_EMPTY_VALUE : NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
    }

    if (pCoordinate->minutes >= 60000000)
    {
        return NPR_OVERFLOW;
    }

    switch (angularCoordinateType)
    {
        case ACR_LATITUDE:
        {
            if (pCoordinate->hemisphere != H_NORTH && pCoordinate->hemisphere != H_SOUTH)
            {
                return NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            if (pCoordinate->degrees >= 91)
            {
                return NPR_OVERFLOW;
            }
            break;
        }
        case ACR_LONGITUDE:
        {
            if (pCoordinate->hemisphere != H_EAST && pCoordinate->hemisphere != H_WEST)
            {
                return NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            if (pCoordinate->degrees >= 181)
            {
                return NPR_OVERFLOW;
            }
            break;
        }
        default: return NPR_INVALID;
    }

    pCoordinate->isValid = true;

    return NPR_VALID;
}
