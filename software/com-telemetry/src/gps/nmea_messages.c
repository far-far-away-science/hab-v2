#include "nmea_messages_impl.h"

#include <math.h>

int32_t angularCoordinateToInt32Degrees(AngularCoordinate coord)
{
    if (coord.isValid)
    {
        return (int32_t) (1000000.0f * (coord.degrees + (float) coord.minutes / (1000000.0f * 60.0f)));
    }
    else
    {
        return 0;
    }
}

void parseNmeaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult)
{
    // TODO
}

void parseGpggaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult)
{
    if (!pGpggaMessage || !pResult)
    {
        return;
    }

    uint8_t gpsQuality;
    GpggaData gpggaData;
    NmeaParsingContext parsingContext = { pGpggaMessage, 0 };

    // header
    PARSE_DUMMY_TOKEN(parsingContext); // GPGGA header
    PARSE_GPS_TIME(parsingContext, gpggaData.utcTime);
    PARSE_LATITUDE(parsingContext, gpggaData.latitude);
    PARSE_LONGITUDE(parsingContext, gpggaData.longitude);
    PARSE_UINT8_DEFAULT_TO_0(parsingContext, gpsQuality);
    PARSE_UINT8_DEFAULT_TO_0(parsingContext, gpggaData.numberOfSattelitesInUse);
    PARSE_DUMMY_TOKEN(parsingContext); // horizontal dilution of position
    PARSE_FIXED_POINT_UINT32_F1_DEFAULT_TO_0(parsingContext, gpggaData.altitudeMslMeters);
    // rest of the fields are ignored

    gpggaData.fixType = (GPS_FIX_TYPE) gpsQuality;

    if ((gpggaData.fixType == GPSFT_GPS || gpggaData.fixType == GPSFT_DGPS || gpggaData.fixType == GPSFT_MANUAL_INPUT_MODE) &&
        gpggaData.latitude.isValid && gpggaData.longitude.isValid)
    {
        pResult->gpggaData = gpggaData;
        pResult->isValid = true;
    }
}

void parseGpvtgMessageIfValid(const NmeaMessage* pGpvtgMessage, GpsData* pResult)
{
    GpvtgData gpvtgData;
    NmeaParsingContext nmeaParsingContext = { pGpvtgMessage, 0 };

    PARSE_DUMMY_TOKEN(nmeaParsingContext); // GPVTG header
    PARSE_FIXED_POINT_UINT16_F1_DEFAULT_TO_0(nmeaParsingContext, gpvtgData.trueCourseDegrees);
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // true track type
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // not used
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // not used
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // speed in knots
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // speed in knots unit (fixed)
    PARSE_FIXED_POINT_UINT16_F1_DEFAULT_TO_0(nmeaParsingContext, gpvtgData.speedKph);
    // rest of the fields are ignored

    pResult->gpvtgData = gpvtgData;
}
