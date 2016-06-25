#include "nmea_messages_impl.h"

#include <math.h>
#include <string.h>

void resetGpsData(GpsData* pGpsData)
{
    // TODO
}

void parseNmeaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult)
{
    if (memcmp(pGpggaMessage->message + 3, "GGA", 3) == 0)
    {
        // Global Positioning System fix
        parseGpggaMessageIfValid(pGpggaMessage, pResult);
    }
    else if (memcmp(pGpggaMessage->message + 3, "VTG", 3) == 0)
    {
        // Track made good and ground speed
        parseGpvtgMessageIfValid(pGpggaMessage, pResult);
    }
}

void parseGpggaMessageIfValid(const NmeaMessage* pGpggaMessage, GpsData* pResult)
{
    if (!pGpggaMessage || !pResult)
    {
        return;
    }

    GpggaData gpggaData;
    uint8_t checksum = 0;
    NmeaParsingContext nmeaParsingContext = { pGpggaMessage };

    // header
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // GPGGA header
    PARSE_GPS_TIME(nmeaParsingContext, gpggaData.utcTime);
    PARSE_LATITUDE(nmeaParsingContext, gpggaData.latitude);
    PARSE_LONGITUDE(nmeaParsingContext, gpggaData.longitude);
    PARSE_FIX_TYPE(nmeaParsingContext, gpggaData.fixType);
    PARSE_FIXED_POINT_W2F0(nmeaParsingContext, gpggaData.numberOfSattelitesInUse);
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // horizontal dilution of position
    PARSE_FIXED_POINT_W5F1(nmeaParsingContext, gpggaData.altitudeMslMeters);
    PARSE_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_LAST_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_CHECKSUM(nmeaParsingContext, checksum);

    pResult->gpggaData.isValid = !pGpggaMessage->hasError && isChecksumValid(checksum, pGpggaMessage->message, pGpggaMessage->size);
    pResult->gpggaData = gpggaData;
}

void parseGpvtgMessageIfValid(const NmeaMessage* pGpvtgMessage, GpsData* pResult)
{
    GpvtgData gpvtgData;
    uint8_t checksum = 0;
    NmeaParsingContext nmeaParsingContext = { pGpvtgMessage };

    PARSE_DUMMY_TOKEN(nmeaParsingContext); // GPVTG header
    PARSE_FIXED_POINT_W3F1(nmeaParsingContext, gpvtgData.trueCourseDegrees);
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // true track type
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // not used
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // not used
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // speed in knots
    PARSE_DUMMY_TOKEN(nmeaParsingContext); // speed in knots unit (fixed)
    PARSE_FIXED_POINT_W3F1(nmeaParsingContext, gpvtgData.speedKph);
    PARSE_LAST_DUMMY_TOKEN(nmeaParsingContext);
    PARSE_CHECKSUM(nmeaParsingContext, checksum);

    pResult->gpvtgData.isValid = pGpvtgMessage->hasError && isChecksumValid(checksum, pGpvtgMessage->message, pGpvtgMessage->size);
    pResult->gpvtgData = gpvtgData;
}
