#pragma once

#include "test/test.h"
#include "aprs/aprs_board_impl.h"
#include "stm32l0xx_hal.h"

#ifdef TEST
    static GpsData g_gpsData;
    static uint8_t g_aprsPayloadBuffer[APRS_PAYLOAD_BUFFER_MAX_LENGTH];
#endif

TEST_CLASS(aprs_board_createGpsAprsPayload_perf,
{
    TEST_METHOD(createGpsAprsPayload_should_be_fast,
    {
        uint16_t trialsCount = 0;

        g_gpsData.gpggaData.isValid = true;
        g_gpsData.gpggaData.utcTime.isValid = true;
        memcpy(g_gpsData.gpggaData.utcTime.hours.string, "12", 2);
        memcpy(g_gpsData.gpggaData.utcTime.minutes.string, "37", 2);
        memcpy(g_gpsData.gpggaData.utcTime.seconds.string, "57.00", 5);
        g_gpsData.gpggaData.latitude.isValid = true;
        g_gpsData.gpggaData.latitude.hemisphere = H_NORTH;
        memcpy(g_gpsData.gpggaData.latitude.degrees.string, "48", 2);
        memcpy(g_gpsData.gpggaData.latitude.minutes.string, "07.380000", 9);
        g_gpsData.gpggaData.longitude.isValid = true;
        g_gpsData.gpggaData.longitude.hemisphere = H_WEST;
        memcpy(g_gpsData.gpggaData.longitude.degrees.string, "11", 2);
        memcpy(g_gpsData.gpggaData.longitude.minutes.string, "31.000000", 9);
        memcpy(g_gpsData.gpggaData.altitudeMslMeters.string, "00011.5", 7);
        g_gpsData.gpggaData.fixType = GPSFT_GPS;
        memcpy(g_gpsData.gpggaData.numberOfSattelitesInUse.string, "06", 2);
        memcpy(g_gpsData.gpvtgData.trueCourseDegrees.string, "011.0", 5);
        memcpy(g_gpsData.gpvtgData.speedKph.string, "001.0", 5);

        const uint32_t startTimeMilliseconds = HAL_GetTick();

        for (; trialsCount < 1000; ++trialsCount)
        {
            createGpsAprsPayload(&g_gpsData, g_aprsPayloadBuffer, APRS_PAYLOAD_BUFFER_MAX_LENGTH);
        }

        const uint32_t endTimeMilliseconds = HAL_GetTick();
        const float actualRuntimeMilliseconds = (endTimeMilliseconds - startTimeMilliseconds) / (float) trialsCount;

        // the following data is for 16MHz
#ifndef DEBUG
        ASSERT_IS_TRUE(actualRuntimeMilliseconds <= 0.05f);
#else
        ASSERT_IS_TRUE(actualRuntimeMilliseconds <= 0.072f);
#endif
    })
})
