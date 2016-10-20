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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
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

#ifndef USB_CALLBACKS_H_
#define USB_CALLBACKS_H_

#include <stdbool.h>
#include "usb_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize USB peripheral
void usbInit(void);
// Returns true if USB is connected
bool usbIsConnected(void);

// USB callbacks

#ifdef USB_RESET
// Handle USB reset
void USB_RESET(void);
#endif
#ifdef USB_DATA_SETUP
// Set up a class specific request with data stage, fills ENDPOINT_INFO accordingly
// If IN tokens are expected, then wLength & wOffset will be filled with the total number of
// bytes to transfer and the starting position
// If OUT tokens are expected, then rLength & rOffset will be filled similarly
uint32_t USB_DATA_SETUP(uint8_t type, uint8_t request);
#endif
#ifdef USB_NO_DATA_SETUP
// Set up a class specific request without data stage
uint32_t USB_NO_DATA_SETUP(uint8_t type, uint8_t request);
#endif
#ifdef USB_CUSTOM_DESCRIPTOR
// Report a custom descriptor, length is in the return value, and buffer location is set in
// output argument (3rd); 0 length will NAK the request
uint16_t USB_CUSTOM_DESCRIPTOR(uint8_t desc, uint8_t target, uint8_t **buffer);
#endif
#ifdef USB_SET_CONFIGURATION
// Callback invoked when the configuration index of the USB device is changed
uint32_t USB_SET_CONFIGURATION(uint32_t index);
#endif
#ifdef USB_DEVICE_GET_STATUS
// Callback invoked to obtain USB device status bits; result will be ORed with remote wakeup
// and self-powered flags (0x02 and 0x01 respectively)
uint16_t USB_DEVICE_GET_STATUS();
#endif
#ifdef USB_INTF_GET_STATUS
// Callback invoked to obtain USB interface status bits
uint16_t USB_DEVICE_GET_STATUS(uint32_t interface);
#endif
#ifdef USB_EP_GET_STATUS
// Callback invoked to obtain USB endpoint status bits; result will be ORed with 0x01 stall flag
uint16_t USB_EP_GET_STATUS(uint32_t ep);
#endif
#ifdef USB_EP_CLEAR_FEATURE
// Clears an endpoint feature, with endpoint index and feature specified
uint32_t USB_EP_CLEAR_FEATURE(uint32_t ep, uint32_t feature);
#endif
#ifdef USB_EP_SET_FEATURE
// Set endpoint feature, with endpoint index and feature specified
uint32_t USB_EP_SET_FEATURE(uint32_t ep, uint32_t feature);
#endif
#ifdef USB_SETUP_OUT_COMPLETE
// Callback invoked after control data has been received on endpoint zero
void USB_SETUP_OUT_COMPLETE();
#endif
#ifdef USB_SETUP_IN_COMPLETE
// Callback invoked after control data has been sent on endpoint zero
void USB_SETUP_IN_COMPLETE();
#endif
#ifdef USB_SUSPEND
// Callback invoked after a suspend request has been received from USB
void USB_SUSPEND(void);
#endif
#ifdef USB_RESUME
// Callback invoked after a resume request has been received from USB
void USB_RESUME(void);
#endif
// Endpoint TX/RX functions declared in the config
#ifdef USB_EP1_RX
void USB_EP1_RX (void);
#endif
#ifdef USB_EP1_TX
void USB_EP1_TX (void);
#endif
#ifdef USB_EP2_RX
void USB_EP2_RX (void);
#endif
#ifdef USB_EP2_TX
void USB_EP2_TX (void);
#endif
#ifdef USB_EP3_RX
void USB_EP3_RX (void);
#endif
#ifdef USB_EP3_TX
void USB_EP3_TX (void);
#endif
#ifdef USB_EP4_RX
void USB_EP4_RX (void);
#endif
#ifdef USB_EP4_TX
void USB_EP4_TX (void);
#endif
#ifdef USB_EP5_RX
void USB_EP5_RX (void);
#endif
#ifdef USB_EP5_TX
void USB_EP5_TX (void);
#endif
#ifdef USB_EP6_RX
void USB_EP6_RX (void);
#endif
#ifdef USB_EP6_TX
void USB_EP6_TX (void);
#endif
#ifdef USB_EP7_RX
void USB_EP7_RX (void);
#endif
#ifdef USB_EP7_TX
void USB_EP7_TX (void);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
