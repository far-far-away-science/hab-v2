/*
 * lora.h - LoRA definitions and includes
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

// LoRA frequency
#define RF_FREQUENCY 434000000U

// Flags indicating RX status (bit mask)
#define RF_RXSTATUS_TIMEOUT 0x80
#define RF_RXSTATUS_OK 0x40
#define RF_RXSTATUS_ERR 0x20

// Commonly used for writing
#define RFLR_WRITE_REGISTER 0x80U

// Clears the LoRA reception flag once data has been read
void loRAClearRX();
// Retrieves the current RSSI value in dBm
int32_t loRAGetRSSI();
// Reports the LoRA received data status
uint32_t loRAGetRXStatus();
// Reports the LoRA transmitted data status
bool loRAGetTXStatus();
// Retrieves the current SNR value in dB
int32_t loRAGetSNR();
// Initializes the LoRA radio
void loRAInit();
// Reads up to maxLen bytes of the received packet into *data, returning the actual number of
// bytes read
uint32_t loRAReadPacket(void *data, const uint32_t maxLen);
// Changes the LoRA frequency
void loRASetFrequency(const uint32_t frequency);
// Sets the output power in dBm
void loRASetPower(uint32_t power);
// Stops receiving or transmitting data
void loRAStandby();
// Starts receiving data
void loRAStartReceive();
// Transmits len bytes of data to the LoRA device; cannot be used while receiving
// power level is from 2 to 17
void loRAStartTransmit(const void *data, const uint32_t len);
