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

#ifndef USB_CORE_H_
#define USB_CORE_H_

#include <stdint.h>
#include "usb_config.h"
#include "usb_reg_map.h"

#if defined(__cplusplus)
extern "C" {
#endif

// Power consumption attributes
#define USB_CONFIG_ATTR_BUSPOWERED 0x80
#define USB_CONFIG_ATTR_SELF_POWERED 0x40
// Remote wakeup flag
#define USB_CONFIG_ATTR_REMOTE_WKUP 0x20
// Flag bit for sending transfers
#define USB_FLAG_MULTIPLE 0x01
// Flag bit for if USB is connected at all
#define USB_FLAG_CONNECTED 0x02

// Descriptor types that can be requested by the host
#define DEVICE_DESCRIPTOR 1
#define CONFIG_DESCRIPTOR 2
#define STRING_DESCRIPTOR 3
#define INTERFACE_DESCRIPTOR 4
#define ENDPOINT_DESCRIPTOR 5

// Possible endpoint/device features that can be set or cleared
#define ENDPOINT_STALL 0
#define DEVICE_REMOTE_WAKEUP 1

// Mask for the request type
#define REQUEST_TYPE_MASK 0x60
// Mask to get the recipient of the request
#define RECIPIENT_MASK 0x1F
// Everything but the direction
#define RECPT_TYPE_MASK (REQUEST_TYPE_MASK | RECIPIENT_MASK)
// Standard request
#define STANDARD_REQUEST 0x00
// Class request
#define CLASS_REQUEST 0x20
// Vendor request
#define VENDOR_REQUEST 0x40
// Possible legal recipients for a request
#define DEVICE_RECIPIENT 0
#define INTERFACE_RECIPIENT 1
#define ENDPOINT_RECIPIENT 2
#define OTHER_RECIPIENT 3

// Handle CTRM, WKUPM, SUSPM, RESETM
#define USB_ISR_MSK 0x9C00

// Possible values for the USB control endpoint (#0) state machine
#define WAIT_SETUP 0
#define SETTING_UP 1
#define IN_DATA 2
#define OUT_DATA 3
#define LAST_IN_DATA 4
#define LAST_OUT_DATA 5
#define WAIT_STATUS_IN 6
#define WAIT_STATUS_OUT 7
#define STALLED 8
#define PAUSE 9

// Standard USB requests
#define GET_STATUS 0
#define CLEAR_FEATURE 1
#define SET_FEATURE 3
#define SET_ADDRESS 5
#define GET_DESCRIPTOR 6
#define SET_DESCRIPTOR 7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE 10
#define SET_INTERFACE 11

// USB return codes
#define USB_SUCCESS 0
#define USB_ERROR 1
#define USB_UNSUPPORT 2
#define USB_NOT_READY 3

// Pointer holding the vital information about a USB descriptor
typedef struct {
	uint8_t *descriptor;
	uint16_t descriptorSize;
} USBDescriptorPtr;

typedef struct {
	// Bitmask for USB request type
	uint8_t bmRequestType;
	// Byte value indicating request code
	uint8_t bRequest;
	// Word value indicating request value
	uint16_t wValue;
	// Word value indicating index/count associated with request
	uint16_t wIndex;
	// Word length value if request needs data
	uint16_t wLength;

	// Flags for various USB device features (like remote wakeup)
	uint8_t featureFlags;
	uint8_t reserved;
	// Index of currently selected configuration, interface, and alternate setting
	uint8_t configIndex;
	uint8_t interfaceIndex;
	uint8_t alternateIndex;
	// USB state machine active state
	uint8_t devState;
	// Word storage location for returning status to the USB host
	uint16_t statusInfo;

	// Number of data bytes remaining to be sent or received
	uint16_t dataLength;
	// Offset into the data
	uint16_t dataOffset;
	// Location of the buffer where data is coming from
	uint8_t *buffer;
} USBDeviceState;
// USB device state
extern volatile USBDeviceState usbDevState;

// Shortcut for device configured
#define IS_USB_CONFIG() (usbDevState.configIndex > 0U)

#if defined(__cplusplus)
}
#endif

#endif
