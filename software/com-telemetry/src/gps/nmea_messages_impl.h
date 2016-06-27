#pragma once

#include "nmea_messages.h"

typedef enum AngularCoordinateType_t
{
    ACR_LATITUDE  = 2,
    ACR_LONGITUDE = 3,
} AngularCoordinateType;

#define NMEA_NO_POINT                       4294967295
#define NMEA_UNLIMITED_NUMBER_OF_CHARACTERS 4294967295

#define NMEA_MAX_UINT32_DIV_10 429496729

#define NPR_IS_VALID(npr) \
    ((npr) & NPR_INVALID) == 0

#define NPR_IS_INVALID(npr) \
    ((npr) & NPR_INVALID) != 0

#define PARSE_DUMMY_TOKEN(nmeaParsingContext) \
    if (NPR_IS_INVALID(findNextTokenStart(&(nmeaParsingContext)))) { return; }

#define PARSE_GPS_TIME(nmeaParsingContext, utcTime) \
    if (NPR_IS_INVALID(parseGpsTime(&(nmeaParsingContext), &(utcTime)))) { return; }

#define PARSE_LATITUDE(nmeaParsingContext, angularCoordinate) \
    if (NPR_IS_INVALID(parseAngularCoordinate(&(nmeaParsingContext), ACR_LATITUDE, &(angularCoordinate)))) { return; }

#define PARSE_LONGITUDE(nmeaParsingContext, angularCoordinate) \
    if (NPR_IS_INVALID(parseAngularCoordinate(&(nmeaParsingContext), ACR_LONGITUDE, &(angularCoordinate)))) { return; }

#define PARSE_UINT8_DEFAULT_TO_0(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseUInt8(&(nmeaParsingContext), NMEA_UNLIMITED_NUMBER_OF_CHARACTERS, &(result)))) { return; }

#define PARSE_FIXED_POINT_UINT32_F1_DEFAULT_TO_0(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseUInt32FixedPoint(&(nmeaParsingContext), 0, 1, &(result)))) { return; }

#define PARSE_FIXED_POINT_UINT16_F1_DEFAULT_TO_0(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseUInt16FixedPoint(&(nmeaParsingContext), 0, 1, &(result)))) { return; }

#define PARSE_FIXED_POINT_UINT16_F2_DEFAULT_TO_0(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseUInt16FixedPoint(&(nmeaParsingContext), 0, 2, &(result)))) { return; }

typedef struct NmeaParsingContext_t
{
    const NmeaMessage* pMessage;
    uint32_t tokenStartIdx;
} NmeaParsingContext;

typedef enum NMEA_PARSING_RESULT_t
{
    NPR_VALID                            = 0x00,
    NPR_INVALID                          = 0x01,
    NPR_EMPTY_VALUE                      = 0x02,
    NPR_OVERFLOW                         = 0x04 | NPR_INVALID,
    NPR_NOT_ENOUGH_DIGITS                = 0x08 | NPR_INVALID,
    NPR_UNEXPECTED_END_OF_MESSAGE        = 0x10 | NPR_INVALID,
    NPR_UNEXPECTED_CHARACTER_ENCOUNTERED = 0x20 | NPR_INVALID,
    NPR_UNEXPECTED_SEPARATOR_ENCOUNTERED = 0x40 | NPR_INVALID,
    NPR_INVALID_FORMAT                   = 0x80 | NPR_INVALID,
} NMEA_PARSING_RESULT;

bool canUInt32Overflow(uint32_t previousValue, uint8_t newDigit);

NMEA_PARSING_RESULT findNextTokenStart(NmeaParsingContext* pContext);

NMEA_PARSING_RESULT parseHemisphere(NmeaParsingContext* pContext, HEMISPHERE* pHemisphere);

/*
 * maxFractionalDigitsCount - higher precision than specified will be ignored
 *                          - if not enough fractional digits is encountered then zeroes will be suffixed automatically to the number ('1.1' for W2F2 will result in '110')
 *                          - if decimal point isn't encountered it's no big deal
 */
NMEA_PARSING_RESULT parseUInt32FixedPoint(NmeaParsingContext* pContext, uint8_t minNumberOfWholeDigits, uint8_t maxFractionalDigitsCount, uint32_t* pResult);
NMEA_PARSING_RESULT parseUInt16FixedPoint(NmeaParsingContext* pContext, uint8_t minNumberOfWholeDigits, uint8_t maxFractionalDigitsCount, uint16_t* pResult);

/*
 * Used to parse part of digits from the token
 */
NMEA_PARSING_RESULT parseUInt8(NmeaParsingContext* pContext, uint32_t maxNumberOfCharactersToConsider, uint8_t* pResult);

NMEA_PARSING_RESULT parseGpsTime(NmeaParsingContext* pContext, GpsTime* pTime);
NMEA_PARSING_RESULT parseAngularCoordinate(NmeaParsingContext* pContext, AngularCoordinateType angularCoordinateType, AngularCoordinate* pCoordinate);
