/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2015 Stephen Carlson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

#include <cortex.h>
#include "usb_core.h"

// USB descriptor types
#define USB_DESCRIPTOR_TYPE_DEVICE 0x01
#define USB_DESCRIPTOR_TYPE_CONFIGURATION 0x02
#define USB_DESCRIPTOR_TYPE_STRING 0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE 0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT 0x05

// Endpoint types
#define EP_TYPE_INTERRUPT 0x03
#define EP_TYPE_BULK 0x02
#define EP_TYPE_ISOCHRONOUS 0x01
#define EP_TYPE_CONTROL 0x00

// Input and output endpoints
#define USB_DESCRIPTOR_ENDPOINT_IN USB_EP_IN
#define USB_DESCRIPTOR_ENDPOINT_OUT 0x00

// 2 bytes (wide chars) per character + size byte + type byte
#define USB_DESCRIPTOR_STRING_LEN(x) (2 + ((x) << 1))

#if defined(__cplusplus)
extern "C" {
#endif

// String descriptor structure
#define USB_DESCRIPTOR_STRING(len) struct {\
	uint8_t bLength;\
	uint8_t bDescriptorType;\
	uint16_t bString[len];\
} PACKED

// CDC functional descriptor structure
#define CDC_FUNCTIONAL_DESCRIPTOR_SIZE(size) (3 + (size))
#define CDC_FUNCTIONAL_DESCRIPTOR(size)	struct {\
	uint8_t bLength;\
	uint8_t bDescriptorType;\
	uint8_t SubType;\
	uint8_t Data[size];\
} PACKED

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} PACKED USBDescriptorDevice;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} PACKED USBDescriptorConfigHeader;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} PACKED USBDescriptorInterface;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} PACKED USBDescriptorEndpoint;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bString[];
} USBDescriptorString;

// Prototype device descriptors available in descriptors.c
extern const USBDescriptorPtr usbDeviceDescriptor;
extern const USBDescriptorPtr usbConfigDescriptor;
extern const USBDescriptorPtr usbStringDescriptor[USB_NUM_STRING_DESC];

#if defined(__cplusplus)
}
#endif

#endif
