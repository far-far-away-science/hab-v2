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
    GPSFT_INVALID            = '0',
    GPSFT_GPS                = '1', // normal GPS
    GPSFT_DGPS               = '2', // differential GPS, high precision
    GPSFT_PPS                = '3',
    GPSFT_REALTIME_KINEMATIC = '4',
    GPSFT_FLOAT_RTK          = '5',
    GPSFT_ESTIMATED          = '6',
    GPSFT_MANUAL_INPUT_MODE  = '7', // previously retreived / entered value stored in device's ROM
    GPSFT_SIMULATION_MODE    = '8',
} GPS_FIX_TYPE;

typedef struct fixedPointW2F0_t
{
    uint8_t string[2];
} fixedPointW2F0;

typedef struct fixedPointW2F2_t
{
    uint8_t string[5];
} fixedPointW2F2;

typedef struct fixedPointW2F6_t
{
    uint8_t string[9];
} fixedPointW2F6;

typedef struct fixedPointW3F0_t
{
    uint8_t string[3];
} fixedPointW3F0;

typedef struct fixedPointW3F1_t
{
    uint8_t string[5];
} fixedPointW3F1;

typedef struct fixedPointW5F1_t
{
    uint8_t string[7];
} fixedPointW5F1;

typedef struct GpsTime_t
{
    bool isValid;
    fixedPointW2F0 hours;
    fixedPointW2F0 minutes;
    fixedPointW2F2 seconds;
} GpsTime;

typedef struct AngularCoordinate_t
{
    bool isValid;
    fixedPointW3F0 degrees; // 0-180 or 0-90
    fixedPointW2F6 minutes;
    HEMISPHERE hemisphere;
} AngularCoordinate;

typedef struct GpggaData_t
{
    bool isValid;
    GpsTime utcTime;
    AngularCoordinate latitude;
    AngularCoordinate longitude;
    fixedPointW5F1 altitudeMslMeters;
    GPS_FIX_TYPE fixType;
    fixedPointW2F0 numberOfSattelitesInUse;
} GpggaData;

typedef struct GpvtgData_t
{
    bool isValid;
    fixedPointW3F1 trueCourseDegrees;
    fixedPointW3F1 speedKph;
} GpvtgData;

typedef struct GpsData_t
{
    GpggaData gpggaData;
    GpvtgData gpvtgData;
} GpsData;

void resetGpsData(GpsData* pGpsData);

void parseNmeaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult);
void parseGpggaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult);
void parseGpvtgMessageIfValid(const NmeaMessage* pGpvtgMessage, GpsData* pResult);
