#pragma once

void TurnOnGreenLed(void);

#define ERROR_UART()
#define ERROR_NMEA_BUFFER_OVERFLOW()

#define ERROR_TEST_FAILED() \
    TurnOnGreenLed();
