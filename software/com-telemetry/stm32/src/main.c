#include <hx1/hx1.h>

#include <aprs/aprs.h>

#include <test/test.h>
#include <test/trace_uart.h>

#include <gps/nmea_buffer.h>

#include <signals/signals.h>

#include <st_link/st_link.h>

#include <telemetry/telemetry.h>

#include <copernicus/copernicus.h>

#include <system_clock/system_clock.h>

#include <stm32l0xx_hal.h>

Telemetry g_telemetry = { 0 };
NmeaMessage g_nmeaMessage = { 0 };

int main(void)
{
#ifdef TEST
    HAL_Init();
    configSystemClock();
    stLinkInit();
    traceUartInit();

    EXECUTE_TESTS();

    traceUartDeInit();

    for (;;)
    {
    }
#else
    HAL_Init();
    configSystemClock();
    initializeSignals();
    stLinkInit();
    copernicusUartInit();

    signalInitialized(true);

    bool hasGpsMessage = false;

    for (;;)
    {
        // TODO distinguish between modes:
        // - ascending close to the ground
        // - ascending close to balloon popping event
        // - descending close to the ground
        // - landed not moving
        // - landed moving (water landing or on the truck :) )

        if (nmeaReadMessage(&g_nmeaRingBuffer, &g_nmeaMessage))
        {
            hasGpsMessage = true;
        }

        if (!isAprsMessageTransmitting())
        {
            // TODO add delay between APRS messages
            // TODO send APRS telemetry message
            if (hasGpsMessage)
            {
                if (encodeNmeaAprsMessage(&CALLSIGN_SOURCE, &g_nmeaMessage, &g_ax25EncodedAprsMessage))
                {
                    transmitAprsMessage();
                }
            }
        }
    }
#endif
}
