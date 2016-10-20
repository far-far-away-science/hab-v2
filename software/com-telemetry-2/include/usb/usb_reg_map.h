/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2015 Stephen Carlson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#include <cortex.h>
#include "usb_config.h"

#ifndef USB_REG_MAP_H_
#define USB_REG_MAP_H_

// Flags that should not be changed if a non-toggle or NOP is desired
#define USB_EP_CTR_NOP (USB_EP_CTR_RX | USB_EP_CTR_TX)
#define USB_EP_NONTOGGLE (USB_EP_CTR_RX | USB_EP_SETUP | USB_EP_EP_TYPE_MASK | \
	USB_EP_EP_KIND | USB_EP_CTR_TX | USB_EP_EA)

// Conveniences for the peripheral memory area (PMA) and buffer table
#ifdef PMA_32BIT
#define usbPMA(offset) (&PMA[(offset) << 1])
#else
#define usbPMA(offset) (&PMA[offset])
#endif
#define usbBTABLE(offset) ((__IO uint16_t *)usbPMA(USB->BTABLE + (offset)))

// Endpoint conveniences
#define usbEP(epNum) (USB->EP[epNum])
#define usbNOP(epr, preserve, clear) (((epr) & (((preserve) | USB_EP_NONTOGGLE) & ~(clear))) \
	| USB_EP_CTR_NOP)

// Clear the correct-transfer RX flag
static INLINE uint32_t _usbClearCTR_RX(uint32_t epr) {
	return (epr | USB_EP_CTR_TX) & (USB_EP_NONTOGGLE & ~USB_EP_CTR_RX);
}
// Clear the correct-transfer TX flag
static INLINE uint32_t _usbClearCTR_TX(uint32_t epr) {
	return (epr | USB_EP_CTR_RX) & (USB_EP_NONTOGGLE & ~USB_EP_CTR_TX);
}

// Get the RX endpoint status
static INLINE uint32_t usbGetRXStatus(uint32_t ep) {
	return usbEP(ep) & USB_EP_STAT_RX_MASK;
}
#define _usbIsRXStall(epr) ((epr & USB_EP_STAT_RX_MASK) == USB_EP_STAT_RX_STALL)
// Set the RX endpoint status
static INLINE void usbSetRXStatus(uint32_t ep, uint32_t status) {
	usbEP(ep) = usbNOP(usbEP(ep), USB_EP_STAT_RX_MASK, 0U) ^ status;
}
// Get the TX endpoint status
static INLINE uint32_t usbGetTXStatus(uint32_t ep) {
	return usbEP(ep) & USB_EP_STAT_TX_MASK;
}
#define _usbIsTXStall(epr) ((epr & USB_EP_STAT_TX_MASK) == USB_EP_STAT_TX_STALL)
// Set the TX endpoint status
static INLINE void usbSetTXStatus(uint32_t ep, uint32_t status) {
	usbEP(ep) = usbNOP(usbEP(ep), USB_EP_STAT_TX_MASK, 0U) ^ status;
}
// Set both status, rxStat and txStat should be ORed together
static INLINE void usbSetRXTXStatus(uint32_t ep, uint32_t status) {
	usbEP(ep) = usbNOP(usbEP(ep), USB_EP_STAT_TX_MASK | USB_EP_STAT_RX_MASK, 0U) ^ status;
}

// Set the endpoint type
static INLINE void usbSetEPType(uint32_t ep, uint32_t type) {
	usbEP(ep) = usbNOP(usbEP(ep), 0U, USB_EP_EP_TYPE_MASK) | type;
}
// Set the endpoint kind
static INLINE void usbSetEPKind(uint32_t ep, uint32_t kind) {
	usbEP(ep) = usbNOP(usbEP(ep), 0U, USB_EP_EP_KIND) | kind;
}
// Set the endpoint address
static INLINE void usbSetEPAddress(uint32_t ep, uint32_t addr) {
	usbEP(ep) = usbNOP(usbEP(ep), 0U, USB_EP_EA) | addr;
}
// Configure an endpoint (all parameters), to be used in RESET only!
static INLINE void usbConfigureEndpoint(uint32_t ep, uint32_t value) {
	usbEP(ep) = usbNOP(usbEP(ep), USB_EP_STAT_TX_MASK | USB_EP_STAT_RX_MASK,
		USB_EP_EP_TYPE_MASK | USB_EP_EP_KIND | USB_EP_EA) | value;
}

// Clear the RX data toggle flag and sets the RX endpoint to valid
static INLINE uint32_t _usbClearDTOG_RX(uint32_t epr) {
	epr = usbNOP(epr, USB_EP_DTOG_RX | USB_EP_STAT_RX_MASK, 0U) ^ USB_EP_STAT_RX_VALID;
	if (epr & USB_EP_DTOG_RX)
		epr |= USB_EP_DTOG_RX;
	return epr;
}
// Clear the TX data toggle flag and sets the TX endpoint to valid
static INLINE uint32_t _usbClearDTOG_TX(uint32_t epr) {
	epr = usbNOP(epr, USB_EP_DTOG_TX | USB_EP_STAT_TX_MASK, 0U) ^ USB_EP_STAT_TX_VALID;
	if (epr & USB_EP_DTOG_TX)
		epr |= USB_EP_DTOG_TX;
	return epr;
}

// Accesses the TX address
#define _usbTXAddrPtr(ep) usbBTABLE((ep) << 3)
static INLINE uint16_t usbGetTXAddr(uint32_t ep) {
	return *_usbTXAddrPtr(ep);
}
static INLINE void usbSetTXAddr(uint32_t ep, uint16_t addr) {
	*_usbTXAddrPtr(ep) = addr & ~0x1;
}

// Accesses the RX address
#define _usbRXAddrPtr(ep) usbBTABLE(((ep) << 3) + 4)
static INLINE uint16_t usbGetRXAddr(uint32_t ep) {
	return *_usbRXAddrPtr(ep);
}
static INLINE void usbSetRXAddr(uint32_t ep, uint16_t addr) {
	*_usbRXAddrPtr(ep) = addr & ~0x1;
}

// Accesses the TX count (does not cover double-buffered and isochronous input types)
#define _usbTXCountPtr(ep) usbBTABLE(((ep) << 3) + 2)
static INLINE uint16_t usbGetTXCount(uint32_t ep) {
	return *_usbTXCountPtr(ep);
}
static INLINE void usbSetTXCount(uint32_t ep, uint16_t count) {
	*_usbTXCountPtr(ep) = count;
}

// Accesses the RX count (does not cover double-buffered and isochronous input types)
#define _usbRXCountPtr(ep) usbBTABLE(((ep) << 3) + 6)
static INLINE uint16_t usbGetRXCount(uint32_t ep) {
	return *_usbRXCountPtr(ep) & 0x3FF;
}
void usbSetRXCount(uint32_t ep, uint32_t count);

// PMA copy routines
void usbCopyToPMA(const void *buf, uint32_t len, uint32_t pma_offset);
void usbCopyFromPMA(void *buf, uint32_t len, uint32_t pma_offset);

#endif
