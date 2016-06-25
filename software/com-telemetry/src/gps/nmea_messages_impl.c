#include "nmea_messages_impl.h"

#include <math.h>
#include <ctype.h>
#include <stdlib.h>

bool isSeparator(uint8_t c)
{
    return c == ',' || c == '*';
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

NMEA_PARSING_RESULT parseLastDummyToken(NmeaParsingContext* pContext)
{
    return NPR_INVALID; // TODO
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

    if (NPR_IS_INVALID(result = parseFixedNumberOfDigits(pContext, 2, pTime->hours.string)))
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
    if (NPR_IS_INVALID(result = parseFixedNumberOfDigits(pContext, 2, pTime->minutes.string)))
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
    if (NPR_IS_INVALID(result = parseFixedPoint(pContext, 2, 2, 2, pTime->seconds.string)))
    {
        return result;
    }
    else if (result == NPR_EMPTY_VALUE)
    {
        return NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED;
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
        if (NPR_IS_INVALID(resultCoordinate = parseFixedNumberOfDigits(pContext, angularCoordinateType, pCoordinate->degrees.string)))
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
        if (NPR_IS_INVALID(resultCoordinate = parseFixedPoint(pContext, 2, 2, 6, pCoordinate->minutes.string)))
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

    switch (angularCoordinateType)
    {
        case ACR_LATITUDE:
        {
            if (pCoordinate->hemisphere != H_NORTH && pCoordinate->hemisphere != H_SOUTH)
            {
                return NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            break;
        }
        case ACR_LONGITUDE:
        {
            if (pCoordinate->hemisphere != H_EAST && pCoordinate->hemisphere != H_WEST)
            {
                return NPR_UNEXPECTED_CHARACTER_ENCOUNTERED;
            }
            break;
        }
        default: return NPR_INVALID;
    }

    pCoordinate->isValid = true;

    return NPR_VALID;
}

NMEA_PARSING_RESULT parseFixType(NmeaParsingContext* pContext, GPS_FIX_TYPE* pFixType)
{
    return NPR_INVALID; // TODO
}

NMEA_PARSING_RESULT parseFixedNumberOfDigits(NmeaParsingContext* pContext, uint8_t digitsCount, uint8_t* pResultBuffer)
{
    return NPR_INVALID; // TODO
}

NMEA_PARSING_RESULT parseFixedPoint(NmeaParsingContext* pContext,
                                    uint8_t minNumberOfWholeDigits,
                                    uint8_t maxNumberOfWholeDigits,
                                    uint8_t maxFractionalDigitsCount,
                                    uint8_t* pResult)
{
    return NPR_INVALID; // TODO
}

NMEA_PARSING_RESULT parseChecksum(NmeaParsingContext* pContext, uint8_t* pChecksum)
{
    return NPR_INVALID; // TODO
}

bool isChecksumValid(uint8_t expectedCheckSum, const uint8_t* pBuffer, uint8_t bufferSize)
{
    return false; // TODO
}
