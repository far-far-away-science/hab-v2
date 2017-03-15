/*
 * CMSIS Cortex-M0 Device Peripheral Access Layer
 * Provides definitions and functions to access the STM32 low-level registers.
 */

#ifndef CORTEX_H_

// This prevents multiple inclusion
#define CORTEX_H_

// Allow integers to be specified by sign and magnitude (bit-width) independent of definition
#include <stdint.h>
#include <stdbool.h>

// Begin C++ extern to C
#ifdef __cplusplus
extern "C" {
#endif

// Modifier for I/O ports to force direct memory access (avoid register caching)
#define __I volatile
#define __IO volatile

// By default, there are 2 priority bits for the NVIC
#define NVIC_PRIO_BITS 2

// Interrupt types available
typedef enum IRQn {
	// Cortex-M3 default interrupts
	// Non-maskable interrupt
	NonMaskableInt_IRQn = -14,
	// Illegal memory usage or memory management failure (stack overflow, etc.) interrupt
	HardFault_IRQn = -13,
	// SV call interrupt
	SVCall_IRQn = -5,
	// SV pending interrupt
	PendSV_IRQn = -2,
	// System tick interrupt
	SysTick_IRQn = -1,

	// STM32L0 MD specific peripheral interrupts
	// Watchdog interrupt
	WWDG_IRQn = 0,
	// Power supply interrupt
	PVD_IRQn = 1,
	// Real-time clock interrupt
	RTC_IRQn = 2,
	// Flash memory interrupt
	FLASH_IRQn = 3,
	// Reset and clock control interrupt
	RCC_IRQn = 4,
	// External pin interrupts
	EXTI1_0_IRQn = 5,
	EXTI3_2_IRQn = 6,
	EXTI15_4_IRQn = 7,
	// Touch sensing controller interrupt
	TSC_IRQn = 8,
	// DMA channel 1 interrupt
	DMA1_Channel1_IRQn = 9,
	DMA1_Channel3_2_IRQn = 10,
	DMA1_Channel7_4_IRQn = 11,
	// ADC and comparator interrupts (EXTI21/22 technically)
	ADC_COMP_IRQn = 12,
	// Low-power TIM1 interrupt (EXTI29 technically)
	LPTIM1_IRQn = 13,
	// USART 4 and 5 interrupt
	USART4_5_IRQn = 14,
	// Timer #2 interrupt
	TIM2_IRQn = 15,
	// Timer #3 interrupt
	TIM3_IRQn = 16,
	// Timer #6 and DAC interrupt
	TIM6_DAC_IRQn = 17,
	// Timer #7 interrupt
	TIM7_IRQn = 18,
	// Timer #21 interrupt
	TIM21_IRQn = 20,
	// I2C #3 interrupt
	I2C3_IRQn = 21,
	// Timer #22 interrupt
	TIM22_IRQn = 22,
	// I2C #1 interrupt
	I2C1_IRQn = 23,
	// I2C #2 interrupt
	I2C2_IRQn = 24,
	// SPI #1 interrupt
	SPI1_IRQn = 25,
	// SPI #2 interrupt
	SPI2_IRQn = 26,
	// USART #1 interrupt
	USART1_IRQn = 27,
	// USART #2 interrupt
	USART2_IRQn = 28,
	// RNG and LPUART1 interrupt
	RNG_LPUART1_IRQn = 29,
	// LCD interrupt
	LCD_IRQn = 30,
	// USB interrupt
	USB_IRQn = 31,
} IRQn_Type;

// ADC register access structure
typedef struct {
	// Interrupt and status register
	__IO uint32_t ISR;
	// Interrupt enable register
	__IO uint32_t IER;
	// Control register
	__IO uint32_t CR;
	// Configuration register #1
	__IO uint32_t CFGR1;
	// Configuration register #2
	__IO uint32_t CFGR2;
	// Sampling time register
	__IO uint32_t SMPR;
	uint32_t RESERVED0[2];
	// Analog watchdog threshold register
	__IO uint32_t TR;
	uint32_t RESERVED1;
	// Channel selection register
	__IO uint32_t CHSELR;
	uint32_t RESERVED2[5];
	// Regular data register
	__I uint32_t DR;
	uint32_t RESERVED3[28];
	// Calibration register
	__IO uint32_t CALFACT;
} ADC_TypeDef;

// ADC common area register access structure
typedef struct {
	__IO uint32_t CCR;
} ADC_Common_TypeDef;

// Comparator register access structure
typedef struct {
	// Control and status register
	__IO uint32_t CSR;
} COMP_TypeDef;

// CRC register access structure
typedef struct {
	// Data register (evil union hack)
	union {
		__IO uint32_t DR;
		__IO uint16_t DR16;
		__IO uint8_t DR8;
	};
	// Independent data register
	__IO uint32_t IDR;
	// Control register
	__IO uint32_t CR;
	uint32_t RESERVED;
	// Initial value register
	__IO uint32_t INIT;
	// Polynomial register
	__IO uint32_t POL;
} CRC_TypeDef;

// Clock recovery system register access structure
typedef struct {
	// Control register
	__IO uint32_t CR;
	// Configuration register
	__IO uint32_t CFGR;
	// Interrupt and status register
	__IO uint32_t ISR;
	// Interrupt flag clear register
	__IO uint32_t ICR;
} CRS_TypeDef;

// DAC register access structure
typedef struct {
	// Control register
	__IO uint32_t CR;
	// Software trigger register
	__IO uint32_t SWTRIGR;
	// Channel 1 12-bit right aligned data register
	__IO uint32_t DHR12R1;
	// Channel 1 12-bit left aligned data register
	__IO uint32_t DHR12L1;
	// Channel 1 8-bit right aligned data register
	__IO uint32_t DHR8R1;
	uint32_t RESERVED0[6];
	// Channel 1 data output register
	__I uint32_t DOR1;
	uint32_t RESERVED1;
	// Status register
	__IO uint32_t SR;
} DAC_TypeDef;

// DMA channel register access structure
typedef struct {
	// Control register
	__IO uint32_t CCR;
	// Data count register
	__IO uint32_t CNDTR;
	// Peripheral address register
	__IO uint32_t CPAR;
	// Memory address register
	__IO uint32_t CMAR;
} DMA_Channel_TypeDef;

// DMA system control register access structure
typedef struct {
	// Interrupt status register
	__IO uint32_t ISR;
	// Interrupt flag clear register
	__IO uint32_t IFCR;
} DMA_TypeDef;

// DMA channel remap register access structure
typedef struct {
	// Channel remap register
	__IO uint32_t CSELR;
} DMA_Request_TypeDef;

// External interrupt register access structure
typedef struct {
	// Interrupt mask register
	__IO uint32_t IMR;
	// Event mask register
	__IO uint32_t EMR;
	// Rising trigger selection register
	__IO uint32_t RTSR;
	// Falling trigger selection register
	__IO uint32_t FTSR;
	// Software interrupt event register
	__IO uint32_t SWIER;
	// Interrupt pending register
	__IO uint32_t PR;
} EXTI_TypeDef;

// FLASH memory register access structure
typedef struct {
	// Access control register
	__IO uint32_t ACR;
	// Program/erase control register
	__IO uint32_t PECR;
	// Power down key register
	__IO uint32_t PDKEYR;
	// Program/erase key register
	__IO uint32_t PEKEYR;
	// Program memory key register
	__IO uint32_t PRGKEYR;
	// Option byte key register
	__IO uint32_t OPTKEYR;
	// Status register
	__IO uint32_t SR;
	// Option byte register
	__IO uint32_t OBR;
	// Write protection register
	__IO uint32_t WRPR;
} FLASH_TypeDef;

// GPIO register access structure
typedef struct {
	// Port mode register
	__IO uint32_t MODER;
	// Output type register
	__IO uint32_t OTYPER;
	// Output speed register
	__IO uint32_t OSPEEDR;
	// Pull-up/pull-down enable register
	__IO uint32_t PUPDR;
	// Input data register
	__IO uint32_t IDR;
	// Output data register
	__IO uint32_t ODR;
	// Bit set register
	__IO uint32_t BSRR;
	// Configuration lock register
	__IO uint32_t LCKR;
	// Alternate function I/O registers
	__IO uint32_t AFR[2];
	// Bit reset register
	__IO uint32_t BRR;
} GPIO_TypeDef;

// I2C register access structure
typedef struct {
	// Configuration register #1
	__IO uint32_t CR1;
	// Configuration register #2
	__IO uint32_t CR2;
	// Own-address register #1
	__IO uint32_t OAR1;
	// Own-address register #2
	__IO uint32_t OAR2;
	// Timing register
	__IO uint32_t TIMINGR;
	// Timeout register
	__IO uint32_t TIMEOUTR;
	// Interrupt and status register
	__IO uint32_t ISR;
	// Interrupt clear register
	__IO uint32_t ICR;
	// PEC register
	__IO uint32_t PECR;
	// Receive data register
	__IO uint32_t RXDR;
	// Transmit data register
	__IO uint32_t TXDR;
} I2C_TypeDef;

// Independent watchdog register access structure
typedef struct {
	// Key register
	__IO uint32_t KR;
	// Prescaler register
	__IO uint32_t PR;
	// Reload register
	__IO uint32_t RLR;
	// Status register
	__IO uint32_t SR;
	// Window register
	__IO uint32_t WINR;
} IWDG_TypeDef;

// Low-power timer register access structure
typedef struct {
	// Interrupt and status register
	__IO uint32_t ISR;
	// Interrupt clear register
	__IO uint32_t ICR;
	// Interrupt enable register
	__IO uint32_t IER;
	// Configuration register
	__IO uint32_t CFGR;
	// Control register
	__IO uint32_t CR;
	// Compare register
	__IO uint32_t CMP;
	// Autoreload register
	__IO uint32_t ARR;
	// Counter register
	__IO uint32_t CNT;
} LPTIM_TypeDef;

// Interrupt controller register access structure
typedef struct {
	// Interrupt Set Enable register
	__IO uint32_t ISER[1];
	uint32_t RESERVED0[31];
	// Interrupt Clear Enable register
	__IO uint32_t ICER[1];
	uint32_t RESERVED1[31];
	// Interrupt Set Pending register
	__IO uint32_t ISPR[1];
	uint32_t RESERVED2[31];
	// Interrupt Clear Pending register
	__IO uint32_t ICPR[1];
	uint32_t RESERVED3[31 + 64];
	// Interrupt Priority register
	__IO uint32_t IP[8];
} NVIC_TypeDef;

// Power register access struture
typedef struct {
	// Power control register
	__IO uint32_t CR;
	// Power status register
	__IO uint32_t CSR;
} PWR_TypeDef;

// Reset and clock control register access structure
typedef struct {
	// Control register
	__IO uint32_t CR;
	// Internal clock source calibration register
	__IO uint32_t ICSCR;
	// Clock recovery RC (48 MHz) register
	__IO uint32_t CRRCR;
	// Configuration register
	__IO uint32_t CFGR;
	// Interrupt enable register
	__IO uint32_t CIER;
	// Interrupt flag register
	__IO uint32_t CIFR;
	// Interrupt flag clear register
	__IO uint32_t CICR;
	// IO port reset register
	__IO uint32_t IOPRSTR;
	// AHB peripheral reset register
	__IO uint32_t AHBRSTR;
	// APB2 peripheral reset register
	__IO uint32_t APB2RSTR;
	// APB1 peripheral reset register
	__IO uint32_t APB1RSTR;
	// IO port enable register
	__IO uint32_t IOPENR;
	// AHB clock enable register
	__IO uint32_t AHBENR;
	// APB2 clock enable register
	__IO uint32_t APB2ENR;
	// APB1 clock enable register
	__IO uint32_t APB1ENR;
	// IO port enable in sleep mode register
	__IO uint32_t IOPSMENR;
	// AHB clock enable in sleep mode register
	__IO uint32_t AHBSMENR;
	// APB2 clock enable in sleep mode register
	__IO uint32_t APB2SMENR;
	// APB1 clock enable in sleep mode register
	__IO uint32_t APB1SMENR;
	// Clock configuration register
	__IO uint32_t CCIPR;
	// Control and status register
	__IO uint32_t CSR;
} RCC_TypeDef;

// RTC register access structure
typedef struct {
	// Time register
	__IO uint32_t TR;
	// Date register
	__IO uint32_t DR;
	// Control register
	__IO uint32_t CR;
	// Initialization and status register
	__IO uint32_t ISR;
	// Prescaler register
	__IO uint32_t PRER;
	// Wakeup timer register
	__IO uint32_t WUTR;
	uint32_t RESERVED;
	// Alarm A register
	__IO uint32_t ALRMAR;
	// Alarm B register
	__IO uint32_t ALRMBR;
	// Write protection register
	__IO uint32_t WPR;
	// Sub-second register
	__IO uint32_t SSR;
	// Shift control register
	__IO uint32_t SHIFTR;
	// Timestamp time register
	__IO uint32_t TSTR;
	// Timestamp date register
	__IO uint32_t TSDR;
	// Timestamp sub-second register
	__IO uint32_t TSSSR;
	// Calibration register
	__IO uint32_t CALR;
	// Tamper configuration register
	__IO uint32_t TAMPCR;
	// Alarm A sub-second register
	__IO uint32_t ALRMASSR;
	// Alarm B sub-second register
	__IO uint32_t ALRMBSSR;
	// Option register
	__IO uint32_t OR;
	// Backup registers
	__IO uint32_t BKPR[5];
} RTC_TypeDef;

// System Control Block register access structure
typedef struct {
	// CPU ID register
	__I uint32_t CPUID;
	// Interrupt Control State register
	__IO uint32_t ICSR;
	// Vector Table Offset register
	__IO uint32_t VTOR;
	// Application Interrupt and Reset Control register
	__IO uint32_t AIRCR;
	// System Control register
	__IO uint32_t SCR;
	// Configuration Control register
	__IO uint32_t CCR;
	uint32_t RESERVED0;
	// System Handler Priority register
	__IO uint32_t SHP[2];
	// System Handler Control and State register
	__IO uint32_t SHCSR;
} SCB_TypeDef;

// SPI register access structure
typedef struct {
	// Control register #1
	__IO uint32_t CR1;
	// Control register #2
	__IO uint32_t CR2;
	// Status register
	__IO uint32_t SR;
	// Data register
	__IO uint32_t DR;
	// CRC polynomial register
	__IO uint32_t CRCPR;
	// Receive CRC register
	__IO uint32_t RXCRCR;
	// Transmit CRC register
	__IO uint32_t TXCRCR;
	// I2S configuration register
	__IO uint32_t I2SCFGR;
	// I2C prescaler register
	__IO uint32_t I2SPR;
} SPI_TypeDef;

// SYSCFG register access structure
typedef struct {
	// Configuration register #1
	__IO uint32_t CFGR1;
	// Configuration register #2
	__IO uint32_t CFGR2;
	// External interrupt configuration register
	__IO uint32_t EXTICR[4];
	uint32_t RESERVED[2];
	// Configuration register #3
	__IO uint32_t CFGR3;
} SYSCFG_TypeDef;

// SysTick register access structure
typedef struct {
	// Control and status register
	__IO uint32_t CTRL;
	// Reload value register
	__IO uint32_t LOAD;
	// Current value register
	__IO uint32_t VAL;
	// SysTick Calibration register
	__I uint32_t CALIB;
} SysTick_TypeDef;

// Timer control register access structure
typedef struct {
	// Configuration register #1
	__IO uint32_t CR1;
	// Configuration register #2
	__IO uint32_t CR2;
	// Slave mode configuration register
	__IO uint32_t SMCR;
	// Interrupt enable register
	__IO uint32_t DIER;
	// Status register
	__IO uint32_t SR;
	// Event generation register
	__IO uint32_t EGR;
	// Capture/compare mode register #1
	__IO uint32_t CCMR1;
	// Capture/compare mode register #2
	__IO uint32_t CCMR2;
	// Capture/compare enable register
	__IO uint32_t CCER;
	// Timer count register
	__IO uint32_t CNT;
	// Timer prescaler register
	__IO uint32_t PSC;
	// Auto-reload register
	__IO uint32_t ARR;
	// Repetition counter register
	__IO uint32_t RCR;
	// Capture/compare value register #1
	__IO uint32_t CCR1;
	// Capture/compare value register #2
	__IO uint32_t CCR2;
	// Capture/compare value register #3
	__IO uint32_t CCR3;
	// Capture/compare value register #4
	__IO uint32_t CCR4;
	// Break and dead time register
	__IO uint32_t BDTR;
	// DMA control register
	__IO uint32_t DCR;
	// DMA address for full transfer
	__IO uint32_t DMAR;
	// Option register
	__IO uint32_t OR;
} TIM_TypeDef;

// USART control register access structure
typedef struct {
	// Control register #1
	__IO uint32_t CR1;
	// Control register #2
	__IO uint32_t CR2;
	// Control register #3
	__IO uint32_t CR3;
	// Baud rate register
	__IO uint32_t BRR;
	// Guard time and prescaler register
	__IO uint32_t GTPR;
	// Receiver timeout register
	__IO uint32_t RTOR;
	// Request register
	__IO uint32_t RQR;
	// Interrupt and status register
	__IO uint32_t ISR;
	// Interrupt flag clear register
	__IO uint32_t ICR;
	// Receive data register
	__IO uint32_t RDR;
	// Transmit data register
	__IO uint32_t TDR;
} USART_TypeDef;

// USB register access structure
#define USB_NR_EP_REGS 8
typedef struct {
	// Endpoint registers
	__IO uint32_t EP[8];
	uint16_t RESERVED1[16];
	// Control register
	__IO uint32_t CNTR;
	// Interrupt status register
	__IO uint32_t ISTR;
	// Frame number register
	__IO uint32_t FNR;
	// Device address register
	__IO uint32_t DADDR;
	// Buffer table address register, 8-byte aligned
	__IO uint32_t BTABLE;
	// LPM control and status register
	__IO uint32_t LPMCSR;
	// Battery charging detector register
	__IO uint32_t BCDR;
} USB_TypeDef;

// Window watchdog register access structure
typedef struct {
	// Control register
	__IO uint32_t CR;
	// Configuration register
	__IO uint32_t CFR;
	// Status register
	__IO uint32_t SR;
} WWDG_TypeDef;

// Peripheral memory map: addresses in the direct region
// Flash base address
#define FLASH_BASE ((uint32_t)0x08000000)
// Flash second bank base address
#define FLASH_BANK2_BASE ((uint32_t)0x08018000)
// EEPROM base address
#define DATA_EEPROM_BASE ((uint32_t)0x08080000)
// EEPROM second bank base address
#define DATA_EEPROM_BANK2_BASE ((uint32_t)0x08080C00)
// RAM base address
#define SRAM_BASE ((uint32_t)0x20000000)
// Peripheral base address
#define PERIPH_BASE ((uint32_t)0x40000000)
// SCS base address
#define SCS_BASE ((uint32_t)0xE000E000)

// APB1 peripherals base address
#define APB1PERIPH_BASE PERIPH_BASE
// APB2 peripherals base address
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
// AHB peripherals base address
#define AHBPERIPH_BASE (PERIPH_BASE + 0x20000)
// I/O ports base address
#define IOPPERIPH_BASE (PERIPH_BASE + 0x10000000)

// Peripherals on APB1
// Timer #2
#define TIM2_BASE (APB1PERIPH_BASE + 0x00000000)
#define TIM2 ((TIM_TypeDef*)TIM2_BASE)
// Timer #3
#define TIM3_BASE (APB1PERIPH_BASE + 0x00000400)
#define TIM3 ((TIM_TypeDef*)TIM3_BASE)
// Timer #6
#define TIM6_BASE (APB1PERIPH_BASE + 0x00001000)
#define TIM6 ((TIM_TypeDef*)TIM6_BASE)
// RTC
#define RTC_BASE (APB1PERIPH_BASE + 0x00002800)
#define RTC ((RTC_TypeDef*)RTC_BASE)
// Window watchdog
#define WWDG_BASE (APB1PERIPH_BASE + 0x00002C00)
#define WWDG ((WWDG_TypeDef*)WWDG_BASE)
// Independent watchdog
#define IWDG_BASE (APB1PERIPH_BASE + 0x00003000)
#define IWDG ((IWDG_TypeDef*)IWDG_BASE)
// SPI #2
#define SPI2_BASE (APB1PERIPH_BASE + 0x00003800)
#define SPI2 ((SPI_TypeDef*)SPI2_BASE)
// USART #2
#define USART2_BASE (APB1PERIPH_BASE + 0x00004400)
#define USART2 ((USART_TypeDef*)USART2_BASE)
// Low-power USART #1
#define LPUART1_BASE (APB1PERIPH_BASE + 0x00004800)
#define LPUART1 ((USART_TypeDef*)LPUART1_BASE)
// USART #4
#define USART4_BASE (APB1PERIPH_BASE + 0x00004C00)
#define USART4 ((USART_TypeDef*)USART4_BASE)
// USART #5
#define USART5_BASE (APB1PERIPH_BASE + 0x00005000)
#define USART5 ((USART_TypeDef*)USART5_BASE)
// I2C #1
#define I2C1_BASE (APB1PERIPH_BASE + 0x00005400)
#define I2C1 ((I2C_TypeDef*)I2C1_BASE)
// I2C #2
#define I2C2_BASE (APB1PERIPH_BASE + 0x00005800)
#define I2C2 ((I2C_TypeDef*)I2C2_BASE)
// USB
#define USB_BASE (APB1PERIPH_BASE + 0x00005C00)
#define USB ((USB_TypeDef*)USB_BASE)
// PMA (shared between USB and CAN) peripheral shared RAM
#define PMA_BASE (APB1PERIPH_BASE + 0x00006000)
#define PMA ((__IO uint8_t*)PMA_BASE)
// Clock recovery system
#define CRS_BASE (APB1PERIPH_BASE + 0x00006C00)
#define CRS ((CRS_TypeDef*)CRS_BASE)
// Power control
#define PWR_BASE (APB1PERIPH_BASE + 0x00007000)
#define PWR ((PWR_TypeDef*)PWR_BASE)
// DAC
#define DAC_BASE (APB1PERIPH_BASE + 0x00007400)
#define DAC ((DAC_TypeDef*)DAC_BASE)
// I2C #3
#define I2C3_BASE (APB1PERIPH_BASE + 0x00007800)
#define I2C3 ((I2C_TypeDef*)I2C3_BASE)
// Low-power timer #1
#define LPTIM1_BASE (APB1PERIPH_BASE + 0x00007C00)
#define LPTIM1 ((LPTIM_TypeDef*)LPTIM1_BASE)

// Peripherals on APB2
// System configuration
#define SYSCFG_BASE (APB2PERIPH_BASE + 0x00000000)
#define SYSCFG ((SYSCFG_TypeDef*)SYSCFG_BASE)
// Comparator #1
#define COMP1_BASE (APB2PERIPH_BASE + 0x00000018)
#define COMP1 ((COMP_TypeDef*)COMP1_BASE)
// Comparator #2
#define COMP2_BASE (APB2PERIPH_BASE + 0x0000001C)
#define COMP2 ((COMP_TypeDef*)COMP1_BASE)
// External interrupt management
#define EXTI_BASE (APB2PERIPH_BASE + 0x00000400)
#define EXTI ((EXTI_TypeDef*)EXTI_BASE)
// Timer #21
#define TIM21_BASE (APB2PERIPH_BASE + 0x00000800)
#define TIM21 ((TIM_TypeDef*)TIM21_BASE)
// Timer #22
#define TIM22_BASE (APB2PERIPH_BASE + 0x00001400)
#define TIM22 ((TIM_TypeDef*)TIM22_BASE)
// ADC #1
#define ADC1_BASE (APB2PERIPH_BASE + 0x00002400)
#define ADC1 ((ADC_TypeDef*)ADC1_BASE)
// ADC common
#define ADC_BASE (APB2PERIPH_BASE + 0x00002708)
#define ADC ((ADC_Common_TypeDef*)ADC1_BASE)
// SPI #1
#define SPI1_BASE (APB2PERIPH_BASE + 0x00003000)
#define SPI1 ((SPI_TypeDef*)SPI1_BASE)
// USART #1
#define USART1_BASE (APB2PERIPH_BASE + 0x00003800)
#define USART1 ((USART_TypeDef*)USART1_BASE)

// Reset and clock control
#define RCC_BASE (AHBPERIPH_BASE + 0x1000)
#define RCC ((RCC_TypeDef*)RCC_BASE)
// DMA #1
#define DMA1_BASE (AHBPERIPH_BASE + 0x0000)
#define DMA1 ((DMA_TypeDef*)DMA1_BASE)
#define DMA1_Channel1_BASE (DMA1_BASE + 0x0008)
#define DMA1_Channel1 ((DMA_Channel_TypeDef*)DMA1_Channel1_BASE)
#define DMA1_Channel2_BASE (DMA1_BASE + 0x001C)
#define DMA1_Channel2 ((DMA_Channel_TypeDef*)DMA1_Channel2_BASE)
#define DMA1_Channel3_BASE (DMA1_BASE + 0x0030)
#define DMA1_Channel3 ((DMA_Channel_TypeDef*)DMA1_Channel3_BASE)
#define DMA1_Channel4_BASE (DMA1_BASE + 0x0044)
#define DMA1_Channel4 ((DMA_Channel_TypeDef*)DMA1_Channel4_BASE)
#define DMA1_Channel5_BASE (DMA1_BASE + 0x0058)
#define DMA1_Channel5 ((DMA_Channel_TypeDef*)DMA1_Channel5_BASE)
#define DMA1_Channel6_BASE (DMA1_BASE + 0x006C)
#define DMA1_Channel6 ((DMA_Channel_TypeDef*)DMA1_Channel6_BASE)
#define DMA1_Channel7_BASE (DMA1_BASE + 0x0080)
#define DMA1_Channel7 ((DMA_Channel_TypeDef*)DMA1_Channel7_BASE)
#define DMA1_Request_BASE (DMA1_BASE + 0x00A8)
#define DMA1_Request ((DMA_Request_TypeDef*)DMA1_Request_BASE)
// SysTick
#define SysTick_BASE (SCS_BASE + 0x0010)
#define SysTick ((SysTick_TypeDef*)SysTick_BASE)
// NVIC
#define NVIC_BASE (SCS_BASE + 0x0100)
#define NVIC ((NVIC_TypeDef*)NVIC_BASE)
// System Control Block
#define SCB_BASE (SCS_BASE + 0x0D00)
#define SCB ((SCB_TypeDef*)SCB_BASE)
// Flash memory
#define FLASH_R_BASE (AHBPERIPH_BASE + 0x2000)
#define FLASH ((FLASH_TypeDef*)FLASH_R_BASE)
// CRC
#define CRC_BASE (AHBPERIPH_BASE + 0x00003000)
#define CRC ((CRC_TypeDef*)CRC_BASE)
// RNG
#define RNG_BASE (AHBPERIPH_BASE + 0x00005000)

// GPIO A
#define GPIOA_BASE (IOPPERIPH_BASE + 0x00000000)
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)
// GPIO B
#define GPIOB_BASE (IOPPERIPH_BASE + 0x00000400)
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)
// GPIO C
#define GPIOC_BASE (IOPPERIPH_BASE + 0x00000800)
#define GPIOC ((GPIO_TypeDef*)GPIOC_BASE)
// GPIO D
#define GPIOD_BASE (IOPPERIPH_BASE + 0x00000C00)
#define GPIOD ((GPIO_TypeDef*)GPIOD_BASE)
// GPIO E
#define GPIOE_BASE (IOPPERIPH_BASE + 0x00001000)
#define GPIOE ((GPIO_TypeDef*)GPIOE_BASE)
// GPIO H
#define GPIOH_BASE (IOPPERIPH_BASE + 0x00001C00)
#define GPIOH ((GPIO_TypeDef*)GPIOH_BASE)

// ADC defines
// ADC ready
#define ADC_ISR_ADRDY ((uint32_t)0x00000001)
// End-of-sampling flag
#define ADC_ISR_EOSMP ((uint32_t)0x00000002)
// End-of-conversion flag
#define ADC_ISR_EOC ((uint32_t)0x00000004)
// End-of-sequence flag
#define ADC_ISR_EOSEQ ((uint32_t)0x00000008)
// Overrun flag
#define ADC_ISR_OVR ((uint32_t)0x00000010)
// Analog watchdog triggered flag
#define ADC_ISR_AWD ((uint32_t)0x00000080)
// End of calibration flag
#define ADC_ISR_EOCAL ((uint32_t)0x00000800)

// ADC ready interrupt enable
#define ADC_IER_ADRDY ((uint32_t)0x00000001)
// End-of-sampling interrupt enable
#define ADC_IER_EOSMP ((uint32_t)0x00000002)
// End-of-conversion interrupt enable
#define ADC_IER_EOC ((uint32_t)0x00000004)
// End-of-sequence interrupt enable
#define ADC_IER_EOSEQ ((uint32_t)0x00000008)
// Overrun interrupt enable
#define ADC_IER_OVR ((uint32_t)0x00000010)
// Analog watchdog triggered interrupt enable
#define ADC_IER_AWD ((uint32_t)0x00000080)
// End of calibration interrupt enable
#define ADC_IER_EOCAL ((uint32_t)0x00000800)

// ADC enable
#define ADC_CR_ADEN ((uint32_t)0x00000001)
// ADC disable
#define ADC_CR_ADDIS ((uint32_t)0x00000002)
// ADC start
#define ADC_CR_ADSTART ((uint32_t)0x00000004)
// ADC abort
#define ADC_CR_ADSTP ((uint32_t)0x00000010)
// ADC voltage regulator enable
#define ADC_CR_ADVREGEN ((uint32_t)0x10000000)
// ADC calibration start and run flag
#define ADC_CR_ADCAL ((uint32_t)0x80000000)

// ADC DMA enable
#define ADC_CFGR1_DMAEN ((uint32_t)0x00000001)
// ADC DMA circular mode enable
#define ADC_CFGR1_DMACFG ((uint32_t)0x00000002)
// Scan ADC channels in reverse
#define ADC_CFGR1_SCANDIR ((uint32_t)0x00000004)
// ADC resolution bits
#define ADC_CFGR1_RES_0 ((uint32_t)0x00000008)
#define ADC_CFGR1_RES_1 ((uint32_t)0x00000010)
#define ADC_CFGR1_RES ((uint32_t)0x00000018)
// ADC resolution of 12 bits
#define ADC_CFGR1_RES_12BIT ((uint32_t)0x00000000)
// ADC resolution of 10 bits
#define ADC_CFGR1_RES_10BIT ((uint32_t)0x00000008)
// ADC resolution of 8 bits
#define ADC_CFGR1_RES_BIT ((uint32_t)0x00000010)
// ADC resolution of 6 bits
#define ADC_CFGR1_RES_6BIT ((uint32_t)0x00000018)
// Align ADC data to the left
#define ADC_CFGR1_ALIGN ((uint32_t)0x00000020)
// ADC external trigger selection bits
#define ADC_CFGR1_EXTSEL ((uint32_t)0x000001C0)
// ADC external trigger polarity selection bits
#define ADC_CFGR1_EXTEN ((uint32_t)0x00000C00)
#define ADC_CFGR1_EXTEN_0 ((uint32_t)0x00000400)
#define ADC_CFGR1_EXTEN_1 ((uint32_t)0x00000800)
// Overwrite old data on ADC overrun
#define ADC_CFGR1_OVRMOD ((uint32_t)0x00001000)
// ADC continuous conversion enable
#define ADC_CFGR1_CONT ((uint32_t)0x00002000)
// ADC wait conversion enable
#define ADC_CFGR1_WAIT ((uint32_t)0x00004000)
// ADC automatic shutoff enable
#define ADC_CFGR1_AUTOFF ((uint32_t)0x00008000)
// ADC discontinuous mode enable
#define ADC_CFGR1_DISCEN ((uint32_t)0x00010000)
// Enable ADC analog watchdog on one channel only
#define ADC_CFGR1_AWDSGL ((uint32_t)0x00400000)
// ADC analog watchdog enable
#define ADC_CFGR1_AWDEN ((uint32_t)0x00800000)

// ADC oversampling enable
#define ADC_CFGR2_OVSE ((uint32_t)0x00000001)
// ADC oversampling ratio bits
#define ADC_CFGR2_OVSR ((uint32_t)0x0000001C)
#define ADC_CFGR2_OVSR_2 ((uint32_t)0x00000000)
#define ADC_CFGR2_OVSR_4 ((uint32_t)0x00000004)
#define ADC_CFGR2_OVSR_8 ((uint32_t)0x00000008)
#define ADC_CFGR2_OVSR_16 ((uint32_t)0x0000000C)
#define ADC_CFGR2_OVSR_32 ((uint32_t)0x00000010)
#define ADC_CFGR2_OVSR_64 ((uint32_t)0x00000014)
#define ADC_CFGR2_OVSR_128 ((uint32_t)0x00000018)
#define ADC_CFGR2_OVSR_256 ((uint32_t)0x0000001C)
// ADC oversampling shift bits
#define ADC_CFGR2_OVSS ((uint32_t)0x000001E0)
#define ADC_CFGR2_OVSS_NONE ((uint32_t)0x00000000)
#define ADC_CFGR2_OVSS_1BIT ((uint32_t)0x00000020)
#define ADC_CFGR2_OVSS_2BITS ((uint32_t)0x00000040)
#define ADC_CFGR2_OVSS_3BITS ((uint32_t)0x00000060)
#define ADC_CFGR2_OVSS_4BITS ((uint32_t)0x00000080)
#define ADC_CFGR2_OVSS_5BITS ((uint32_t)0x000000A0)
#define ADC_CFGR2_OVSS_6BITS ((uint32_t)0x000000C0)
#define ADC_CFGR2_OVSS_7BITS ((uint32_t)0x000000E0)
#define ADC_CFGR2_OVSS_8BITS ((uint32_t)0x00000100)
// Require triggers for ADC oversampling
#define ADC_CFGR2_TOVS ((uint32_t)0x00000200)
// ADC clock mode selection bits
#define ADC_CFGR2_CKMODE_0 ((uint32_t)0x40000000)
#define ADC_CFGR2_CKMODE_1 ((uint32_t)0x80000000)
#define ADC_CFGR2_CKMODE ((uint32_t)0xC0000000)
// ADC clock mode using HSI (asynchronous)
#define ADC_CFGR2_CKMODE_ASYNC ((uint32_t)0x00000000)
// ADC clock mode using APB2CLK / 2
#define ADC_CFGR2_CKMODE_PCLK_2 ((uint32_t)0x40000000)
// ADC clock mode using APB2CLK / 4
#define ADC_CFGR2_CKMODE_PCLK_4 ((uint32_t)0x80000000)
// ADC clock mode using APB2CLK (may only be used in a few circumstances)
#define ADC_CFGR2_CKMODE_PCLK ((uint32_t)0xC0000000)

// CRC defines
// Reset CRC
#define CRC_CR_RESET ((uint32_t)0x00000001)
// CRC polynomial size
#define CRC_CR_POLYSIZE_0 ((uint32_t)0x00000008)
#define CRC_CR_POLYSIZE_1 ((uint32_t)0x00000010)
#define CRC_CR_POLYSIZE ((uint32_t)0x00000018)
// 32-bit CRC polynomial
#define CRC_CR_POLYSIZE_32BIT ((uint32_t)0x00000000)
// 16-bit CRC polynomial
#define CRC_CR_POLYSIZE_16BIT ((uint32_t)0x00000008)
// 8-bit CRC polynomial
#define CRC_CR_POLYSIZE_8BIT ((uint32_t)0x00000010)
// 7-bit CRC polynomial
#define CRC_CR_POLYSIZE_7BIT ((uint32_t)0x00000018)
// Reverse CRC input data bits
#define CRC_CR_REV_IN_0 ((uint32_t)0x00000020)
#define CRC_CR_REV_IN_1 ((uint32_t)0x00000040)
#define CRC_CR_REV_IN ((uint32_t)0x00000060)
// No CRC input bits reverse
#define CRC_CR_REV_IN_NONE ((uint32_t)0x00000000)
// CRC input bits reversed by byte
#define CRC_CR_REV_IN_BYTE ((uint32_t)0x00000020)
// CRC input bits reversed by halfword
#define CRC_CR_REV_IN_HWORD ((uint32_t)0x00000040)
// CRC input bits reversed by word
#define CRC_CR_REV_IN_WORD ((uint32_t)0x00000060)
// Reverse CRC output bits
#define CRC_CR_REV_OUT ((uint32_t)0x00000080)

// CRS defines
// Enable CRS automatic trimming
#define CRS_CR_AUTOTRIMEN ((uint32_t)0x00000040)
// Enable CRS frequency error counter
#define CRS_CR_CEN ((uint32_t)0x00000020)

// DAC defines
// DAC channel 1 enable/disable flag
#define DAC_CR_EN1 ((uint32_t)0x00000001)
// DAC channel 1 buffer disable
#define DAC_CR_BOFF1 ((uint32_t)0x00000002)
// DAC channel 1 trigger enable
#define DAC_CR_TEN1 ((uint32_t)0x00000004)
// Use Timer 6 as DMA trigger
#define DAC_CR_TSEL1_TIM6 ((uint32_t)0x00000000)
// Use Timer 21 as DMA trigger
#define DAC_CR_TSEL1_TIM21 ((uint32_t)0x00000018)
// Use Timer 2 as DMA trigger
#define DAC_CR_TSEL1_TIM2 ((uint32_t)0x00000020)
// Use EXTI line 9 as DMA trigger
#define DAC_CR_TSEL1_EXTI9 ((uint32_t)0x00000030)
// Use software as DMA trigger
#define DAC_CR_TSEL1_SOFT ((uint32_t)0x00000038)
// All trigger bits
#define DAC_CR_TSEL1_BITS ((uint32_t)0x00000038)
// DAC channel 1 DMA enable
#define DAC_CR_DMAEN1 ((uint32_t)0x00001000)

// Trigger from software
#define DAC_SWTRIGR_SWTRIG ((uint32_t)0x00000001)

// DMA defines
// DMA channel enable
#define DMA_CCR_EN ((uint32_t)0x00000001)
// Enable transfer complete interrupt
#define DMA_CCR_TCIE ((uint32_t)0x00000002)
// Enable half transfer interrupt
#define DMA_CCR_HTIE ((uint32_t)0x00000004)
// Enable transfer error interrupt
#define DMA_CCR_TEIE ((uint32_t)0x00000008)
// Peripheral to memory
#define DMA_CCR_SRC ((uint32_t)0x00000000)
// Memory to peripheral
#define DMA_CCR_DST ((uint32_t)0x00000010)
// Circular mode enable
#define DMA_CCR_CIRC ((uint32_t)0x00000020)
// Auto-increment peripheral address
#define DMA_CCR_PERIPHINC ((uint32_t)0x00000040)
// Auto-increment memory address
#define DMA_CCR_MEMINC ((uint32_t)0x00000080)
// Source size of byte
#define DMA_CCR_SRC_BYTE ((uint32_t)0x00000000)
// Source size of halfword
#define DMA_CCR_SRC_HWORD ((uint32_t)0x00000100)
// Source size of word
#define DMA_CCR_SRC_WORD ((uint32_t)0x00000200)
// Destination size of byte
#define DMA_CCR_DST_BYTE ((uint32_t)0x00000000)
// Destination size of halfword
#define DMA_CCR_DST_HWORD ((uint32_t)0x00000400)
// Destination size of word
#define DMA_CCR_DST_WORD ((uint32_t)0x00000800)
// Low priority transfer
#define DMA_CCR_PRI_LOW ((uint32_t)0x00000000)
// Medium priority transfer
#define DMA_CCR_PRI_MED ((uint32_t)0x00001000)
// High priority transfer
#define DMA_CCR_PRI_HIGH ((uint32_t)0x00002000)
// Very high priority transfer
#define DMA_CCR_PRI_MAX ((uint32_t)0x00003000)
// Memory-to-memory?
#define DMA_CCR_M2M ((uint32_t)0x00004000)

// Remap DMA channel 1 to ADC
#define DMA_CSELR_C1S_ADC ((uint32_t)0x00000000)
// Remap DMA channel 1 to TIM2 CH3
#define DMA_CSELR_C1S_TIM2 ((uint32_t)0x00000008)
// Remap DMA channel 1 to AES input
#define DMA_CSELR_C1S_AES ((uint32_t)0x0000000B)
// Remap DMA channel 2 to ADC
#define DMA_CSELR_C2S_ADC ((uint32_t)0x00000000)
// Remap DMA channel 2 to SPI1 RX
#define DMA_CSELR_C2S_SPI1 ((uint32_t)0x00000010)
// Remap DMA channel 2 to USART1 TX
#define DMA_CSELR_C2S_USART1 ((uint32_t)0x00000030)
// Remap DMA channel 2 to LPUART1 TX
#define DMA_CSELR_C2S_LPUART1 ((uint32_t)0x00000050)
// Remap DMA channel 2 to I2C1 TX
#define DMA_CSELR_C2S_I2C1 ((uint32_t)0x00000060)
// Remap DMA channel 2 to TIM2 UP
#define DMA_CSELR_C2S_TIM2 ((uint32_t)0x00000080)
// Remap DMA channel 2 to DAC channel 1
#define DMA_CSELR_C2S_DAC1 ((uint32_t)0x00000090)
// Remap DMA channel 2 to AES output
#define DMA_CSELR_C2S_AES ((uint32_t)0x000000B0)
// Remap DMA channel 3 to SPI1 TX
#define DMA_CSELR_C3S_SPI1 ((uint32_t)0x00000100)
// Remap DMA channel 3 to USART1 RX
#define DMA_CSELR_C3S_USART1 ((uint32_t)0x00000300)
// Remap DMA channel 3 to LPUART1 RX
#define DMA_CSELR_C3S_LPUART1 ((uint32_t)0x00000500)
// Remap DMA channel 3 to I2C1 RX
#define DMA_CSELR_C3S_I2C1 ((uint32_t)0x00000600)
// Remap DMA channel 3 to TIM2 CH2
#define DMA_CSELR_C3S_TIM2 ((uint32_t)0x00000800)
// Remap DMA channel 3 to AES output
#define DMA_CSELR_C3S_AES ((uint32_t)0x00000B00)
// Remap DMA channel 4 to SPI2 RX
#define DMA_CSELR_C4S_SPI2 ((uint32_t)0x00002000)
// Remap DMA channel 4 to USART1 TX
#define DMA_CSELR_C4S_USART1 ((uint32_t)0x00003000)
// Remap DMA channel 4 to USART2 TX
#define DMA_CSELR_C4S_USART2 ((uint32_t)0x00004000)
// Remap DMA channel 4 to I2C2 TX
#define DMA_CSELR_C4S_I2C2 ((uint32_t)0x00007000)
// Remap DMA channel 4 to TIM2 CH4
#define DMA_CSELR_C4S_TIM2 ((uint32_t)0x00008000)
// Remap DMA channel 5 to SPI2 TX
#define DMA_CSELR_C5S_SPI2 ((uint32_t)0x00020000)
// Remap DMA channel 5 to USART1 RX
#define DMA_CSELR_C5S_USART1 ((uint32_t)0x00030000)
// Remap DMA channel 5 to USART2 RX
#define DMA_CSELR_C5S_USART2 ((uint32_t)0x00040000)
// Remap DMA channel 5 to I2C2 RX
#define DMA_CSELR_C5S_I2C2 ((uint32_t)0x00070000)
// Remap DMA channel 5 to TIM2 CH1
#define DMA_CSELR_C5S_TIM2 ((uint32_t)0x00080000)
// Remap DMA channel 6 to SPI2 RX
#define DMA_CSELR_C6S_SPI2 ((uint32_t)0x00200000)
// Remap DMA channel 6 to USART2 RX
#define DMA_CSELR_C6S_USART2 ((uint32_t)0x00400000)
// Remap DMA channel 6 to LPUART1 RX
#define DMA_CSELR_C6S_LPUART1 ((uint32_t)0x00500000)
// Remap DMA channel 6 to I2C1 TX
#define DMA_CSELR_C6S_I2C1 ((uint32_t)0x00600000)
// Remap DMA channel 7 to SPI2 TX
#define DMA_CSELR_C7S_SPI2 ((uint32_t)0x02000000)
// Remap DMA channel 7 to USART2 TX
#define DMA_CSELR_C7S_USART2 ((uint32_t)0x04000000)
// Remap DMA channel 7 to LPUART1 TX
#define DMA_CSELR_C7S_LPUART1 ((uint32_t)0x05000000)
// Remap DMA channel 7 to I2C1 RX
#define DMA_CSELR_C7S_I2C1 ((uint32_t)0x06000000)
// Remap DMA channel 7 to TIM2 CH2/CH4
#define DMA_CSELR_C7S_TIM2 ((uint32_t)0x08000000)

// Flags in the interrupt register
// GIF = any flag, TCIF = complete flag, TEIF = error flag, HTIF = halfway done flag
#define DMA_ISR_GIF1 ((uint32_t)0x00000001)
#define DMA_ISR_TCIF1 ((uint32_t)0x00000002)
#define DMA_ISR_HTIF1 ((uint32_t)0x00000004)
#define DMA_ISR_TEIF1 ((uint32_t)0x00000008)
#define DMA_ISR_GIF2 ((uint32_t)0x00000010)
#define DMA_ISR_TCIF2 ((uint32_t)0x00000020)
#define DMA_ISR_HTIF2 ((uint32_t)0x00000040)
#define DMA_ISR_TEIF2 ((uint32_t)0x00000080)
#define DMA_ISR_GIF3 ((uint32_t)0x00000100)
#define DMA_ISR_TCIF3 ((uint32_t)0x00000200)
#define DMA_ISR_HTIF3 ((uint32_t)0x00000400)
#define DMA_ISR_TEIF3 ((uint32_t)0x00000800)
#define DMA_ISR_GIF4 ((uint32_t)0x00001000)
#define DMA_ISR_TCIF4 ((uint32_t)0x00002000)
#define DMA_ISR_HTIF4 ((uint32_t)0x00004000)
#define DMA_ISR_TEIF4 ((uint32_t)0x00008000)
#define DMA_ISR_GIF5 ((uint32_t)0x00010000)
#define DMA_ISR_TCIF5 ((uint32_t)0x00020000)
#define DMA_ISR_HTIF5 ((uint32_t)0x00040000)
#define DMA_ISR_TEIF5 ((uint32_t)0x00080000)
#define DMA_ISR_GIF6 ((uint32_t)0x00100000)
#define DMA_ISR_TCIF6 ((uint32_t)0x00200000)
#define DMA_ISR_HTIF6 ((uint32_t)0x00400000)
#define DMA_ISR_TEIF6 ((uint32_t)0x00800000)
#define DMA_ISR_GIF7 ((uint32_t)0x01000000)
#define DMA_ISR_TCIF7 ((uint32_t)0x02000000)
#define DMA_ISR_HTIF7 ((uint32_t)0x04000000)
#define DMA_ISR_TEIF7 ((uint32_t)0x08000000)

// Clears flags in the interrupt register for a given channel
#define DMA_IFCR_CH1 ((uint32_t)0x0000000F)
#define DMA_IFCR_CH2 ((uint32_t)0x000000F0)
#define DMA_IFCR_CH3 ((uint32_t)0x00000F00)
#define DMA_IFCR_CH4 ((uint32_t)0x0000F000)
#define DMA_IFCR_CH5 ((uint32_t)0x000F0000)
#define DMA_IFCR_CH6 ((uint32_t)0x00F00000)
#define DMA_IFCR_CH7 ((uint32_t)0x0F000000)

// Flash memory defines
// Enable one wait state in FLASH
#define FLASH_ACR_LATENCY ((uint32_t)0x00000001)
// Prefetch buffer enable
#define FLASH_ACR_PRFTBE ((uint8_t)0x00000002)

// Lock data memory operations
#define FLASH_PECR_PELOCK ((uint32_t)0x00000001)
// Lock program memory operations
#define FLASH_PECR_PRGLOCK ((uint32_t)0x00000002)
// Lock option byte operations
#define FLASH_PECR_OPTLOCK ((uint32_t)0x00000004)
// Select flash memory for operation
#define FLASH_PECR_PROG ((uint32_t)0x00000008)
// Select data (EEPROM) memory for operation
#define FLASH_PECR_DATA ((uint32_t)0x00000010)
// Force erase before write enable
#define FLASH_PECR_FIX ((uint32_t)0x00000100)
// Request erase operation
#define FLASH_PECR_ERASE ((uint32_t)0x00000200)
// Half page programming enable
#define FLASH_PECR_FPRG ((uint32_t)0x00000400)
// End-of-programming interrupt enable
#define FLASH_PECR_EOPIE ((uint32_t)0x00010000)
// Error interrupt enable
#define FLASH_PECR_ERRIE ((uint32_t)0x00020000)
// Launch option bytes loader (causes a reset!)
#define FLASH_PECR_OBL_LAUNCH ((uint32_t)0x00040000)

// PEKEYR unlock sequence value #1
#define FLASH_PEKEYR_PEKEY1 ((uint32_t)0x89ABCDEF);
// PEKEYR unlock sequence value #2
#define FLASH_PEKEYR_PEKEY2 ((uint32_t)0x02030405);

// GPIO defines
// AFIO 0 selection
#define GPIO_AF0_SPI1 ((uint8_t)0x00)
#define GPIO_AF0_SPI2 ((uint8_t)0x00)
#define GPIO_AF0_USART1 ((uint8_t)0x00)
#define GPIO_AF0_USART2 ((uint8_t)0x00)
#define GPIO_AF0_LPUART1 ((uint8_t)0x00)
#define GPIO_AF0_USB ((uint8_t)0x00)
#define GPIO_AF0_LPTIM ((uint8_t)0x00)
#define GPIO_AF0_TSC ((uint8_t)0x00)
#define GPIO_AF0_TIM2 ((uint8_t)0x00)
#define GPIO_AF0_TIM21 ((uint8_t)0x00)
#define GPIO_AF0_TIM22 ((uint8_t)0x00)
#define GPIO_AF0_EVENTOUT ((uint8_t)0x00)
#define GPIO_AF0_MCO ((uint8_t)0x00)
#define GPIO_AF0_SWDIO ((uint8_t)0x00)
#define GPIO_AF0_SWCLK ((uint8_t)0x00)

// AFIO 1 selection
#define GPIO_AF1_SPI1 ((uint8_t)0x01)
#define GPIO_AF1_SPI2 ((uint8_t)0x01)
#define GPIO_AF1_I2C1 ((uint8_t)0x01)
#define GPIO_AF1_TIM2 ((uint8_t)0x01)
#define GPIO_AF1_TIM21 ((uint8_t)0x01)

// AFIO 2 selection
#define GPIO_AF2_SPI2 ((uint8_t)0x02)
#define GPIO_AF2_LPUART1 ((uint8_t)0x02)
#define GPIO_AF2_USB ((uint8_t)0x02)
#define GPIO_AF2_LPTIM ((uint8_t)0x02)
#define GPIO_AF2_TIM2 ((uint8_t)0x02)
#define GPIO_AF2_TIM22 ((uint8_t)0x02)
#define GPIO_AF2_EVENTOUT ((uint8_t)0x02)
#define GPIO_AF2_RTC_50Hz ((uint8_t)0x02)

// AFIO 3 selection
#define GPIO_AF3_I2C1 ((uint8_t)0x03)
#define GPIO_AF3_TSC ((uint8_t)0x03)
#define GPIO_AF3_EVENTOUT ((uint8_t)0x03)
#define GPIO_AF3_TIM3 ((uint8_t)0x03)

// AFIO 4 selection
#define GPIO_AF4_I2C1 ((uint8_t)0x04)
#define GPIO_AF4_USART1 ((uint8_t)0x04)
#define GPIO_AF4_USART2 ((uint8_t)0x04)
#define GPIO_AF4_LPUART1 ((uint8_t)0x04)
#define GPIO_AF4_TIM22 ((uint8_t)0x04)
#define GPIO_AF4_EVENTOUT ((uint8_t)0x04)

// AFIO 5 selection
#define GPIO_AF5_SPI2 ((uint8_t)0x05)
#define GPIO_AF5_I2C2 ((uint8_t)0x05)
#define GPIO_AF5_TIM2 ((uint8_t)0x05)
#define GPIO_AF5_TIM21 ((uint8_t)0x05)
#define GPIO_AF5_TIM22 ((uint8_t)0x05)

// AFIO 6 selection
#define GPIO_AF6_I2C2 ((uint8_t)0x06)
#define GPIO_AF6_TIM21 ((uint8_t)0x06)
#define GPIO_AF6_EVENTOUT ((uint8_t)0x06)

// AFIO 7 selection
#define GPIO_AF7_COMP1 ((uint8_t)0x07)
#define GPIO_AF7_COMP2 ((uint8_t)0x07)

// I2C defines
// I2C enable
#define I2C_CR1_PE ((uint32_t)0x00000001)
// Transmit interrupt enable
#define I2C_CR1_TXIE ((uint32_t)0x00000002)
// Receive interrupt enable
#define I2C_CR1_RXIE ((uint32_t)0x00000004)
// Address matched interrupt enable
#define I2C_CR1_ADDRIE ((uint32_t)0x00000008)
// No acknowledge interrupt enable
#define I2C_CR1_NACKIE ((uint32_t)0x00000010)
// STOP interrupt enable
#define I2C_CR1_STOPIE ((uint32_t)0x00000020)
// Transfer complete interrupt enable
#define I2C_CR1_TCIE ((uint32_t)0x00000040)
// Error interrupt enable
#define I2C_CR1_ERRIE ((uint32_t)0x00000080)
// Analog filter disable
#define I2C_CR1_ANFOFF ((uint32_t)0x00001000)
// Transmit DMA enable
#define I2C_CR1_TXDMAEN ((uint32_t)0x00004000)
// Receive DMA enable
#define I2C_CR1_RXDMAEN ((uint32_t)0x00008000)
// Slave byte control
#define I2C_CR1_SBC ((uint32_t)0x00010000)
// Clock stretching disable
#define I2C_CR1_NOSTRETCH ((uint32_t)0x00020000)
// Wakeup from STOP on address match enable
#define I2C_CR1_WUPEN ((uint32_t)0x00040000)
// General call enable
#define I2C_CR1_GCEN ((uint32_t)0x00080000)
// Packet error checking enable
#define I2C_CR1_PECEN ((uint32_t)0x00800000)

// Transfer direction selection (1 = read 0 = write)
#define I2C_CR2_RD_WRN ((uint32_t)0x00000400)
// 10-bit addressing enable
#define I2C_CR2_ADD10 ((uint32_t)0x00000800)
// Start-bit generation flag
#define I2C_CR2_START ((uint32_t)0x00002000)
// Stop-bit generation flag
#define I2C_CR2_STOP ((uint32_t)0x00000400)
// No-acknowledge enable flag (otherwise acknowledged)
#define I2C_CR2_NACK ((uint32_t)0x00000800)
// Automatically repeat send or receive
#define I2C_CR2_RELOAD ((uint32_t)0x01000000)
// Send STOP after completion (master mode)
#define I2C_CR2_AUTOEND ((uint32_t)0x02000000)

// Enable address #1
#define I2C_OAR1_OA1EN ((uint32_t)0x00008000)
// Use 10-bit mode for address #1
#define I2C_OAR1_OA1MODE ((uint32_t)0x00000400)
// Enable address #2
#define I2C_OAR2_OA2EN ((uint32_t)0x00008000)

// Transmit buffer empty flag
#define I2C_ISR_TXE ((uint32_t)0x00000001)
// Transmit interrupt status flag (meant for slave mode)
#define I2C_ISR_TXIS ((uint32_t)0x00000002)
// Receiver data full flag
#define I2C_ISR_RXNE ((uint32_t)0x00000004)
// Address matched flag (slave mode)
#define I2C_ISR_ADDR ((uint32_t)0x00000008)
// No acknowledge received flag
#define I2C_ISR_NACKF ((uint32_t)0x00000010)
// Stop detection flag
#define I2C_ISR_STOPF ((uint32_t)0x00000020)
// Transfer complete flag (for auto master transfers with NBYTES)
#define I2C_ISR_TC ((uint32_t)0x00000040)
// Transfer reload flag (for repeat master transfers with NBYTES)
#define I2C_ISR_TCR ((uint32_t)0x00000080)
// Bus error flag
#define I2C_ISR_BERR ((uint32_t)0x00000100)
// Arbitration lost flag
#define I2C_ISR_ARLO ((uint32_t)0x00000200)
// Overrun flag (slave mode)
#define I2C_ISR_OVR ((uint32_t)0x00000400)
// PEC error
#define I2C_ISR_PECERR ((uint32_t)0x00000800)
// Timeout error
#define I2C_ISR_TIMEOUT ((uint32_t)0x00001000)
// Bus busy flag
#define I2C_ISR_BUSY ((uint32_t)0x00008000)
// Read/write direction requested flag (slave mode)
#define I2C_ISR_DIR ((uint32_t)0x00010000)

// Clear address matched flag
#define I2C_ICR_ADDRCF ((uint32_t)0x00000008)
// Clear no-acknowledge flag
#define I2C_ICR_NACKCF ((uint32_t)0x00000010)
// Clear stop detection flag
#define I2C_ICR_STOPCF ((uint32_t)0x00000020)
// Clear bus error flag
#define I2C_ICR_BERRCF ((uint32_t)0x00000100)
// Clear arbitration lost flag
#define I2C_ICR_ARLOCF ((uint32_t)0x00000200)
// Clear overrun flag
#define I2C_ICR_OVRCF ((uint32_t)0x00000400)
// Clear PEC error flag
#define I2C_ICR_PECCF ((uint32_t)0x00000800)
// Clear timeout flag
#define I2C_ICR_TIMOUTCF ((uint32_t)0x00001000)

// Shift amount for I2C timing prescalar
#define I2C_TIMINGR_PRESC_S 28
// Shift amount for data setup time (SCL delay)
#define I2C_TIMINGR_SCLDEL_S 20
// Shift amount for data hold time (SDA delay)
#define I2C_TIMINGR_SDADEL_S 16
// SCL high period
#define I2C_TIMINGR_SCLH_S 8
// SCL low period
#define I2C_TIMINGR_SCLL_S 0

// IWDG defines
// Unlocks IWDG registers
#define IWDG_KR_UNLOCK ((uint32_t)0x00005555)
// Feeds the IWDG
#define IWDG_KR_FEED ((uint32_t)0x0000AAAA)
// Enables the IWDG
#define IWDG_KR_START ((uint32_t)0x0000CCCC)

// IWDG prescalar selection bits
#define IWDG_PR_PR ((uint32_t)0x00000007)
#define IWDG_PR_PR_DIV4 ((uint32_t)0x00000000)
#define IWDG_PR_PR_DIV8 ((uint32_t)0x00000001)
#define IWDG_PR_PR_DIV16 ((uint32_t)0x00000002)
#define IWDG_PR_PR_DIV32 ((uint32_t)0x00000003)
#define IWDG_PR_PR_DIV64 ((uint32_t)0x00000004)
#define IWDG_PR_PR_DIV128 ((uint32_t)0x00000005)
#define IWDG_PR_PR_DIV256 ((uint32_t)0x00000006)

// PWR defines
// Low power sleep mode enable
#define PWR_CR_LPSDSR ((uint32_t)0x00000001)
// Enter STOP mode on next sleep
#define PWR_CR_PDDS ((uint32_t)0x00000002)
// Clear wakeup flag
#define PWR_CR_CWUF ((uint32_t)0x00000004)
// Clear standby flag
#define PWR_CR_CSBF ((uint32_t)0x00000008)
// PVD enable
#define PWR_CR_PVDE ((uint32_t)0x00000010)
// PVD level selection bits
#define PWR_CR_PLS ((uint32_t)0x000000E0)
// Allow writes to RTC domain
#define PWR_CR_DBP ((uint32_t)0x00000100)
// Disable VREFINT in low power mode
#define PWR_CR_ULP ((uint32_t)0x00000200)
// Fast wake-up enable
#define PWR_CR_FWU ((uint32_t)0x00000400)
// Voltage selection bits
#define PWR_CR_VOS ((uint32_t)0x00001800)
#define PWR_CR_VOS_0 ((uint32_t)0x00000800)
#define PWR_CR_VOS_1 ((uint32_t)0x00001000)
// Low-power run enable
#define PWR_CR_LPRUN ((uint32_t)0x00004000)

// Wakeup occurred flag
#define PWR_CSR_WUF ((uint32_t)0x00000001)
// Standby mode entered flag
#define PWR_CSR_SBF ((uint32_t)0x00000002)
// PVD trigger flag
#define PWR_CSR_PVDO ((uint32_t)0x00000004)
// VREFINT ready flag
#define PWR_CSR_VREFINTRDYF ((uint32_t)0x00000008)
// Voltage regulator ready flag
#define PWR_CSR_VOSF ((uint32_t)0x00000010)

// RCC defines
// Enable HSI
#define RCC_CR_HSI16ON ((uint32_t)0x00000001)
// Enable HSI in stop mode
#define RCC_CR_HSI16KERON ((uint32_t)0x00000002)
// HSI ready?
#define RCC_CR_HSI16RDYF ((uint32_t)0x00000004)
// Divide HSI by 4
#define RCC_CR_HSI16DIVEN ((uint32_t)0x00000008)
// HSI divided by 4?
#define RCC_CR_HSI16DIVF ((uint32_t)0x00000010)
// Enable MSI
#define RCC_CR_MSION ((uint32_t)0x00000100)
// MSI ready?
#define RCC_CR_MSIRDY ((uint32_t)0x00000200)
// Enable HSE
#define RCC_CR_HSEON ((uint32_t)0x00010000)
// HSE ready?
#define RCC_CR_HSERDY ((uint32_t)0x00020000)
// Bypass HSE for external oscillator
#define RCC_CR_HSEBYP ((uint32_t)0x00040000)
// Enable PLL
#define RCC_CR_PLLON ((uint32_t)0x01000000)
// PLL ready?
#define RCC_CR_PLLRDY ((uint32_t)0x02000000)

// 48 MHz oscillator ready?
#define RCC_CRRCR_HSI48RDY ((uint32_t)0x00000002)
// Enable 48 MHz oscillator
#define RCC_CRRCR_HSI48ON ((uint32_t)0x00000001)

// Bits for system clock select
#define RCC_CFGR_SW ((uint32_t)0x00000003)
// Set HSI as system clock
#define RCC_CFGR_SW_HSI ((uint32_t)0x00000001)
// Set PLL as system clock
#define RCC_CFGR_SW_PLL ((uint32_t)0x00000003)
// Bits for system clock status
#define RCC_CFGR_SWS ((uint32_t)0x0000000C)
// HSI is used as system clock
#define RCC_CFGR_SWS_HSI ((uint32_t)0x00000004)
// PLL is used as system clock
#define RCC_CFGR_SWS_PLL ((uint32_t)0x0000000C)
// Bits for AHB clock divider (HCLK)
#define RCC_CFGR_HPRE ((uint32_t)0x000000F0)
// Bits for APB1 clock divider
#define RCC_CFGR_PPRE1 ((uint32_t)0x00000700)
// Bits for APB2 clock divider
#define RCC_CFGR_PPRE2 ((uint32_t)0x00003800)
// Bits for ADC clock divider
#define RCC_CFGR_ADCPRE ((uint32_t)0x0000C000)
// Bits for PLL multiplier
#define RCC_CFGR_PLLMUL ((uint32_t)0x003C0000)
// Bits for PLL divider
#define RCC_CFGR_PLLDIV ((uint32_t)0x00C00000)
// Multiply PLL by 3
#define RCC_CFGR_PLLMUL_3 ((uint32_t)0x00000000)
// Multiply PLL by 4
#define RCC_CFGR_PLLMUL_4 ((uint32_t)0x00040000)
// Multiply PLL by 8
#define RCC_CFGR_PLLMUL_8 ((uint32_t)0x000C0000)
// Divide PLL by 2
#define RCC_CFGR_PLLDIV_2 ((uint32_t)0x00400000)
// Divide PLL by 3
#define RCC_CFGR_PLLDIV_3 ((uint32_t)0x00800000)
// Divide PLL by 4
#define RCC_CFGR_PLLDIV_4 ((uint32_t)0x00C00000)
// Select HSI16 as clock when waking up from STOP
#define RCC_CFGR_STOPWUCK ((uint32_t)0x00008000)
// PLL source from HSE
#define RCC_CFGR_PLLSRC_HSE ((uint32_t)0x00010000)

// Reset I/O port A
#define RCC_IOPRSTR_IOPARST ((uint32_t)0x00000001)
// Reset I/O port B
#define RCC_IOPRSTR_IOPBRST ((uint32_t)0x00000002)
// Reset I/O port C
#define RCC_IOPRSTR_IOPCRST ((uint32_t)0x00000004)
// Reset I/O port D
#define RCC_IOPRSTR_IOPDRST ((uint32_t)0x00000008)
// Reset I/O port H
#define RCC_IOPRSTR_IOPHRST ((uint32_t)0x00000080)

// Reset DMA on AHB
#define RCC_AHBRSTR_DMARST ((uint32_t)0x00000001)
// Reset memory interface on AHB
#define RCC_AHBRSTR_MIFRST ((uint32_t)0x00000100)
// Reset CRC calculations on AHB
#define RCC_AHBRSTR_CRCRST ((uint32_t)0x00001000)
// Reset TSC on AHB
#define RCC_AHBRSTR_TSCRST ((uint32_t)0x00010000)
// Reset RNG on AHB
#define RCC_AHBRSTR_RNGRST ((uint32_t)0x00100000)
// Reset AES module on AHB
#define RCC_AHBRSTR_CRYPTRST ((uint32_t)0x01000000)

// Reset SYSCFG on APB2
#define RCC_APB2RSTR_SYSCFGRST ((uint32_t)0x00000001)
// Reset TIM21 on APB2
#define RCC_APB2RSTR_TIM21RST ((uint32_t)0x00000004)
// Reset TIM22 on APB2
#define RCC_APB2RSTR_TIM22RST ((uint32_t)0x00000020)
// Reset ADC on APB2
#define RCC_APB2RSTR_ADCRST ((uint32_t)0x00000200)
// Reset SPI1 on APB2
#define RCC_APB2RSTR_SPI1RST ((uint32_t)0x00001000)
// Reset USART1 on APB2
#define RCC_APB2RSTR_USART1RST ((uint32_t)0x00004000)

// Reset TIM2 on APB1
#define RCC_APB1RSTR_TIM2RST ((uint32_t)0x00000001)
// Reset TIM6 on APB1
#define RCC_APB1RSTR_TIM6RST ((uint32_t)0x00000010)
// Reset WWDG on APB1
#define RCC_APB1RSTR_WWDGRST ((uint32_t)0x00000400)
// Reset USART2 on APB1
#define RCC_APB1RSTR_USART2RST ((uint32_t)0x00020000)
// Reset SPI2 on APB1
#define RCC_APB1RSTR_SPI2RST ((uint32_t)0x00004000)
// Reset USART2 on APB1
#define RCC_APB1RSTR_USART2RST ((uint32_t)0x00020000)
// Reset LPUART1 on APB1
#define RCC_APB1RSTR_LPUART1RST ((uint32_t)0x00040000)
// Reset I2C1 on APB1
#define RCC_APB1RSTR_I2C1RST ((uint32_t)0x00200000)
// Reset I2C2 on APB1
#define RCC_APB1RSTR_I2C2RST ((uint32_t)0x00400000)
// Reset USB on APB1
#define RCC_APB1RSTR_USBRST ((uint32_t)0x00800000)
// Reset clock recovery system on APB1
#define RCC_APB1RSTR_CRSRST ((uint32_t)0x08000000)
// Reset PWR on APB1
#define RCC_APB1RSTR_PWRRST ((uint32_t)0x10000000)
// Reset DAC on APB1
#define RCC_APB1RSTR_DACRST ((uint32_t)0x20000000)
// Reset LPTIM1 on APB1
#define RCC_APB1RSTR_LPTIM1RST ((uint32_t)0x80000000)

// Enable I/O port A
#define RCC_IOPENR_IOPAEN ((uint32_t)0x00000001)
// Enable I/O port B
#define RCC_IOPENR_IOPBEN ((uint32_t)0x00000002)
// Enable I/O port C
#define RCC_IOPENR_IOPCEN ((uint32_t)0x00000004)
// Enable I/O port D
#define RCC_IOPENR_IOPDEN ((uint32_t)0x00000008)
// Enable I/O port H
#define RCC_IOPENR_IOPHEN ((uint32_t)0x00000080)

// Enable DMA on AHB
#define RCC_AHBENR_DMAEN ((uint32_t)0x00000001)
// Enable memory interface on AHB
#define RCC_AHBENR_MIFEN ((uint32_t)0x00000100)
// Enable CRC calculations on AHB
#define RCC_AHBENR_CRCEN ((uint32_t)0x00001000)
// Enable TSC on AHB
#define RCC_AHBENR_TSCEN ((uint32_t)0x00010000)
// Enable RNG on AHB
#define RCC_AHBENR_RNGEN ((uint32_t)0x00100000)
// Enable AES module on AHB
#define RCC_AHBENR_CRYPTEN ((uint32_t)0x01000000)

// Enable SYSCFG on APB2
#define RCC_APB2ENR_SYSCFGEN ((uint32_t)0x00000001)
// Enable TIM21 on APB2
#define RCC_APB2ENR_TIM21EN ((uint32_t)0x00000004)
// Enable TIM22 on APB2
#define RCC_APB2ENR_TIM22EN ((uint32_t)0x00000020)
// Enable ADC on APB2
#define RCC_APB2ENR_ADCEN ((uint32_t)0x00000200)
// Enable SPI1 on APB2
#define RCC_APB2ENR_SPI1EN ((uint32_t)0x00001000)
// Enable USART1 on APB2
#define RCC_APB2ENR_USART1EN ((uint32_t)0x00004000)

// Enable TIM2 on APB1
#define RCC_APB1ENR_TIM2EN ((uint32_t)0x00000001)
// Enable TIM6 on APB1
#define RCC_APB1ENR_TIM6EN ((uint32_t)0x00000010)
// Enable WWDG on APB1
#define RCC_APB1ENR_WWDGEN ((uint32_t)0x00000400)
// Enable USART2 on APB1
#define RCC_APB1ENR_USART2EN ((uint32_t)0x00020000)
// Enable SPI2 on APB1
#define RCC_APB1ENR_SPI2EN ((uint32_t)0x00004000)
// Enable USART2 on APB1
#define RCC_APB1ENR_USART2EN ((uint32_t)0x00020000)
// Enable LPUART1 on APB1
#define RCC_APB1ENR_LPUART1EN ((uint32_t)0x00040000)
// Enable I2C1 on APB1
#define RCC_APB1ENR_I2C1EN ((uint32_t)0x00200000)
// Enable I2C2 on APB1
#define RCC_APB1ENR_I2C2EN ((uint32_t)0x00400000)
// Enable USB on APB1
#define RCC_APB1ENR_USBEN ((uint32_t)0x00800000)
// Enable clock recovery system on APB1
#define RCC_APB1ENR_CRSEN ((uint32_t)0x08000000)
// Enable PWR on APB1
#define RCC_APB1ENR_PWREN ((uint32_t)0x10000000)
// Enable DAC on APB1
#define RCC_APB1ENR_DACEN ((uint32_t)0x20000000)
// Enable LPTIM1 on APB1
#define RCC_APB1ENR_LPTIM1EN ((uint32_t)0x80000000)

// Low-power reset flag
#define RCC_CSR_LPWRRSTF ((uint32_t)0x80000000)
// Window watchdog reset flag
#define RCC_CSR_WWDGRSTF ((uint32_t)0x40000000)
// Independent watchdog reset flag
#define RCC_CSR_IWDGRSTF ((uint32_t)0x20000000)
// Software reset flag
#define RCC_CSR_SFTRSTF ((uint32_t)0x10000000)
// Power on reset flag
#define RCC_CSR_PORRSTF ((uint32_t)0x08000000)
// Reset by !RESET! pin flag
#define RCC_CSR_PINRSTF ((uint32_t)0x04000000)
// Option byte reset flag
#define RCC_CSR_OBLRSTF ((uint32_t)0x02000000)
// Remove reset flags
#define RCC_CSR_RMVF ((uint32_t)0x01000000)
// RTC reset
#define RCC_CSR_RTCRST ((uint32_t)0x00080000)
// RTC enable
#define RCC_CSR_RTCEN ((uint32_t)0x00040000)
// RTC source select bits
#define RCC_CSR_RTCSEL ((uint32_t)0x00030000)
#define RCC_CSR_RTCSEL_LSE ((uint32_t)0x00010000)
#define RCC_CSR_RTCSEL_LSI ((uint32_t)0x00020000)
// LSE drive strength bits
#define RCC_CSR_LSEDRV ((uint32_t)0x00001800)
#define RCC_CSR_LSEDRV_0 ((uint32_t)0x00000800)
#define RCC_CSR_LSEDRV_1 ((uint32_t)0x00001000)
// LSE bypass
#define RCC_CSR_LSEBYP ((uint32_t)0x00000400)
// LSE ready?
#define RCC_CSR_LSERDY ((uint32_t)0x00000200)
// LSE enable
#define RCC_CSR_LSEON ((uint32_t)0x00000100)
// LSI ready?
#define RCC_CSR_LSIRDY ((uint32_t)0x00000002)
// LSI enable
#define RCC_CSR_LSION ((uint32_t)0x00000001)

// Selects the HSI 48 clock for USB
#define RCC_CCIPR_HSI48SEL ((uint32_t)0x04000000)
// Selects the APB clock for I2C1
#define RCC_CCIPR_I2C1SEL_APB ((uint32_t)0x00000000)
// Selects the system clock for I2C1
#define RCC_CCIPR_I2C1SEL_SYS ((uint32_t)0x00001000)
// Selects the HSI clock for I2C1
#define RCC_CCIPR_I2C1SEL_HSI ((uint32_t)0x00002000)

// RTC registers
// AM/PM (in 24 hour use AM)
#define RTC_TR_PM ((uint32_t)0x00400000)
#define RTC_TR_PM_S 22
// Hour tens (BCD)
#define RTC_TR_HT ((uint32_t)0x00300000)
#define RTC_TR_HT_S 20
// Hour ones (BCD)
#define RTC_TR_HU ((uint32_t)0x000F0000)
#define RTC_TR_HU_S 16
// Minute tens (BCD)
#define RTC_TR_MNT ((uint32_t)0x00007000)
#define RTC_TR_MNT_S 12
// Minute ones (BCD)
#define RTC_TR_MNU ((uint32_t)0x00000F00)
#define RTC_TR_MNU_S 8
// Second tens (BCD)
#define RTC_TR_ST ((uint32_t)0x00000070)
#define RTC_TR_ST_S 4
// Second ones (BCD)
#define RTC_TR_SU ((uint32_t)0x0000000F)
#define RTC_TR_SU_S 0

// Year tens (BCD)
#define RTC_DR_YT ((uint32_t)0x00F00000)
#define RTC_DR_YT_S 20
// Year ones (BCD)
#define RTC_DR_YU ((uint32_t)0x000F0000)
#define RTC_DR_YU_S 16
// Day of week: 1 = Monday, 7 = Sunday, 0 = not allowed
#define RTC_DR_WDU ((uint32_t)0x0000E000)
#define RTC_DR_WDU_S 13
#define RTC_DR_MONDAY ((uint32_t)1U << RTC_DR_WDU_S)
#define RTC_DR_TUESDAY ((uint32_t)2U << RTC_DR_WDU_S)
#define RTC_DR_WEDNESDAY ((uint32_t)3U << RTC_DR_WDU_S)
#define RTC_DR_THURSDAY ((uint32_t)4U << RTC_DR_WDU_S)
#define RTC_DR_FRIDAY ((uint32_t)5U << RTC_DR_WDU_S)
#define RTC_DR_SATURDAY ((uint32_t)6U << RTC_DR_WDU_S)
#define RTC_DR_SUNDAY ((uint32_t)7U << RTC_DR_WDU_S)
// Month tens (1 or 0...)
#define RTC_DR_MT ((uint32_t)0x00001000)
#define RTC_DR_MT_S 12
// Month ones (BCD)
#define RTC_DR_MU ((uint32_t)0x00000F00)
#define RTC_DR_MU_S 8
// Day of month tens (BCD)
#define RTC_DR_DT ((uint32_t)0x00000030)
#define RTC_DR_DT_S 4
// Day of month ones (BCD)
#define RTC_DR_DU ((uint32_t)0x0000000F)
#define RTC_DR_DU_S 0

// Calibration output enable
#define RTC_CR_COE ((uint32_t)0x00800000)
// Output signal selection bits on ALARM pin
#define RTC_CR_OSEL ((uint32_t)0x00600000)
// ALARM signal polarity
#define RTC_CR_POL ((uint32_t)0x00100000)
// Calibration output on RTC_CALIB
#define RTC_CR_COSEL ((uint32_t)0x00080000)
// Backup bit intended for DST
#define RTC_CR_BKP ((uint32_t)0x00040000)
// Subtracts one hour atomically (DST)
#define RTC_CR_SUB1H ((uint32_t)0x00020000)
// Adds one hour atomically (DST)
#define RTC_CR_ADD1H ((uint32_t)0x00010000)
// Timestamp interrupt enable
#define RTC_CR_TSIE ((uint32_t)0x00008000)
// Wake-up timer interrupt enable
#define RTC_CR_WUTIE ((uint32_t)0x00004000)
// Alarm B interrupt enable
#define RTC_CR_ALRBIE ((uint32_t)0x00002000)
// Alarm A interrupt enable
#define RTC_CR_ALRAIE ((uint32_t)0x00001000)
// Timestamp enable
#define RTC_CR_TSE ((uint32_t)0x00000800)
// Wake-up timer enable
#define RTC_CR_WUTE ((uint32_t)0x00000400)
// Alarm B enable
#define RTC_CR_ALRBE ((uint32_t)0x00000200)
// Alarm A enable
#define RTC_CR_ALRAE ((uint32_t)0x00000100)
// Time format (0 = 24 hour, 1 = AM/PM)
#define RTC_CR_FMT ((uint32_t)0x00000040)
// Bypass shadow registers
#define RTC_CR_BYPSHAD ((uint32_t)0x00000020)
// 50/60 Hz reference clock detect enable
#define RTC_CR_REFCKON ((uint32_t)0x00000010)
// Timestamp edge detection mode on TS pin
#define RTC_CR_TSEDGE ((uint32_t)0x00000008)
// Wake-up clock selection bits
#define RTC_CR_WUCKSEL ((uint32_t)0x00000007)
// RTCCLK/16 selected as wake-up clock
#define RTC_CR_WUCKSEL_16 ((uint32_t)0x00000000)
// RTCCLK/8 selected as wake-up clock
#define RTC_CR_WUCKSEL_8 ((uint32_t)0x00000001)
// RTCCLK/4 selected as wake-up clock
#define RTC_CR_WUCKSEL_4 ((uint32_t)0x00000002)
// RTCCLK/2 selected as wake-up clock
#define RTC_CR_WUCKSEL_2 ((uint32_t)0x00000003)
// ck_spre (usually 1 Hz) selected as wake-up clock
#define RTC_CR_WUCKSEL_SPRE ((uint32_t)0x00000004)
// ck_spre (usually 1 Hz) selected as wake-up clock with 65536 added to WUTR value
#define RTC_CR_WUCKSEL_SPRE_16 ((uint32_t)0x00000006)

// Recalibration pending flag
#define RTC_ISR_RECALPF ((uint32_t)0x00010000)
// Tamper detect on TAMP2 input flag
#define RTC_ISR_TAMP2F ((uint32_t)0x00004000)
// Tamper detect on TAMP1 input flag
#define RTC_ISR_TAMP1F ((uint32_t)0x00002000)
// Timestamp overflow (multiple TS events occurred rapidly) flag
#define RTC_ISR_TSOVF ((uint32_t)0x00001000)
// Timestamp flag
#define RTC_ISR_TSF ((uint32_t)0x00000800)
// Wake-up timer flag
#define RTC_ISR_WUTF ((uint32_t)0x00000400)
// Alarm B flag
#define RTC_ISR_ALRBF ((uint32_t)0x00000200)
// Alarm A flag
#define RTC_ISR_ALRAF ((uint32_t)0x00000100)
// Initialization mode enable
#define RTC_ISR_INIT ((uint32_t)0x00000080)
// Initialization mode flag
#define RTC_ISR_INITF ((uint32_t)0x00000040)
// Register synchronization flag
#define RTC_ISR_RSF ((uint32_t)0x00000020)
// Initialization status flag
#define RTC_ISR_INITS ((uint32_t)0x00000010)
// Shift operation pending flag
#define RTC_ISR_SHPF ((uint32_t)0x00000008)
// Wake-up timer write enabled flag
#define RTC_ISR_WUTWF ((uint32_t)0x00000004)
// Alarm B write enabled flag
#define RTC_ISR_ALRBWF ((uint32_t)0x00000002)
// Alarm A write enabled flag
#define RTC_ISR_ALRAWF ((uint32_t)0x00000001)
// Writing this value to ISR will not clear any flags but leaves the reserved bits the same
#define RTC_ISR_NOP (RTC_ISR_RSF | RTC_ISR_ALRAF | RTC_ISR_ALRBF | RTC_ISR_WUTF | RTC_ISR_TSF |\
	RTC_ISR_TSOVF | RTC_ISR_TAMP1F | RTC_ISR_TAMP2F)

// Asynchronous prescaler flag bits
#define RTC_PRER_PREDIV_A ((uint32_t)0x007F0000)
// Synchronous prescaler flag bits
#define RTC_PRER_PREDIV_S ((uint32_t)0x00007FFF)

// Ignore date in comparison?
#define RTC_ALRMAR_MSK4 ((uint32_t)0x80000000)
// 0 to use day of month, 1 to use day of week
#define RTC_ALRMAR_WDSEL ((uint32_t)0x40000000)
// Date tens (BCD)
#define RTC_ALRMAR_DT ((uint32_t)0x30000000)
// Date ones (BCD)
#define RTC_ALRMAR_DU ((uint32_t)0x0F000000)
// Ignore hours in comparison?
#define RTC_ALRMAR_MSK3 ((uint32_t)0x00800000)
// AM/PM or 0 for 24 hour
#define RTC_ALRMAR_PM ((uint32_t)0x00400000)
// Hour tens (BCD)
#define RTC_ALRMAR_HT ((uint32_t)0x00300000)
// Hour ones (BCD)
#define RTC_ALRMAR_HU ((uint32_t)0x000F0000)
// Ignore minutes in comparison?
#define RTC_ALRMAR_MSK2 ((uint32_t)0x00008000)
// Minute tens (BCD)
#define RTC_ALRMAR_MNT ((uint32_t)0x00007000)
// Minute ones (BCD)
#define RTC_ALRMAR_MNU ((uint32_t)0x00000F00)
// Ignore seconds in comparison?
#define RTC_ALRMAR_MSK1 ((uint32_t)0x00000080)
// Second tens (BCD)
#define RTC_ALRMAR_ST ((uint32_t)0x00000070)
// Second ones (BCD)
#define RTC_ALRMAR_SU ((uint32_t)0x0000000F)

// Ignore date in comparison?
#define RTC_ALRMBR_MSK4 ((uint32_t)0x80000000)
// 0 to use day of month, 1 to use day of week
#define RTC_ALRMBR_WDSEL ((uint32_t)0x40000000)
// Date tens (BCD)
#define RTC_ALRMBR_DT ((uint32_t)0x30000000)
// Date ones (BCD)
#define RTC_ALRMBR_DU ((uint32_t)0x0F000000)
// Ignore hours in comparison?
#define RTC_ALRMBR_MSK3 ((uint32_t)0x00800000)
// AM/PM or 0 for 24 hour
#define RTC_ALRMBR_PM ((uint32_t)0x00400000)
// Hour tens (BCD)
#define RTC_ALRMBR_HT ((uint32_t)0x00300000)
// Hour ones (BCD)
#define RTC_ALRMBR_HU ((uint32_t)0x000F0000)
// Ignore minutes in comparison?
#define RTC_ALRMBR_MSK2 ((uint32_t)0x00008000)
// Minute tens (BCD)
#define RTC_ALRMBR_MNT ((uint32_t)0x00007000)
// Minute ones (BCD)
#define RTC_ALRMBR_MNU ((uint32_t)0x00000F00)
// Ignore seconds in comparison?
#define RTC_ALRMBR_MSK1 ((uint32_t)0x00000080)
// Second tens (BCD)
#define RTC_ALRMBR_ST ((uint32_t)0x00000070)
// Second ones (BCD)
#define RTC_ALRMBR_SU ((uint32_t)0x0000000F)

// Bits for subtracting a fraction of a second atomically
#define RTC_SHIFTR_SUBFS ((uint32_t)0x00007FFF)
// Bit for adding one second atomically
#define RTC_SHIFTR_ADD1S ((uint32_t)0x80000000)

// AM/PM (in 24 hour use AM)
#define RTC_TSTR_PM ((uint32_t)0x00400000)
// Hour tens (BCD)
#define RTC_TSTR_HT ((uint32_t)0x00300000)
// Hour ones (BCD)
#define RTC_TSTR_HU ((uint32_t)0x000F0000)
// Minute tens (BCD)
#define RTC_TSTR_MNT ((uint32_t)0x00007000)
// Minute ones (BCD)
#define RTC_TSTR_MNU ((uint32_t)0x00000F00)
// Second tens (BCD)
#define RTC_TSTR_ST ((uint32_t)0x00000070)
// Second ones (BCD)
#define RTC_TSTR_SU ((uint32_t)0x0000000F)

// Year tens (BCD)
#define RTC_TSDR_YT ((uint32_t)0x00F00000)
// Year ones (BCD)
#define RTC_TSDR_YU ((uint32_t)0x000F0000)
// Day of week: 1 = Monday, 7 = Sunday, 0 = not allowed
#define RTC_TSDR_WDU ((uint32_t)0x0000E000)
// Month tens (1 or 0...)
#define RTC_TSDR_MT ((uint32_t)0x00001000)
// Month ones (BCD)
#define RTC_TSDR_MU ((uint32_t)0x00000F00)
// Day of month tens (BCD)
#define RTC_TSDR_DT ((uint32_t)0x00000030)
// Day of month ones (BCD)
#define RTC_TSDR_DU ((uint32_t)0x0000000F)

// Calibration plus flag
#define RTC_CAL_CALP ((uint32_t)0x00008000)
// 8-second calibration cycle period enable
#define RTC_CAL_CALW8 ((uint32_t)0x00004000)
// 16-second calibration cycle period enable
#define RTC_CAL_CALW16 ((uint32_t)0x00002000)
// Calibration minus bits
#define RTC_CAL_CALM ((uint32_t)0x000001FF)

// Tamper input 2 mask flag
#define RTC_TAMPCR_TAMP2MF ((uint32_t)0x00200000)
// Tamper input 2 backup register preserve
#define RTC_TAMPCR_TAMP2NOERASE ((uint32_t)0x00100000)
// Tamper input 2 interrupt enable
#define RTC_TAMPCR_TAMP2IE ((uint32_t)0x00080000)
// Tamper input 1 mask flag
#define RTC_TAMPCR_TAMP1MF ((uint32_t)0x00040000)
// Tamper input 1 backup register preserve
#define RTC_TAMPCR_TAMP1NOERASE ((uint32_t)0x00020000)
// Tamper input 1 interrupt enable
#define RTC_TAMPCR_TAMP1IE ((uint32_t)0x00010000)
// Disable input pull up precharge of tamper pins prior to sampling
#define RTC_TAMPCR_TAMPPUDIS ((uint32_t)0x00008000)
// Tamper precharge duration bits
#define RTC_TAMPCR_TAMPPRCH ((uint32_t)0x00006000)
// Tamper filter selection bits
#define RTC_TAMPCR_TAMPFLT ((uint32_t)0x00001800)
// Tamper event sampling frequency bits
#define RTC_TAMPCR_TAMPFREQ ((uint32_t)0x00000700)
// Save timestamp on tamper event enable
#define RTC_TAMPCR_TAMPTS ((uint32_t)0x00000080)
// Level select for tamper input 2
#define RTC_TAMPCR_TAMP2TRG ((uint32_t)0x00000010)
// Tamper input 2 enable
#define RTC_TAMPCR_TAMP2E ((uint32_t)0x00000008)
// Tamper interrupt overall enable
#define RTC_TAMPCR_TAMPIE ((uint32_t)0x00000004)
// Level select for tamper input 1
#define RTC_TAMPCR_TAMP1TRG ((uint32_t)0x00000002)
// Tamper input 1 enable
#define RTC_TAMPCR_TAMP1E ((uint32_t)0x00000001)

// Writing these values in sequence enables write access to RTC registers
#define RTC_WPR_INIT1 ((uint32_t)0x000000CA)
#define RTC_WPR_INIT2 ((uint32_t)0x00000053)
// Writing this value re-enables protection on RTC registers
#define RTC_WPR_LOCK ((uint32_t)0x000000FF)

// SCB defines
// Request reset
#define SCB_AIRCR_RESET ((uint32_t)0x00000004)
// Key to allow mutation of application interrupt register
#define SCB_AIRCR_VECTKEY ((uint32_t)0x05FA0000)

// PendSV set
#define SCB_ICSR_PENDSV ((uint32_t)0x10000000)

// Set event on pending masked ISR
#define SCB_SCR_SEVONPEND ((uint32_t)0x00000010)
// Deep sleep enable flag
#define SCB_SCR_SLEEPDEEP ((uint32_t)0x00000004)
// Enable sleep on exit from current ISR
#define SCB_SCR_SLEEPONEXIT ((uint32_t)0x00000002)

// SPI defines
// CPHA = 1 (even edges)
#define SPI_CR1_CPHA_1 ((uint32_t)0x0001)
// CPOL = 1 (active low clock)
#define SPI_CR1_CPOL_1 ((uint32_t)0x0002)
// Baud rate PCLK/2
#define SPI_CR1_DIV2 ((uint32_t)0x0000)
// Baud rate PCLK/4
#define SPI_CR1_DIV4 ((uint32_t)0x0008)
// Baud rate PCLK/8
#define SPI_CR1_DIV8 ((uint32_t)0x0010)
// Baud rate PCLK/16
#define SPI_CR1_DIV16 ((uint32_t)0x0018)
// Baud rate PCLK/32
#define SPI_CR1_DIV32 ((uint32_t)0x0020)
// Baud rate PCLK/64
#define SPI_CR1_DIV64 ((uint32_t)0x0028)
// Baud rate PCLK/128
#define SPI_CR1_DIV128 ((uint32_t)0x0030)
// Baud rate PCLK/256
#define SPI_CR1_DIV256 ((uint32_t)0x0038)
// Least significant bit first
#define SPI_CR1_LSBFIRST ((uint32_t)0x0080)
// SPI enable
#define SPI_CR1_SPE ((uint32_t)0x0040)
// SPI master mode
#define SPI_CR1_MSTR ((uint32_t)0x0104)
// Software slave selection
#define SPI_CR1_NSS_SOFT ((uint32_t)0x0200)
// 16-bit data size
#define SPI_CR1_16BIT ((uint32_t)0x0800)

// Receive DMA enable
#define SPI_CR2_RXDMAEN ((uint32_t)0x0001)
// Transmit DMA enable
#define SPI_CR2_TXDMAEN ((uint32_t)0x0002)
// Receive buffer full interrupt enable
#define SPI_CR2_RXNEIE ((uint32_t)0x0040)
// Transmit buffer empty interrupt enable
#define SPI_CR2_TXEIE ((uint32_t)0x0080)

// Transmit data empty flag
#define SPI_SR_TXE ((uint32_t)0x0002)
// Receive buffer full flag
#define SPI_SR_RXNE ((uint32_t)0x0001)

// SYSCFG defines
// Turn on VREFINT
#define SYSCFG_CFGR3_EN_VREFINT ((uint32_t)0x00000001)
// Enable VREFINT buffer for ADC
#define SYSCFG_CFGR3_ENBUF_VREFINT_ADC ((uint32_t)0x00000100)
// Enable VREFINT buffer for temperature sensor
#define SYSCFG_CFGR3_ENBUF_SENSOR_ADC ((uint32_t)0x00000200)
// Enable VREFINT buffer for comparator 2
#define SYSCFG_CFGR3_ENBUF_VREFINT_COMP ((uint32_t)0x00001000)
// Enable VREFINT buffer for HSI 48
#define SYSCFG_CFGR3_ENREF_HSI48 ((uint32_t)0x00002000)
// VREFINT buffer for HSI 48 ready?
#define SYSCFG_CFGR3_REF_HSI48_RDYF ((uint32_t)0x04000000)
// VREFINT buffer for temperature sensor ready?
#define SYSCFG_CFGR3_SENSOR_ADC_RDYF ((uint32_t)0x08000000)
// VREFINT buffer for ADC ready?
#define SYSCFG_CFGR3_VREFINT_ADC_RDYF ((uint32_t)0x10000000)
// VREFINT buffer for comparator 2 ready?
#define SYSCFG_CFGR3_VREFINT_COMP_RDYF ((uint32_t)0x20000000)
// VREFINT ready?
#define SYSCFG_CFGR3_VREFINT_RDYF ((uint32_t)0x40000000)
// Lock CFGR3
#define SYSCFG_CFGR3_REF_LOCK ((uint32_t)0x80000000)

// SysTick defines
// SysTick enable
#define SysTick_CTRL_ENABLE ((uint32_t)0x00000001)
// SysTick interrupt enable
#define SysTick_CTRL_TICKINT ((uint32_t)0x00000002)
// SysTick clock source
#define SysTick_CTRL_CLKSOURCE ((uint32_t)0x0000004)

// Timer defines
// Auto reload preload enable
#define TIM_CR1_ARPE ((uint32_t)0x00000080)
// Count enable [run timer]
#define TIM_CR1_CEN ((uint32_t)0x00000001)

// Master mode selection to trigger on update [act as prescaler]
#define TIM_CR2_MMS_UPDATE ((uint32_t)0x00000020)

// Slave mode selection to count on trigger
#define TIM_SMCR_SMS_EXTERNAL ((uint32_t)0x00000007)
// Select internal trigger #0
#define TIM_SMCR_TS_ITR0 ((uint32_t)0x00000000)
// Select internal trigger #1
#define TIM_SMCR_TS_ITR1 ((uint32_t)0x00000010)
// Select internal trigger #2
#define TIM_SMCR_TS_ITR2 ((uint32_t)0x00000020)
// Select internal trigger #3
#define TIM_SMCR_TS_ITR3 ((uint32_t)0x00000030)

// Update interrupt enable
#define TIM_DIER_UIE ((uint32_t)0x00000001)
// CC1 interrupt enable
#define TIM_DIER_CC1IE ((uint32_t)0x00000002)
// CC2 interrupt enable
#define TIM_DIER_CC2IE ((uint32_t)0x00000004)
// CC3 interrupt enable
#define TIM_DIER_CC3IE ((uint32_t)0x00000008)
// CC4 interrupt enable
#define TIM_DIER_CC4IE ((uint32_t)0x00000010)
// Update DMA enable
#define TIM_DIER_UDE ((uint32_t)0x00000100)
// CC1 DMA enable
#define TIM_DIER_CC1DE ((uint32_t)0x00000200)
// CC2 DMA enable
#define TIM_DIER_CC2DE ((uint32_t)0x00000400)
// CC3 DMA enable
#define TIM_DIER_CC3DE ((uint32_t)0x00000800)
// CC4 DMA enable
#define TIM_DIER_CC4DE ((uint32_t)0x00001000)

// Update interrupt pending
#define TIM_SR_UIF ((uint32_t)0x00000001)
// CC1 interrupt pending
#define TIM_SR_CC1IF ((uint32_t)0x00000002)
// CC2 interrupt pending
#define TIM_SR_CC2IF ((uint32_t)0x00000004)
// CC3 interrupt pending
#define TIM_SR_CC3IF ((uint32_t)0x00000008)
// CC4 interrupt pending
#define TIM_SR_CC4IF ((uint32_t)0x00000010)

// Generate update
#define TIM_EGR_UG ((uint32_t)0x00000001)
// Generate compare match channel 1
#define TIM_EGR_CC1G ((uint32_t)0x00000002)
// Generate compare match channel 2
#define TIM_EGR_CC2G ((uint32_t)0x00000004)
// Generate compare match channel 3
#define TIM_EGR_CC3G ((uint32_t)0x00000008)
// Generate compare match channel 4
#define TIM_EGR_CC4G ((uint32_t)0x00000010)
// Generate trigger
#define TIM_EGR_TG ((uint32_t)0x00000040)

// Capture/compare channel 1 mode select
#define TIM_CCMR1_CC1S ((uint32_t)0x00000003)
// Set Capture/compare channel 1 as output
#define TIM_CCMR1_CC1S_OUT ((uint32_t)0x00000000)
// Set Capture/compare channel 1 as input on TI1
#define TIM_CCMR1_CC1S_TI1 ((uint32_t)0x00000001)
// Set Capture/compare channel 1 as input on TI2
#define TIM_CCMR1_CC1S_TI2 ((uint32_t)0x00000002)
// Set Capture/compare channel 1 as input on trigger
#define TIM_CCMR1_CC1S_TRC ((uint32_t)0x00000003)
// Capture/compare channel 1 fast enable
#define TIM_CCMR1_OC1FE ((uint32_t)0x00000004)
// Capture/compare channel 1 preload enable
#define TIM_CCMR1_OC1PE ((uint32_t)0x00000008)
// Capture/compare channel 1 output compare mode (OC1REF)
#define TIM_CCMR1_OC1M ((uint32_t)0x00000070)
// Disable OC1REF generation
#define TIM_CCMR1_OC1M_FROZEN ((uint32_t)0x00000000)
// Set OC1REF active one time when match occurs
#define TIM_CCMR1_OC1M_SETACT ((uint32_t)0x00000010)
// Set OC1REF inactive one time when match occurs
#define TIM_CCMR1_OC1M_SETINA ((uint32_t)0x00000020)
// Toggle OC1REF whenever match occurs
#define TIM_CCMR1_OC1M_TOGGLE ((uint32_t)0x00000030)
// Force OC1REF inactive
#define TIM_CCMR1_OC1M_INACTIVE ((uint32_t)0x00000040)
// Force OC1REF active
#define TIM_CCMR1_OC1M_ACTIVE ((uint32_t)0x00000050)
// Set PWM mode 1 on OC1REF
#define TIM_CCMR1_OC1M_PWM1 ((uint32_t)0x00000060)
// Set PWM mode 2 on OC1REF
#define TIM_CCMR1_OC1M_PWM2 ((uint32_t)0x00000070)
// Capture/compare channel 1 output clear enable
#define TIM_CCMR1_OC1CE ((uint32_t)0x00000080)
// Capture/compare channel 2 mode select
#define TIM_CCMR1_CC2S ((uint32_t)0x00000300)
// Set Capture/compare channel 2 as output
#define TIM_CCMR1_CC2S_OUT ((uint32_t)0x00000000)
// Set Capture/compare channel 2 as input on TI2
#define TIM_CCMR1_CC2S_TI2 ((uint32_t)0x00000100)
// Set Capture/compare channel 2 as input on TI1
#define TIM_CCMR1_CC2S_TI1 ((uint32_t)0x00000200)
// Set Capture/compare channel 2 as input on trigger
#define TIM_CCMR1_CC2S_TRC ((uint32_t)0x00000300)
// Capture/compare channel 2 fast enable
#define TIM_CCMR1_OC2FE ((uint32_t)0x00000400)
// Capture/compare channel 2 preload enable
#define TIM_CCMR1_OC2PE ((uint32_t)0x00000800)
// Capture/compare channel 2 output compare mode (OC2REF)
#define TIM_CCMR1_OC2M ((uint32_t)0x00007000)
// Disable OC2REF generation
#define TIM_CCMR1_OC2M_FROZEN ((uint32_t)0x00000000)
// Set OC2REF active one time when match occurs
#define TIM_CCMR1_OC2M_SETACT ((uint32_t)0x00001000)
// Set OC2REF inactive one time when match occurs
#define TIM_CCMR1_OC2M_SETINA ((uint32_t)0x00002000)
// Toggle OC2REF whenever match occurs
#define TIM_CCMR1_OC2M_TOGGLE ((uint32_t)0x00003000)
// Force OC2REF inactive
#define TIM_CCMR1_OC2M_INACTIVE ((uint32_t)0x00004000)
// Force OC2REF active
#define TIM_CCMR1_OC2M_ACTIVE ((uint32_t)0x00005000)
// Set PWM mode 1 on OC2REF
#define TIM_CCMR1_OC2M_PWM1 ((uint32_t)0x00006000)
// Set PWM mode 2 on OC2REF
#define TIM_CCMR1_OC2M_PWM2 ((uint32_t)0x00007000)
// Capture/compare channel 2 output clear enable
#define TIM_CCMR1_OC2CE ((uint32_t)0x00008000)

// Capture/compare channel 3 mode select
#define TIM_CCMR2_CC3S ((uint32_t)0x00000003)
// Set Capture/compare channel 3 as output
#define TIM_CCMR2_CC3S_OUT ((uint32_t)0x00000000)
// Set Capture/compare channel 3 as input on TI1
#define TIM_CCMR2_CC3S_TI1 ((uint32_t)0x00000001)
// Set Capture/compare channel 3 as input on TI2
#define TIM_CCMR2_CC3S_TI2 ((uint32_t)0x00000002)
// Set Capture/compare channel 3 as input on trigger
#define TIM_CCMR2_CC3S_TRC ((uint32_t)0x00000003)
// Capture/compare channel 3 fast enable
#define TIM_CCMR2_OC3FE ((uint32_t)0x00000004)
// Capture/compare channel 3 preload enable
#define TIM_CCMR2_OC3PE ((uint32_t)0x00000008)
// Capture/compare channel 3 output compare mode (OC3REF)
#define TIM_CCMR2_OC3M ((uint32_t)0x00000070)
// Disable OC3REF generation
#define TIM_CCMR2_OC3M_FROZEN ((uint32_t)0x00000000)
// Set OC3REF active one time when match occurs
#define TIM_CCMR2_OC3M_SETACT ((uint32_t)0x00000010)
// Set OC3REF inactive one time when match occurs
#define TIM_CCMR2_OC3M_SETINA ((uint32_t)0x00000020)
// Toggle OC3REF whenever match occurs
#define TIM_CCMR2_OC3M_TOGGLE ((uint32_t)0x00000030)
// Force OC3REF inactive
#define TIM_CCMR2_OC3M_INACTIVE ((uint32_t)0x00000040)
// Force OC3REF active
#define TIM_CCMR2_OC3M_ACTIVE ((uint32_t)0x00000050)
// Set PWM mode 1 on OC3REF
#define TIM_CCMR2_OC3M_PWM1 ((uint32_t)0x00000060)
// Set PWM mode 2 on OC3REF
#define TIM_CCMR2_OC3M_PWM2 ((uint32_t)0x00000070)
// Capture/compare channel 3 output clear enable
#define TIM_CCMR2_OC3CE ((uint32_t)0x00000080)
// Capture/compare channel 4 mode select
#define TIM_CCMR2_CC4S ((uint32_t)0x00000300)
// Set Capture/compare channel 4 as output
#define TIM_CCMR2_CC4S_OUT ((uint32_t)0x00000000)
// Set Capture/compare channel 4 as input on TI2
#define TIM_CCMR2_CC4S_TI2 ((uint32_t)0x00000100)
// Set Capture/compare channel 4 as input on TI1
#define TIM_CCMR2_CC4S_TI1 ((uint32_t)0x00000200)
// Set Capture/compare channel 4 as input on trigger
#define TIM_CCMR2_CC4S_TRC ((uint32_t)0x00000300)
// Capture/compare channel 4 fast enable
#define TIM_CCMR2_OC4FE ((uint32_t)0x00000400)
// Capture/compare channel 4 preload enable
#define TIM_CCMR2_OC4PE ((uint32_t)0x00000800)
// Capture/compare channel 4 output compare mode (OC4REF)
#define TIM_CCMR2_OC4M ((uint32_t)0x00007000)
// Disable OC4REF generation
#define TIM_CCMR2_OC4M_FROZEN ((uint32_t)0x00000000)
// Set OC4REF active one time when match occurs
#define TIM_CCMR2_OC4M_SETACT ((uint32_t)0x00001000)
// Set OC4REF inactive one time when match occurs
#define TIM_CCMR2_OC4M_SETINA ((uint32_t)0x00002000)
// Toggle OC4REF whenever match occurs
#define TIM_CCMR2_OC4M_TOGGLE ((uint32_t)0x00003000)
// Force OC4REF inactive
#define TIM_CCMR2_OC4M_INACTIVE ((uint32_t)0x00004000)
// Force OC4REF active
#define TIM_CCMR2_OC4M_ACTIVE ((uint32_t)0x00005000)
// Set PWM mode 1 on OC4REF
#define TIM_CCMR2_OC4M_PWM1 ((uint32_t)0x00006000)
// Set PWM mode 2 on OC4REF
#define TIM_CCMR2_OC4M_PWM2 ((uint32_t)0x00007000)
// Capture/compare channel 4 output clear enable
#define TIM_CCMR1_OC4CE ((uint32_t)0x00008000)

// Capture/compare I/O 1 enable
#define TIM_CCER_CC1E ((uint32_t)0x00000001)
// Capture/compare I/O 1 polarity
#define TIM_CCER_CC1P ((uint32_t)0x00000002)
// Capture/compare inverted I/O 1 polarity
#define TIM_CCER_CC1NP ((uint32_t)0x00000008)
// Capture/compare I/O 2 enable
#define TIM_CCER_CC2E ((uint32_t)0x00000010)
// Capture/compare I/O 2 polarity
#define TIM_CCER_CC2P ((uint32_t)0x00000020)
// Capture/compare inverted I/O 2 polarity
#define TIM_CCER_CC2NP ((uint32_t)0x00000080)

// USART defines
// Read data register full flag
#define USART_ISR_RXNE ((uint32_t)0x0020)
// Transmit data register empty flag
#define USART_ISR_TXE ((uint32_t)0x0080)

// USART enable flag
#define USART_CR1_UE ((uint32_t)0x0001)
// Reciever enable flag
#define USART_CR1_RE ((uint32_t)0x0004)
// Transmitter enable flag
#define USART_CR1_TE ((uint32_t)0x0008)
// Receive interrupt enable flag
#define USART_CR1_RXNEIE ((uint32_t)0x0020)
// Transmit data register empty interrupt enable flag
#define USART_CR1_TXEIE ((uint32_t)0x0080)
// Parity selection
#define USART_CR1_PS ((uint32_t)0x0200)
// Parity control
#define USART_CR1_PCE ((uint32_t)0x0400)
// Word length flag
#define USART_CR1_M ((uint32_t)0x1000)

// Last bit-clock send flag
#define USART_CR2_LBCL ((uint32_t)0x0100)
// Clock phase
#define USART_CR2_CPHA ((uint32_t)0x0200)
// Clock polarity
#define USART_CR2_CPOL ((uint32_t)0x0400)
// Clock enable
#define USART_CR2_CLKEN ((uint32_t)0x0800)
// Stop bit 0
#define USART_CR2_STOP0 ((uint32_t)0x1000)
// Stop bit 1
#define USART_CR2_STOP1 ((uint32_t)0x2000)

// Flow control RTS enable
#define USART_CR3_RTSE ((uint32_t)0x0100)
// Flow control CTS enable
#define USART_CR3_CTSE ((uint32_t)0x0200)

// USB defines
// Endpoint correct transfer RX
#define USB_EP_CTR_RX 0x8000
// Endpoint data toggle RX
#define USB_EP_DTOG_RX 0x4000
// Endpoint RX status mask
#define USB_EP_STAT_RX_MASK 0x3000
// Endpoint RX disabled
#define USB_EP_STAT_RX_DISABLED 0x0000
// Endpoint RX stalled
#define USB_EP_STAT_RX_STALL 0x1000
// Endpoint RX no acknowledge
#define USB_EP_STAT_RX_NAK 0x2000
// Endpoint RX valid
#define USB_EP_STAT_RX_VALID 0x3000
// Endpoint setup flag
#define USB_EP_SETUP 0x0800
// Endpoint type mask
#define USB_EP_EP_TYPE_MASK 0x0600
// Bulk endpoint type
#define USB_EP_EP_TYPE_BULK 0x0000
// Control endpoint type
#define USB_EP_EP_TYPE_CONTROL 0x0200
// Isochronous endpoint type
#define USB_EP_EP_TYPE_ISO 0x0400
// Interrupt endpoint type
#define USB_EP_EP_TYPE_INTERRUPT 0x0600
// Endpoint kind
#define USB_EP_EP_KIND 0x0100
// Endpoint correct transfer TX
#define USB_EP_CTR_TX 0x0080
// Endpoint data toggle TX
#define USB_EP_DTOG_TX 0x0040
// Endpoint TX status mask
#define USB_EP_STAT_TX_MASK 0x0030
// Endpoint TX disabled
#define USB_EP_STAT_TX_DISABLED 0x0000
// Endpoint TX stalled
#define USB_EP_STAT_TX_STALL 0x0010
// Endpoint TX no acknowledge
#define USB_EP_STAT_TX_NAK 0x0020
// Endpoint TX valid
#define USB_EP_STAT_TX_VALID 0x0030
// Endpoint address field
#define USB_EP_EA 0x000F
// Bit set if the endpoint is an input endpoint
#define USB_EP_IN 0x80

// USB correct transfer
#define USB_CNTR_CTRM 0x8000
// USB DMA memory overflow
#define USB_CNTR_PMAOVERM 0x4000
// USB error
#define USB_CNTR_ERRM 0x2000
// USB wakeup
#define USB_CNTR_WKUPM 0x1000
// USB suspend
#define USB_CNTR_SUSPM 0x0800
// USB reset
#define USB_CNTR_RESETM 0x0400
// USB start of frame
#define USB_CNTR_SOFM 0x0200
// USB end start of frame
#define USB_CNTR_ESOFM 0x0100
// USB resume
#define USB_CNTR_RESUME 0x0010
// USB force suspend
#define USB_CNTR_FSUSP 0x0008
// USB low power mode
#define USB_CNTR_LP_MODE 0x0004
// USB pwoerdown
#define USB_CNTR_PDWN 0x0002
// USB force reset
#define USB_CNTR_FRES 0x0001

// USB correct transfer flag
#define USB_ISTR_CTR 0x8000
// USB DMA memory overflow flag
#define USB_ISTR_PMAOVER 0x4000
// USB error flag
#define USB_ISTR_ERR 0x2000
// USB wakeup flag
#define USB_ISTR_WKUP 0x1000
// USB suspend flag
#define USB_ISTR_SUSP 0x0800
// USB reset flag
#define USB_ISTR_RESET 0x0400
// USB start of frame flag
#define USB_ISTR_SOF 0x0200
// USB expected start of frame flag
#define USB_ISTR_ESOF 0x0100
// USB direction flag
#define USB_ISTR_DIR 0x0010
// USB endpoint ID
#define USB_ISTR_EP_ID 0x000F

// USB data plus
#define USB_FNR_RXDP 0x8000
// USB data minus
#define USB_FNR_RXDM 0x4000
// USB clock
#define USB_FNR_LCK 0x2000
// USB start of frame
#define USB_FNR_LSOF 0x1800
// USB frame number
#define USB_FNR_FN 0x07FF

// USB data address EF
#define USB_DADDR_EF 0x0080
// USB data address
#define USB_DADDR_DADDR 0x007F

// USB buffer table address
#define USB_BTABLE_BTABLE 0xFFF8

// Disables FAULT interrupts
static inline void __disable_fault_irq() { asm volatile ("cpsid f"); }
// Disables interrupts
static inline void __disable_irq() { asm volatile ("cpsid i"); }
// Holds up subsequent memory accesses until previous accesses finish (Data Memory Barrier)
static inline void __dmb() { asm volatile ("dmb"); }
// Waits for memory accesses to complete before continuing (Data Synchronization Barrier)
static inline void __dsb() { asm volatile ("dsb"); }
// Enables FAULT interrupts
static inline void __enable_fault_irq() { asm volatile ("cpsie f"); }
// Enables interrupts
static inline void __enable_irq() { asm volatile ("cpsie i"); }
// Flushes the pipeline, as if a branch prediction failed (Instruction Synchronization Barrier)
static inline void __isb() { asm volatile ("isb"); }
// Resets the processor
static inline __attribute__ ((noreturn)) void __reset() {
	// Ask for reset
	SCB->AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_RESET;
	__dsb();
	// Wait until reset occurs
	while (1);
}
// Goes to sleep until the next interrupt
static inline void __sleep() { asm volatile ("wfi\n\t" "nop"); }

// This is needed to ensure IRQs are known as such to the compiler
#define IRQ void __attribute__ (( interrupt("IRQ") ))
// Used for library functions that are better off in-line for constant expansion
#define INLINE inline __attribute__ (( always_inline ))
// Used to pack structures for building packets and register settings
#define PACKED __attribute__ (( packed ))

// Prototype all ISR functions
// Reset
IRQ ISR_Reset();
// Non-maskable interrupt
IRQ ISR_NMI();
// Hardware fault
IRQ ISR_HardFault();
// Memory management fault
IRQ ISR_MemManage();
// Bus fault
IRQ ISR_BusFault();
// Illegal instruction fault
IRQ ISR_UsageFault();
// Supervisor call
IRQ ISR_SVC();
// Debug monitor
IRQ ISR_DebugMon();
// Pending supervisor call
IRQ ISR_PendSV();
// System tick
IRQ ISR_SysTick();
// Window watchdog
IRQ ISR_WWDG();
// Power/voltage detection
IRQ ISR_PVD();
// Real-time clock
IRQ ISR_RTC();
// Flash memory
IRQ ISR_FLASH();
// Reset and clock control
IRQ ISR_RCC();
// External interrupts all Px0 and Px1 pins
IRQ ISR_EXTI1_0();
// External interrupts all Px2 and Px3 pins
IRQ ISR_EXTI3_2();
// External interrupts all Px4-Px15 pins
IRQ ISR_EXTI15_4();
// Touch sensing controller
IRQ ISR_TSC();
// DMA1 channel interrupts
IRQ ISR_DMA1_Channel1();
IRQ ISR_DMA1_Channel3_2();
IRQ ISR_DMA1_Channel7_4();
// ADC and comparator
IRQ ISR_ADC_COMP();
// Low-power TIM1
IRQ ISR_LPTIM1();
// USART 4 and 5
IRQ ISR_USART4_5();
// TIM2
IRQ ISR_TIM2();
// TIM3
IRQ ISR_TIM3();
// TIM6 and DAC
IRQ ISR_TIM6_DAC();
// TIM7
IRQ ISR_TIM7();
// TIM21
IRQ ISR_TIM21();
// TIM22
IRQ ISR_TIM22();
// I2C1
IRQ ISR_I2C1();
// I2C2
IRQ ISR_I2C2();
// I2C3
IRQ ISR_I2C3();
// SPI1
IRQ ISR_SPI1();
// SPI2
IRQ ISR_SPI2();
// USART1
IRQ ISR_USART1();
// USART2
IRQ ISR_USART2();
// Low-power UART1, RNG
IRQ ISR_RNG_LPUART1();
// USB
IRQ ISR_USB();

// End C++ extern to C
#ifdef __cplusplus
}
#endif

#endif
