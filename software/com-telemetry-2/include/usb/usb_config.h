/*
 * usb_config.h - Configures the USB core as required for the application
 */

#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Configure USB core for the peripheral in use

// VID and PID used by this device
#define USB_VID 0x1EAF
#define USB_PID 0x0010

// Define these to the endpoint function names
#undef USB_EP1_RX
#define USB_EP1_TX usbVcomTX
#undef USB_EP2_RX
#undef USB_EP2_TX
#define USB_EP3_RX usbVcomRX
#undef USB_EP3_TX
#undef USB_EP4_RX
#undef USB_EP4_TX
#undef USB_EP5_RX
#undef USB_EP5_TX
#undef USB_EP6_RX
#undef USB_EP6_TX
#undef USB_EP7_RX
#undef USB_EP7_TX
#undef USB_EP8_RX
#undef USB_EP8_TX

// If user software defines callbacks for specific functionality, include them here
// USB reset
#define USB_RESET usbReset
// Custom setup request with data
#define USB_DATA_SETUP usbDataSetup
// Custom setup request without data
#define USB_NO_DATA_SETUP usbNoDataSetup
// Custom descriptor request
#undef USB_CUSTOM_DESCRIPTOR
// Configuration index set by host
#undef USB_SET_CONFIGURATION
// Interface and alternate setting index set by host
#undef USB_SET_INTERFACE
// Retrieve status of device
#undef USB_DEVICE_GET_STATUS
// Retrieve status of interface
#undef USB_INTF_GET_STATUS
// Retrieve status of endpoint
#undef USB_EP_GET_STATUS
// Clear endpoint feature
#undef USB_EP_CLEAR_FEATURE
// Set endpoint feature
#undef USB_EP_SET_FEATURE
// Called after a data setup OUT transfer is completed (into the STM32)
#undef USB_SETUP_OUT_COMPLETE
// Called after a data setup IN transfer is completed (from the STM32)
#undef USB_SETUP_IN_COMPLETE
// Suspend callback
#undef USB_SUSPEND
// Resume callback
#undef USB_RESUME

// Unique ID number configuration; if neither is defined, no ID is presented (smallest!)
// If either is defined, make sure to update USB_NUM_STRING_DESC appropriately to have space
// for it in slot 0x03

// Present static ID number set in usb_descriptors.c
#define USB_ID_STATIC
// Present ID number of STM32 chip (larger, but unique per device!), slot 0x03 can be NULL
#undef USB_ID_DYNAMIC

// STM32 series configuration
// Undefine for L0 (CM0+), define for F1, L1 (CM3)
#undef PMA_32BIT
// ISR_USB for L0, ISR_USB_LP_CAN1_RX0 for F1, USB_LP_IRQHandler for L1
#define USB_IRQ_NAME ISR_USB
// 0x1FF80050 for L0 and L1 medium density, 0x1FFFF7E8 for F1, 0x1FF800D0 for L1 high density
#define USB_UNIQUE_ADDR ((volatile uint8_t *)0x1FF80050)

// Endpoint configuration

// # of endpoints implemented (including endpoint zero)
#define USB_NUM_EP 4
// # of configurations implemented
#define USB_NUM_CONFIG 1
// # of interfaces implemented
#define USB_NUM_INTF 1
// # of alternate configurations implemented
#define USB_NUM_ALT 1
// Maximum packet size (0x40 for USB FS)
#define USB_MAX_PACKET 0x40
// # of string descriptors
#define USB_NUM_STRING_DESC 4

// Standard control endpoint

// Endpoint index (always zero)
#define USB_CTRL_ENDP 0
// RX address (0x00 to 0x1F is used by the BTABLE)
#define USB_CTRL_RX_ADDR 0x20
// TX address
#define USB_CTRL_TX_ADDR 0x40
// Endpoint size
#define USB_CTRL_EPSIZE 0x20

// Auto-set by directive above
#ifdef PMA_32BIT
typedef uint32_t pma_t;
#else
typedef uint16_t pma_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
