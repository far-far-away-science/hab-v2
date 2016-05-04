#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "nmea_buffer.h"

/*
 * Design choices:
 * - need to minimize data conversion from strings to numbers, etc.
 * - I2C accepts integers
 * - APRS accepts strings slightly formatted compared to nmea
 *
 * Frequencies
 * - APRS once in 30 seconds
 * - I2C once every message is received
 */

typedef enum HEMISPHERE_t
{
    H_UNKNOWN = '?',
    H_NORTH   = 'N',
    H_SOUTH   = 'S',
    H_EAST    = 'E',
    H_WEST    = 'W',
} HEMISPHERE;

typedef enum GPS_FIX_TYPE_t
{
    GPSFT_INVALID            = 0,
    GPSFT_GPS                = 1, // normal GPS
    GPSFT_DGPS               = 2, // differential GPS, high precision
    GPSFT_PPS                = 3,
    GPSFT_REALTIME_KINEMATIC = 4,
    GPSFT_FLOAT_RTK          = 5,
    GPSFT_ESTIMATED          = 6,
    GPSFT_MANUAL_INPUT_MODE  = 7, // previously retreived / entered value stored in device's ROM
    GPSFT_SIMULATION_MODE    = 8,
} GPS_FIX_TYPE;

typedef uint16_t fixedPointW3F1_t;
typedef uint16_t fixedPointW2F2_t;
typedef uint32_t fixedPointW2F6_t;
typedef uint32_t fixedPointW5F1_t;

typedef struct GpsTime_t
{
    bool isValid;
    uint8_t hours;
    uint8_t minutes;
    fixedPointW2F2_t seconds;
} GpsTime;

typedef struct AngularCoordinate_t
{
    bool isValid;
    uint8_t degrees;
    fixedPointW2F6_t minutes;
    HEMISPHERE hemisphere;
} AngularCoordinate;

typedef struct GpggaData_t
{
    GpsTime utcTime;
    AngularCoordinate latitude;
    AngularCoordinate longitude;
    fixedPointW5F1_t altitudeMslMeters;
    GPS_FIX_TYPE fixType;
    uint8_t numberOfSattelitesInUse;
} GpggaData;

typedef struct GpvtgData_t
{
    fixedPointW3F1_t trueCourseDegrees;
    fixedPointW3F1_t speedKph;
} GpvtgData;

/*
 * had to choose to use floats as it seems that nmea messages precision can vary given number of sattelites? settings?
 */
typedef struct GpsData_t
{
    bool isValid;
    GpggaData gpggaData;
    GpvtgData gpvtgData;
} GpsData;

// degrees times 10^6
int32_t angularCoordinateToInt32Degrees(AngularCoordinate lat);

void parseGpggaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult);
void parseGpvtgMessageIfValid(const NmeaMessage* pGpvtgMessage, GpsData* pResult);
