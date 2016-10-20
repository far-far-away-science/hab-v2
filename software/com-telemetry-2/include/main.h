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

// Enable these flags to adjust oscillator speeds
// PLL sets core clock to 32 MHz (HSI * 2) and 1.8 V, otherwise uses 16 MHz (HSI) and 1.5 V
#undef PLL
// LSE enables the 32.768 KHz crystal and RTC
#define LSE
// LSI uses the internal low-speed oscillator; LSE must also be defined to turn on the RTC
#undef LSI
// LSE compensation in parts per 2^20 (approximately ppm), positive speeds it up, negative
// slows it down
#define LSE_COMP -6

// Pin definitions
// DAC
#define PIN_DAC GPIOA, 4
// SPI has PB3 (SCK), PB4 (MISO), PB5 (MOSI), and PA15 (!CS!)
#define PIN_SCK GPIOB, 3
#define PIN_MISO GPIOB, 4
#define PIN_MOSI GPIOB, 5
#define PIN_CS GPIOA, 15
// I2C has PB6 (SCL), PB7 (SDA)
#define PIN_SCL GPIOB, 6
#define PIN_SDA GPIOB, 7
// Amplifier shutdown
#define PIN_AMP_SD GPIOA, 2
// LED (PA6) and piezo (PA7) through transistor
#define PIN_LED GPIOA, 6
#define PIN_PIEZO GPIOA, 7
// Analog inputs for photocell (PA1) and wall power detect (PA3)
#define PIN_POWER_DET GPIOA, 3
#define PIN_PHOTOCELL GPIOA, 1
// USB ports
#define PIN_USBDM GPIOA, 11
#define PIN_USBDP GPIOA, 12
// ADC channels for those two pins
#define ADC_CHAN_PWR 3
#define ADC_CHAN_PC 1
// Remember the dummy channel if fixing errata 2.2!
#define ADC_IDX_PWR 1
#define ADC_IDX_PC 0
// Buttons - snooze (PA0), up (PA5), alarm (PB1), and down (PA8)
// SNOOZE and DOWN silk labels are swapped on the first 3 B-02 boards (fixed in CAD)
#define PIN_SNOOZE GPIOA, 0
#define PIN_UP GPIOA, 5
#define PIN_ALARM GPIOB, 1
#define PIN_DOWN GPIOA, 8
// Bits for the button pins, since they are reused in the GPIO registers and the EXTI registers
#define BTN_SNOOZE_BIT 0x0001
#define BTN_ALARM_BIT 0x0002
#define BTN_UP_BIT 0x0020
#define BTN_DOWN_BIT 0x0100
#define BTN_ALL_BIT (BTN_SNOOZE_BIT | BTN_ALARM_BIT | BTN_UP_BIT | BTN_DOWN_BIT)
// EXTI bit for the RTC
#define RTC_BIT 0x020000

// Results from the ADC will end up here
extern volatile uint16_t adcResults[2];

// MCU startup routine called by the startup.s
void initMCU();
// Called from the reset handler to run the program
int main(void);

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
