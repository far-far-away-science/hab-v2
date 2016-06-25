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

#define PARSE_LAST_DUMMY_TOKEN(nmeaParsingContext) \
    if (NPR_IS_INVALID(parseLastDummyToken(&(nmeaParsingContext)))) { return; }

#define PARSE_GPS_TIME(nmeaParsingContext, utcTime) \
    if (NPR_IS_INVALID(parseGpsTime(&(nmeaParsingContext), &(utcTime)))) { return; }

#define PARSE_LATITUDE(nmeaParsingContext, angularCoordinate) \
    if (NPR_IS_INVALID(parseAngularCoordinate(&(nmeaParsingContext), ACR_LATITUDE, &(angularCoordinate)))) { return; }

#define PARSE_LONGITUDE(nmeaParsingContext, angularCoordinate) \
    if (NPR_IS_INVALID(parseAngularCoordinate(&(nmeaParsingContext), ACR_LONGITUDE, &(angularCoordinate)))) { return; }

#define PARSE_FIX_TYPE(nmeaParsingContext, fixType) \
    if (NPR_IS_INVALID(parseFixType(&(nmeaParsingContext), &(fixType)))) { return; }

#define PARSE_FIXED_POINT_W2F0(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseFixedPoint(&(nmeaParsingContext), 0, 2, 0, (result).string))) { return; }

#define PARSE_FIXED_POINT_W3F1(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseFixedPoint(&(nmeaParsingContext), 0, 3, 1, (result).string))) { return; }

#define PARSE_FIXED_POINT_W5F1(nmeaParsingContext, result) \
    if (NPR_IS_INVALID(parseFixedPoint(&(nmeaParsingContext), 0, 5, 1, (result).string))) { return; }

#define PARSE_CHECKSUM(nmeaParsingContext, checksum) \
    if (NPR_IS_INVALID(parseChecksum(&(nmeaParsingContext), &(checksum)))) { return; }

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

NMEA_PARSING_RESULT parseLastDummyToken(NmeaParsingContext* pContext);

NMEA_PARSING_RESULT findNextTokenStart(NmeaParsingContext* pContext);

NMEA_PARSING_RESULT parseHemisphere(NmeaParsingContext* pContext, HEMISPHERE* pHemisphere);

NMEA_PARSING_RESULT parseFixType(NmeaParsingContext* pContext, GPS_FIX_TYPE* pFixType);

NMEA_PARSING_RESULT parseGpsTime(NmeaParsingContext* pContext, GpsTime* pTime);
NMEA_PARSING_RESULT parseAngularCoordinate(NmeaParsingContext* pContext, AngularCoordinateType angularCoordinateType, AngularCoordinate* pCoordinate);

NMEA_PARSING_RESULT parseFixedPoint(NmeaParsingContext* pContext,
                                    uint8_t minNumberOfWholeDigits,
                                    uint8_t maxNumberOfWholeDigits,
                                    uint8_t maxFractionalDigitsCount,
                                    uint8_t* pResult);

NMEA_PARSING_RESULT parseFixedNumberOfDigits(NmeaParsingContext* pContext, uint8_t digitsCount, uint8_t* pResultBuffer);

NMEA_PARSING_RESULT parseChecksum(NmeaParsingContext* pContext, uint8_t* pChecksum);

bool isChecksumValid(uint8_t expectedCheckSum, const uint8_t* pBuffer, uint8_t bufferSize);
