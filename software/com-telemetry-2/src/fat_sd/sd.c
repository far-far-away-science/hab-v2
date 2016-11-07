/*
 * sd.c - SD card control functions
 *
 * Parts of this code was taken from:
 *
 * Copyright (c) 2010, Martin Thomas, ChaN
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <periph.h>
#include "main.h"
#include "stmtime.h"
#include "ffconf.h"
#include "diskio.h"

#ifdef HABV2
// Definitions for MMC/SDC commands
// GO_IDLE_STATE
#define GO_IDLE_STATE (0x40+0)
// SEND_OP_COND (MMC)
#define SEND_OP_COND_MMC (0x40+1)
// SEND_OP_COND (SDC)
#define SEND_OP_COND_SDC (0xC0+41)
// SEND_IF_COND
#define SEND_IF_COND (0x40+8)
// SEND_CSD
#define SEND_CSD (0x40+9)
// SEND_CID
#define SEND_CID (0x40+10)
// STOP_TRANSMISSION
#define STOP_TRANSMISSION (0x40+12)
// SD_STATUS (SDC)
#define SD_STATUS (0xC0+13)
// SET_BLOCKLEN
#define SET_BLOCKLEN (0x40+16)
// READ_SINGLE_BLOCK
#define READ_SINGLE_BLOCK (0x40+17)
// READ_MULTIPLE_BLOCK
#define READ_MULTIPLE_BLOCK (0x40+18)
// SET_BLOCK_COUNT (MMC)
#define CMD23 (0x40+23)
// SET_WR_BLOCK_ERASE_COUNT (SDC)
#define ACMD23 (0xC0+23)
// WRITE_BLOCK
#define WRITE_BLOCK (0x40+24)
// WRITE_MULTIPLE_BLOCK
#define WRITE_MULTIPLE_BLOCK (0x40+25)
// APP_CMD
#define APP_CMD (0x40+55)
// READ_OCR
#define READ_OCR (0x40+58)

// SD/MMC card type detected
// MMC standard capacity
#define CT_MMC 0x01U
// SD standard capacity
#define CT_SD1 0x02U
// SD high capacity
#define CT_SD2 0x04U
// SD high capacity size over 2GB
#define CT_BLOCK 0x08U
// SD block size (fixed to 512 bytes according to spec)
#define SD_BLOCK 512U

// Commands to select and deselect the card
#define SPI_RECV() spiWriteRead(0xFF)
#define SPI_SELECT() spiSelect(SPI_DEVICE_SD)
#define SPI_DESELECT() spiSelect(SPI_DEVICE_NONE)

// SD timeout in systick (10ms) units
// Initial timeout
#define SD_TIMEOUT 50U
// Timeout for read/write operation to start
#define SD_READ_TIMEOUT 5U

// SD card status
static uint8_t sdStatus;

// Waits for the SD card to be ready
static bool sdWaitReady(void) {
	uint8_t res;
	const uint32_t then = sysTime;
	// Initial 8 clocks
	SPI_RECV();
	// Send clocks until SD card returns idle high
	do {
		res = SPI_RECV();
		if (res == 0xFFU)
			// SD card holds DO low when not ready
			return true;
	} while (sysTime - then < SD_TIMEOUT);
	return false;
}

// Sends a command to the SD card
static uint8_t sdCommand(uint8_t cmd, uint32_t arg) {
	uint8_t res = 0U, buffer[6];
	uint32_t i = 10U;
	if (cmd & 0x80U) {
		// ACMD<n> means send CMD 55, then CMD<n>
		cmd &= 0x7FU;
		res = sdCommand(APP_CMD, 0U);
	}
	if (res <= 1U) {
		SPI_DESELECT();
		// Select the card and wait for ready
		SPI_SELECT();
		if (sdWaitReady()) {
			// Send command packet, unfortunately arg is in big endian
			buffer[0] = cmd;
			buffer[1] = (uint8_t)(arg >> 24);
			buffer[2] = (uint8_t)(arg >> 16);
			buffer[3] = (uint8_t)(arg >> 8);
			buffer[4] = (uint8_t)arg;
			// Fake the CRC
			if (cmd == GO_IDLE_STATE)
				// Valid for GO_IDLE_STATE = 0
				buffer[5] = 0x95U;
			else if (cmd == SEND_IF_COND)
				// Valid for SEND_IF_COND = 0x1AA
				buffer[5] = 0x87U;
			else
				// Dummy + Stop
				buffer[5] = 0x01U;
			spiWriteBytes(buffer, 6);
			// Skip padding byte on stop
			if (cmd == STOP_TRANSMISSION)
				SPI_RECV();
			// 10 attempts for valid response
			do {
				res = SPI_RECV();
			} while ((res & 0x80U) != 0U && --i);
		} else
			// Timed out
			res = 0xFFU;
	}
	// Return response
	return res;
}

// Returns the current status of the SD card
DSTATUS disk_status(BYTE disk) {
	return (sdStatus != 0U) ? 0 : STA_NOINIT;
}

// Receives the specified number of data bytes from the SD card
static bool sdReceive(uint8_t *buffer, uint32_t count) {
	uint8_t token;
	uint16_t ignore = 0x0000;
	// Set timeout
	const uint32_t then = sysTime;
	sysTickEnable();
	{
		// Wait for a valid data token
		do {
			token = SPI_RECV();
		} while (token == 0xFF && sysTime - then < SD_READ_TIMEOUT);
	}
	sysTickDisable();
	// Token must be 0xFE
	if (token != 0xFEU)
		return false;
	// Send a load of 0xFF for data
	spiWriteReadBytes(buffer, NULL, count);
	// Discard CRC
	spiWriteReadBytes((uint8_t *)&ignore, NULL, 2);
	return true;
}

// Sends the specified data bytes to the SD card
static bool sdSend(const uint8_t *buffer, uint32_t count) {
	uint32_t crc = 0U;
	uint8_t response;
	// Token must be 0xFE
	spiWrite(0xFEU);
	// Send data
	spiWriteReadBytes(NULL, buffer, count);
	// Any CRC is acceptable
	spiWriteReadBytes(NULL, &crc, 2);
	// Read the data response
	spiWriteReadBytes(&response, NULL, 1);
	if ((response & 0x1FU) == 0x05U) {
		sdWaitReady();
		return true;
	}
	return false;
}

// Initialize the SD card
DSTATUS disk_initialize(BYTE disk) {
	uint8_t ocr[4];
	uint32_t type = 0U;
	const uint32_t then = sysTime;
	// Send 80 dummy clocks with data held high
	spiWriteBytes(NULL, 10U);
	sysTickEnable();
	{
		if (sdCommand(GO_IDLE_STATE, 0) == 1U) {
			// Enter idle state
			if (sdCommand(SEND_IF_COND, 0x1AAU) == 1U) {
				// SDHC, larger cards can use block addressing
				spiWriteReadBytes(ocr, NULL, 4);
				// Can card work at VDD range of 2.7-3.6V?
				if (ocr[2] == 0x01U && ocr[3] == 0xAAU) {
					// Wait until departure from idle state (ACMD41 with HCS bit)
					while (sysTime - then < SD_TIMEOUT && sdCommand(SEND_OP_COND_SDC, 1UL <<
						30) != 0U);
					if (sdCommand(READ_OCR, 0) == 0U) {
						// Check CCS bit in the OCR to see if block addressing should be used
						spiWriteReadBytes(ocr, NULL, 4);
						type = (ocr[0] & 0x40) ? (CT_SD2 | CT_BLOCK) : CT_SD2;
					}
				}
			} else {
				// SD or MMC
				uint8_t cmd;
				if (sdCommand(SEND_OP_COND_SDC, 0) <= 1U) {
					type = CT_SD1;
					cmd = SEND_OP_COND_SDC;
				} else {
					type = CT_MMC;
					cmd = SEND_OP_COND_MMC;
				}
				// Wait until departure from idle state
				while (sysTime - then < SD_TIMEOUT && sdCommand(cmd, 0U) != 0U);
				// Set block length to 512
				if (sdCommand(SET_BLOCKLEN, 512) != 0U)
					type = 0;
			}
		}
		SPI_DESELECT();
		SPI_RECV();
	}
	sysTickDisable();
	// If type == 0, then the card was not found
	sdStatus = type;
	return (type != 0U) ? 0 : STA_NOINIT;
}

// Reads the specified number of 512 byte blocks into the buffer, starting at the given sector
DRESULT disk_read(BYTE disk, BYTE *buff, DWORD sector, BYTE count) {
	// Convert to byte address if necessary
	if (!(sdStatus & CT_BLOCK)) sector *= SD_BLOCK;
	sysTickEnable();
	{
		if (count == 1U) {
			// Read one block
			if (sdCommand(READ_SINGLE_BLOCK, sector) == 0U) {
				if (sdReceive(buff, SD_BLOCK))
					count = 0U;
			}
		} else {
			// Read more than one block
			if (sdCommand(READ_MULTIPLE_BLOCK, sector) == 0U) {
				for (; sdReceive(buff, SD_BLOCK) && count--; buff += SD_BLOCK);
				// Stop the transmission
				sdCommand(STOP_TRANSMISSION, 0);
			}
		}
		// Release bus
		SPI_DESELECT();
		SPI_RECV();
	}
	sysTickDisable();
	return (count == 0) ? RES_OK : RES_ERROR;
}

// Writes the specified number of 512 byte blocks to disk, starting at the given sector
DRESULT disk_write(BYTE disk, const BYTE *buff, DWORD sector, BYTE count) {
	// Convert to byte address if necessary
	if (!(sdStatus & CT_BLOCK)) sector *= SD_BLOCK;
	sysTickEnable();
	{
		if (count == 1U) {
			// Write one block
			if (sdCommand(WRITE_BLOCK, sector) == 0U) {
				if (sdSend(buff, SD_BLOCK))
					count = 0U;
			}
		} else {
			// Write more than one block
			if (sdCommand(WRITE_MULTIPLE_BLOCK, sector) == 0U) {
				for (; sdSend(buff, SD_BLOCK) && count--; buff += SD_BLOCK);
				// Stop the transmission
				sdCommand(STOP_TRANSMISSION, 0);
				// Busy flag to purge the write
				sdWaitReady();
			}
		}
		// Release bus
		SPI_DESELECT();
		SPI_RECV();
	}
	sysTickDisable();
	return (count == 0) ? RES_OK : RES_ERROR;
}

// Disables the SD card
void sdShutdown(void) {
	// Go into idle
	SPI_DESELECT();
	SPI_RECV();
	sdStatus = 0U;
}
#endif
