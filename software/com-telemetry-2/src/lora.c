/*
 * lora.c - LoRA utilities
 */

#include <lora.h>
#include <main.h>
#include <periph.h>
#include "sx1276.h"

// IRQ flags pertaining to reception
#define RFLR_IRQFLAGS_RXSTATUS (RFLR_IRQFLAGS_RXTIMEOUT | RFLR_IRQFLAGS_RXDONE | \
	RFLR_IRQFLAGS_PAYLOADCRCERROR)

// Write and read data to the SX1276
static void sx1276WriteRead(void *output, const void *buffer, uint32_t count) {
	// NSS = 0
	loraSelect();
	spiWriteReadBytes(output, buffer, count);
	// NSS = 1
	loraDeselect();
}

// Reads one register from the SX1276
static uint8_t sx1276ReadRegister(uint8_t addr) {
	uint8_t buffer[2], output[2];
	buffer[0] = addr & 0x7FU;
	sx1276WriteRead(output, buffer, 2U);
	return output[1];
}

// Writes one register to the SX1276
static void sx1276WriteRegister(uint8_t addr, uint8_t value) {
	uint8_t buffer[2];
	buffer[0] = addr | RFLR_WRITE_REGISTER;
	buffer[1] = value;
	sx1276WriteRead(NULL, buffer, 2U);
}

// Clears the LoRA reception flag once data has been read
void loRAClearRX() {
	sx1276WriteRegister(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXSTATUS);
}

// Retrieves the current RSSI value in dBm
int32_t loRAGetRSSI() {
	return (int32_t)sx1276ReadRegister(REG_LR_RSSIVALUE) - 164;
}

// Reports the LoRA received data status
uint32_t loRAGetRXStatus() {
	return sx1276ReadRegister(REG_LR_IRQFLAGS) & RFLR_IRQFLAGS_RXSTATUS;
}

// Reports the LoRA transmitted data status
bool loRAGetTXStatus() {
	return (sx1276ReadRegister(REG_LR_IRQFLAGS) & RFLR_IRQFLAGS_TXDONE) ? true : false;
}

// Retrieves the current SNR value in dB
int32_t loRAGetSNR() {
	return (int32_t)(sx1276ReadRegister(REG_LR_PKTSNRVALUE) >> 2);
}

// Initializes the LoRA radio
void loRAInit() {
	// Initialize range settings in sleep mode
	sx1276WriteRegister(REG_LR_OPMODE, RFLR_OPMODE_SLEEP | RFLR_OPMODE_FREQMODE_ACCESS_LF);
	// Set LNA gain
	sx1276WriteRegister(REG_LR_LNA, RFLR_LNA_GAIN_G1 | RFLR_LNA_BOOST_HF_ON);
	// Set default OCP
	sx1276WriteRegister(REG_LR_OCP, RFLR_OCP_TRIM_100_MA);
	// Set symbol timeout
	sx1276WriteRegister(REG_LR_SYMBTIMEOUTLSB, 0xFFU);
	// Set preamble length
	sx1276WriteRegister(REG_LR_PREAMBLELSB, 0x0CU);
	// Set frequency
	loRASetFrequency(RF_FREQUENCY);
	// Enter LoRA mode
	sx1276WriteRegister(REG_LR_OPMODE, RFLR_OPMODE_SLEEP | RFLR_OPMODE_FREQMODE_ACCESS_LF |
		RFLR_OPMODE_LONGRANGEMODE_ON);
	// Modem configuration (NOT burst writable, they are not adjacent)
	sx1276WriteRegister(REG_LR_MODEMCONFIG1, RFLR_MODEMCONFIG1_BW_62_50_KHZ |
		RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF | RFLR_MODEMCONFIG1_CODINGRATE_4_5);
	sx1276WriteRegister(REG_LR_MODEMCONFIG2, RFLR_MODEMCONFIG2_SF_12 |
		RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON | 0x03U);
	sx1276WriteRegister(REG_LR_MODEMCONFIG3, RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON);
	// Enter standby
	loRAStandby();
	// All interrupts should be available
	sx1276WriteRegister(REG_LR_IRQFLAGSMASK, 0U);
	// Clear all interrupt flags
	sx1276WriteRegister(REG_LR_IRQFLAGS, 0xFFU);
}

// Reads up to maxLen bytes of the received packet into *data, returning the actual number of
// bytes read
uint32_t loRAReadPacket(void *data, const uint32_t maxLen) {
	uint32_t len = (uint32_t)sx1276ReadRegister(REG_LR_NBRXBYTES);
	uint8_t header[1];
	// Transfer address
	header[0] = REG_LR_FIFO;
	// Cap length if necessary
	if (len > maxLen)
		len = maxLen;
	// Manually fire burst read
	loraSelect();
	spiWriteReadBytes(NULL, header, sizeof(header));
	// Read data (up to maxLen)
	spiWriteReadBytes(data, NULL, len);
	// NSS = 1
	loraDeselect();
	return len;
}

// Changes the LoRA frequency
void loRASetFrequency(const uint32_t frequency) {
	uint8_t frf[4];
	// Calculate new frequency = frequency in MHz * 16384
	uint32_t lf = ((frequency / 100000U) << 14) / 10U;
	// Write all 3 registers, big endian!
	frf[0] = REG_LR_FRFMSB | RFLR_WRITE_REGISTER;
	frf[1] = (uint8_t)(lf >> 16);
	frf[2] = (uint8_t)(lf >> 8);
	frf[3] = (uint8_t)lf;
	sx1276WriteRead(NULL, frf, sizeof(frf));
}

// Sets the output power in dBm
void loRASetPower(uint32_t power) {
	if (power < 2U || power > 17U)
		power = 10U;
	// Set up output power
	sx1276WriteRegister(REG_LR_PACONFIG, 0xEEU + power);
}

// Stops receiving or transmitting data
void loRAStandby() {
	sx1276WriteRegister(REG_LR_OPMODE, RFLR_OPMODE_STANDBY | RFLR_OPMODE_FREQMODE_ACCESS_LF);
}

// Starts receiving data
void loRAStartReceive() {
	// Clear the FIFO
	sx1276WriteRegister(REG_LR_FIFORXBASEADDR, 0U);
	sx1276WriteRegister(REG_LR_FIFOADDRPTR, 0U);
	// Clear RX flags
	sx1276WriteRegister(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXSTATUS);
	// Enter receive mode
	sx1276WriteRegister(REG_LR_OPMODE, RFLR_OPMODE_RECEIVER | RFLR_OPMODE_FREQMODE_ACCESS_LF |
		RFLR_OPMODE_LONGRANGEMODE_ON);
}

// Transmits len bytes of data to the LoRA device; cannot be used while receiving
// power level is from 2 to 17
void loRAStartTransmit(const void *data, const uint32_t len) {
	const uint8_t reg = REG_LR_FIFO | RFLR_WRITE_REGISTER;
	// Clear the FIFO
	sx1276WriteRegister(REG_LR_FIFOTXBASEADDR, 0U);
	sx1276WriteRegister(REG_LR_FIFOADDRPTR, 0U);
	// Manually fire burst write
	loraSelect();
	spiWriteReadBytes(NULL, &reg, 1U);
	// Write data
	spiWriteReadBytes(NULL, data, len);
	// NSS = 1
	loraDeselect();
	// Set packet length
	sx1276WriteRegister(REG_LR_PAYLOADLENGTH, len);
	// Clear TX flag
	sx1276WriteRegister(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
	// Enter transmit mode
	sx1276WriteRegister(REG_LR_OPMODE, RFLR_OPMODE_TRANSMITTER | RFLR_OPMODE_FREQMODE_ACCESS_LF |
		RFLR_OPMODE_LONGRANGEMODE_ON);
}
