#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum Errors_t
{
    ERR_NONE                 = 0x00,
    ERR_UART_COPERNICUS      = 0x01,
    ERR_CRC                  = 0x02,
    ERR_NMEA_BUFFER_OVERFLOW = 0x04,
    ERR_SIGNAL_TIMER         = 0x08,
    ERR_HX1                  = 0x10,
    ERR_ISR                  = 0x20,
} Errors;

typedef enum ErrorsDetailsCrc_t
{
    ED_CRC_FAILED_TO_INITIALIZE   = 0x01,
    ED_CRC_FAILED_TO_DEINITIALIZE = 0x02,
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

typedef enum ErrorsDetailsHx1_t
{
    ED_HX1_FAILED_TO_INITIALIZE_DAC        = 0x01,
    ED_HX1_FAILED_TO_CONFIGURE_DAC_CHANNEL = 0x02,
    ED_HX1_FAILED_TO_START_DAC_DMA         = 0x04,
} ErrorsDetailsHx1;

typedef struct ErrorsDetails_t
{
    uint32_t isrFault;
    uint32_t fcs;
    uint32_t uartCopernicus;
    uint32_t timer;
    uint32_t hx1;
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
    addError(ERR_ISR, &g_errorsDetails.isrFault, errorId); \
    signalError(true);

#define ERROR_CRC(errorId) \
    addError(ERR_CRC, &g_errorsDetails.fcs, errorId); \
    signalError(true);

#define ERROR_SIGNAL_TIMER(errorId) \
    addError(ERR_SIGNAL_TIMER, &g_errorsDetails.timer, errorId); \
    signalError(true);

#define ERROR_UART_COPERNICUS(errorId) \
    addError(ERR_UART_COPERNICUS, &g_errorsDetails.uartCopernicus, errorId); \
    signalError(true);

#define ERROR_HX1(errorId) \
    addError(ERR_HX1, &g_errorsDetails.hx1, errorId); \
    signalError(true);

#define ERROR_NMEA_BUFFER_OVERFLOW(errorId) \
    addErrorWithoutDetails(ERR_NMEA_BUFFER_OVERFLOW); \
    signalError(true);

#define ERROR_TRACE_UART() \
    signalError(true);
