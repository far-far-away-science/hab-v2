/*
 * main.h - Main program definitions
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <cortex.h>
#include <sys/types.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// GENERAL SETTINGS

// Define one of HAB v2 or Phoenix
#undef HAB_V2
#define PHOENIX
// If defined, enables USART2 on PA2/PA3 for Nucleo debugging
#undef DEBUG_UART
// If defined, enables data logging in packets.c from USART1 (default) or USART2 (debug enabled)
#define ENABLE_LOG
// If defined, disables radio transmission but leaves the rest of the program intact
#undef DISABLE_RADIO
// How many seconds from startup until PI is shut down automatically (6 hours)
#define PI_SHUTDOWN 21600
// Main power voltage in tenths of a volt (same units as transmitted) for good battery
#define POWER_OK 90
// Main power voltage in tenths of a volt for low battery
#define POWER_LOW 80
// If no position updates are received from the primary GPS after this many seconds, switch to
// secondary GPS
#define GPS_TIMEOUT 30

// OSCILLATOR SETTINGS

// Enable these flags to adjust oscillator speeds
// HS32 sets core clock to 32 MHz (HSI * 2, HSE * 4) and 1.8 V, otherwise uses 16 MHz and 1.5 V
#undef HS32
// If true, attempts to start the external 8 MHz crystal before falling back to the HSI
#define HSE
// LSE enables the 32.768 KHz crystal and RTC
#define LSE
// LSI uses the internal low-speed oscillator if the LSE fails to start; LSE must also be
// defined to turn on the RTC
#define LSI
// LSE compensation in parts per 2^20 (approximately ppm), positive speeds it up, negative
// slows it down
#define LSE_COMP 0

// RADIO SETTINGS

// Frequency used to configure the DAC in Hz
#define AUDIO_FREQ 128000
// Interval between APRS sends in seconds, must be at least two
#define APRS_INTERVAL 24U

// GPS SETTINGS

// Baud rate for the low-power UART on GPS1
#define BAUD_GPS1 9600
// Enables or disables the EM406 GPS (GPS2)
#define DUAL_GPS
// Baud rate for the UART on GPS2
#define BAUD_GPS2 4800
// Baud rate for the UART to the Raspberry PI (if DUAL_GPS is set, then BAUD_GPS2 will be used!)
#define BAUD_PI 115200

// 1-WIRE TEMPERATURE SENSOR SETTINGS

// If defined, uses 12-bit DS18B20. Otherwise, uses 9-bit DS1820B
#define DS18B20
// Enables parasite power mode for the temperature sensors, where the STM32 hard drives the
// line high during conversions
#undef PARASITE_POWER
// If defined, dumps the ID of the only DS1820B/DS18B20 to the serial port on receive
// Cannot be used if multiple temperature sensors are connected
#undef DUMP_ID
// If defined, ignores the saved IDs and assumes only one temperature sensor using all-call
#undef SINGLE_SENSOR

// END OF SETTINGS

// Pin definitions
// DAC
#define PIN_DAC GPIOA, 4
// SPI has PB3 (SCK), PB4 (MISO), PB5 (MOSI), and PA15 (!CS!)
#define PIN_SCK GPIOB, 3
#define PIN_MISO GPIOB, 4
#define PIN_MOSI GPIOB, 5
// Chip selects are a bit of a pain since HABv2 had to use a latch for them
#ifdef PHOENIX
#define PIN_LORA_CS GPIOA, 15

// Simple select and deselect LoRA only
#define loraSelect() ioSetOutput(PIN_LORA_CS, 0)
#define loraDeselect() ioSetOutput(PIN_LORA_CS, 1)
#else
#define PIN_LATCH_CS GPIOA, 15
#define PIN_LATCH_A1 GPIOB, 6
#define PIN_LATCH_A0 GPIOB, 7
#define PIN_LATCH_A2 GPIOB, 12

// Selects an SPI device
void spiSelect(uint32_t device);

#define SPI_DEVICE_SD 0U
#define SPI_DEVICE_LORA 1U
#define SPI_DEVICE_HUM 2U
#define SPI_DEVICE_B_HUM 3U
#define SPI_DEVICE_PRS 4U
#define SPI_DEVICE_ADC 5U
#define SPI_DEVICE_NONE 7U

// LoRA abstraction macros
#define loraSelect() spiSelect(SPI_DEVICE_LORA)
#define loraDeselect() spiSelect(SPI_DEVICE_NONE)
#endif
// I2C has PB8 (SCL), PB9 (SDA)
#define PIN_SCL GPIOB, 8
#define PIN_SDA GPIOB, 9
// UART
#ifdef PHOENIX
#define PIN_UART_RX GPIOB, 7
#define PIN_UART_TX GPIOB, 6
#define PIN_GPS_2_RX GPIOA, 10
#define PIN_GPS_2_TX GPIOA, 9
#endif
// Low power UART
#define PIN_LPUART_RX GPIOB, 11
#define PIN_LPUART_TX GPIOB, 10
// HX1 enable
#define PIN_HX1_EN GPIOA, 6
#ifdef PHOENIX
// Analog inputs for battery voltage (PA5), LED in 1 (PA3), LED in 2 (PA2), LED in 3 (PA1)
#define PIN_BATTERY GPIOA, 5
#define PIN_LED_IN_1 GPIOA, 3
#define PIN_LED_IN_2 GPIOA, 2
#define PIN_LED_IN_3 GPIOA, 1
#else
// Analog inputs for battery voltage (PA3), GPS temp (PA0), battery temp (PA1), GoPro #1 (PA2),
// GoPro #2 (PA6)
#define PIN_TEMP_G GPIOA, 0
#define PIN_TEMP_B GPIOA, 1
#define PIN_PHOTO_1 GPIOA, 2
#define PIN_BATTERY GPIOA, 3
#define PIN_PHOTO_2 GPIOA, 6
#endif
// ADC channels for those pins
#define ADC_CHAN_TEMP_I 18
#ifdef PHOENIX
#define ADC_CHAN_LED_1 3
#define ADC_CHAN_LED_2 2
#define ADC_CHAN_LED_3 1
#define ADC_CHAN_BAT 5
#else
#define ADC_CHAN_TEMP_G 0
#define ADC_CHAN_TEMP_B 1
#define ADC_CHAN_PHOTO_1 2
#define ADC_CHAN_BAT 3
#define ADC_CHAN_PHOTO_2 6
#endif
// Remember the dummy channel if fixing errata 2.2!
#ifdef PHOENIX
#define ADC_IDX_LED_3 0
#define ADC_IDX_LED_2 1
#define ADC_IDX_LED_1 2
#define ADC_IDX_BAT 3
#define ADC_IDX_TEMP_I 4
#else
#define ADC_IDX_TEMP_G 0
#define ADC_IDX_TEMP_B 1
#define ADC_IDX_PHOTO_1 2
#define ADC_IDX_BAT 3
#define ADC_IDX_PHOTO_2 4
#define ADC_IDX_TEMP_I 5
#endif
// LEDs (HABv2 supports dimming, Phoenix only on RED)
#define PIN_LED_R GPIOA, 7
#define PIN_LED_G GPIOB, 0
#define PIN_LED_B GPIOB, 1
// Enable PWM dimming of RED only on Phoenix
#define PHOENIX_LED_PWM
// Phoenix supports PI and radio enable
#ifdef PHOENIX
#define PIN_EN_PI GPIOA, 0
#define PIN_EN_RAD GPIOA, 8
#define PIN_OW_TEMP GPIOB, 15
#endif
// Button for transmission
#define PIN_BUTTON GPIOC, 13
// EXTI bits for the button
#define BUTTON_BIT 0x2000U
// EXTI bits for the RTC
#define RTC_BIT 0x120000U
// EXTI bits for LPUART1 line 28)
#define LPUART1_BIT 0x10000000U

// Status and flags
// RTC wakeup (30-second) interrupt occurred for HX1 transmission
#define FLAG_RTC_30S 0x0001U
// DAC HX1 transfer needed to front half (HTIE fired)
#define FLAG_HX1_FRONT 0x0002U
// DAC HX1 transfer needed to front half (TCIE fired)
#define FLAG_HX1_BACK 0x0004U
// Any DAC flag
#define FLAG_HX1_ANY (FLAG_HX1_FRONT | FLAG_HX1_BACK)
// SysTick 10ms has passed
#define FLAG_SYSTICK 0x0008U
// ADC results are ready
#define FLAG_ADC_READY 0x0010U
// RTC one-second flag used to keep main loop running even with IWDG on
#define FLAG_RTC_1S 0x0020U
// GPS #1 data ready
#define FLAG_GPS_1_READY 0x0040U
// GPS #2 data ready
#define FLAG_GPS_2_READY 0x0080U
// Button was pressed
#define FLAG_BUTTON 0x0100U
// RTC alarm triggered
#define FLAG_RTC_ALARM 0x0200U
// Transmission in progress
#define FLAG_XMIT 0x02000000U
// HX1 radio warming up
#define FLAG_HX1_WARMUP 0x04000000U
// LoRA radio active
#define FLAG_LORA 0x08000000U
// System time set flag
#define FLAG_TIME_SET 0x10000000U
// LSI in use flag (instead of LSE)
#define FLAG_LSI 0x20000000U
// HSI in use flag (instead of HSE)
#define FLAG_HSI 0x40000000U
// Crash and reset flag
#define FLAG_CRASHED 0x80000000U

// Error codes used for diagnostic LED blink out
//  Errors are blinked out LSB first, so this would mean:
//  1st blink: RED = replace battery, GREEN = battery OK
//  2nd blink: RED = LoRA not responding, GREEN = LoRA responding
//  3rd blink: RED = BME280 not responding, GREEN = BME280 responding
//  4th blink: RED = Accelerometer not responding, GREEN = accelerometer responding
//  5th blink: RED = DS18(B)20 not responding, GREEN = at least one DS18(B)20 responding
#define ERR_POWER_CRIT 0x0001U
#define ERR_LORA 0x0002U
#define ERR_BME280 0x0004U
#define ERR_ACCEL 0x0008U
#define ERR_TEMP_1 0x0010U
#define ERR_TEMP_2 0x0020U
#define ERR_POWER_LOW 0x0040U
#define ERR_ANY (ERR_POWER_CRIT | ERR_LORA | ERR_BME280 | ERR_ACCEL | ERR_TEMP_1 | ERR_TEMP_2 |\
	ERR_POWER_LOW)
// Only 5 most LSBs are blinked out on failure
#define ERR_BLINK_BITS 5U

// Fixes warning: format '%u' expects argument of type 'unsigned int', but argument has
// type 'uint32_t'; the warning is spurious since they are convertible
#define SI(_x) ((int)(_x))
#define UI(_x) ((unsigned int)(_x))

// Results from the ADC will end up here
extern volatile uint16_t adcResults[6];
// System state register
extern volatile uint32_t sysFlags;

// Clears the specified flags with no risk of interrupt contention
// Only to be used outside of interrupts
uint32_t clearFlags(const uint32_t set);
// Leaves STOP mode and restores the correct clock settings
void exitStopMode();
// MCU startup routine called by the startup.s
void initMCU();
// Called from the reset handler to run the program
int main(void);
// Sets the specified flags with no risk of interrupt contention
// Only to be used outside of interrupts
uint32_t setFlags(const uint32_t set);
// Sets the LED color values
void setLED(uint32_t red, uint32_t green, uint32_t blue);

#if defined(HABV2) && defined(PHOENIX)
#warning("Only one of HABv2 or Phoenix should be specified")
#endif
#if !defined(HABV2) && !defined(PHOENIX)
#warning("Please specify HABv2 or Phoenix")
#endif

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
