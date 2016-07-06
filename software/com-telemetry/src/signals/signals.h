#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum Errors_t
{
    ERR_UART                 = 0x01,
    ERR_FCS                  = 0x02,
    ERR_NMEA_BUFFER_OVERFLOW = 0x04,
    ERR_TIMER                = 0x08,
} Errors;

void initializeSignals(void);

void signalError(bool signal);
void signalInitialized(bool signal);
void signalTransmitting(bool signal);

uint8_t getErrorsMask();
void addError(uint8_t error);
void resetErrors();

#define ERROR_FCS() \
    addError(ERR_FCS); \
    signalError(true);

#define ERROR_UART() \
    addError(ERR_UART); \
    signalError(true);

#define ERROR_NMEA_BUFFER_OVERFLOW() \
    addError(ERR_NMEA_BUFFER_OVERFLOW); \
    signalError(true);

#define ERROR_TIMER() \
    addError(ERR_TIMER); \
    signalError(true);

#define ERROR_TEST_FAILED() \
    signalError(true);
