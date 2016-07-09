#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum Errors_t
{
    ERR_NONE                 = 0x00,
    ERR_UART_COPERNICUS      = 0x01,
    ERR_FCS                  = 0x02,
    ERR_NMEA_BUFFER_OVERFLOW = 0x04,
    ERR_TIMER                = 0x08,
} Errors;

typedef enum ErrorsDetailsFcs_t
{
    ED_FCS_FAILED_TO_INITIALIZE   = 0x01,
    ED_FCS_FAILED_TO_DEINITIALIZE = 0x02,
} ErrorsDetailsFcs;

typedef enum ErrorsDetailsSignalsTimer_t
{
    ED_ST_FAILED_TO_INITIALIZE              = 0x01,
    ED_ST_FAILED_TO_CONFIGURE_CHANNEL_RED   = 0x02,
    ED_ST_FAILED_TO_CONFIGURE_CHANNEL_GREEN = 0x04,
    ED_ST_FAILED_TO_CONFIGURE_CHANNEL_BLUE  = 0x08,
    ED_ST_FAILED_TO_SIGNAL_GREEN            = 0x10,
    ED_ST_FAILED_TO_SIGNAL_RED              = 0x20,
    ED_ST_FAILED_TO_SIGNAL_BLUE             = 0x40,
} ErrorsDetailsTimer;

typedef enum ErrorsDetailsIsrFault_t
{
    ED_ISRF_HARD   = 0x01,
    ED_ISRF_MEMORY = 0x02,
    ED_ISRF_BUS    = 0x03,
    ED_ISRF_USAGE  = 0x04,
} ErrorsDetailsIsrFault;

typedef enum ErrorsDetailsUartCopernicus_t
{
    ED_UC_FAILED_TO_INITIALIZE          = 0x01,
    ED_UC_FAILED_TO_ENABLE_RECEIVE_DATA = 0x02,
    ED_UC_ERROR_WHILE_RECEIVING_DATA    = 0x04,
} ErrorsDetailsUartCopernicus;

typedef struct ErrorsDetails_t
{
    uint32_t isrFault;
    uint32_t fcs;
    uint32_t uartCopernicus;
    uint32_t timer;
} ErrorsDetails;

extern ErrorsDetails g_errorsDetails;

void initializeSignals(void);

void signalError(bool signal);
void signalInitialized(bool signal);
void signalTransmitting(bool signal);

uint8_t getErrorsMask();
void addError(uint8_t errorId, uint32_t* pErrorDetails, uint32_t errorDetailId);
void addErrorWithoutDetails(uint8_t errorId);
void resetErrors();

#define ERROR_ISR_FAULT(errorId) \
    addError(ERR_FCS, &g_errorsDetails.isrFault, errorId); \
    signalError(true);

#define ERROR_FCS(errorId) \
    addError(ERR_FCS, &g_errorsDetails.fcs, errorId); \
    signalError(true);

#define ERROR_SIGNA_TIMER(errorId) \
    addError(ERR_TIMER, &g_errorsDetails.timer, errorId); \
    signalError(true);

#define ERROR_UART_COPERNICUS(errorId) \
    addError(ERR_UART_COPERNICUS, &g_errorsDetails.uartCopernicus, errorId); \
    signalError(true);

#define ERROR_NMEA_BUFFER_OVERFLOW(errorId) \
    addErrorWithoutDetails(ERR_NMEA_BUFFER_OVERFLOW); \
    signalError(true);

#define ERROR_TRACE_UART() \
    signalError(true);
