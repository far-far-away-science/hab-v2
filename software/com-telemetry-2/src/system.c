/*
 * system.c - Low-level system control functions for interrupts, clocks, and peripheral init
 */

#include <main.h>
#include <bkup.h>
#include <comm.h>
#include <periph.h>
#include <stmtime.h>

// Attention: Variable will overflow every now and then, must be handled
volatile uint32_t sysTime;

// Utilities for the (harder) IRQ priority setting routine
#define IRQ_SHIFT(_irq) (((uint32_t)(_irq) & 0x03) << 3)
#define IRQ_SHP(_irq) ((((uint32_t)(_irq) & 0x0F) - 8) >> 2)
#define IRQ_IP(_irq) ((uint32_t)(_irq) >> 2)

// Enables a peripheral interrupt
static INLINE void intEnable(IRQn_Type irq) {
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)irq & 0x1F));
}

// Sets the priority for an interrupt; 3 is lowest priority, 0 is highest
static INLINE void intSetPriority(IRQn_Type irq, uint32_t priority) {
	const uint32_t sh = IRQ_SHIFT(irq);
	if (irq < 0) {
		const uint32_t pos = IRQ_SHP(irq);
		SCB->SHP[pos] = (SCB->SHP[pos] & ~(0xFF << sh)) | (((priority << 6) & 0xFF) << sh);
	} else {
		const uint32_t pos = IRQ_IP(irq);
		NVIC->IP[pos] = (NVIC->IP[pos] & ~(0xFF << sh)) | (((priority << 6) & 0xFF) << sh);
	}
}

// Lock the specified pins of the given port - 1 in a position to lock that pin, 0 to leave
// unlocked
static INLINE void lockPort(GPIO_TypeDef *port, const uint32_t pins) {
	const uint32_t pins1 = pins | 0x10000U;
	port->LCKR = pins1;
	port->LCKR = pins;
	port->LCKR = pins1;
	__dsb();
	(void)port->LCKR;
}

// Initializes the analog-to-digital converter
static INLINE void initADC() {
	const uint32_t temp = RCC->APB2RSTR & ~RCC_APB2RSTR_ADCRST;
	// Enable clock to ADC module
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	// Reset the ADC
	RCC->APB2RSTR = temp | RCC_APB2RSTR_ADCRST;
	__dsb();
	RCC->APB2RSTR = temp;
	// Configure for DMA mode, 12-bit resolution (+4 bits oversampling), automatic off mode
	// Since conversions will occur at most once per second, auto-off is a big savings
	ADC1->CFGR1 = ADC_CFGR1_AUTOFF | ADC_CFGR1_RES_12BIT;
	ADC1->CFGR2 = ADC_CFGR2_CKMODE_PCLK_4 | ADC_CFGR2_OVSE | ADC_CFGR2_OVSR_16 |
		ADC_CFGR2_OVSS_NONE;
	// Enable only these two channels for conversions
	ADC1->CHSELR = (1U << ADC_CHAN_PC) | (1U << ADC_CHAN_PWR);
	// Enable interrupts for calibration complete
	ADC1->IER = ADC_IER_EOCAL;
	// Clear ADC interrupt flags
	ADC1->ISR = ADC_ISR_ADRDY | ADC_ISR_EOCAL | ADC_ISR_OVR;
	// Calibrate the ADC; when calibrated, the ADRDY bit will be asserted and our ISR will run
	ADC1->CR = ADC_CR_ADCAL;
}

// Starts the MCU clocks at the intended speed (16 MHz for HSI-mode, 32 MHz for PLL-mode)
// No startup timeout is enforced since the program will fail anyways if the clock speed is
// wrong...
static INLINE void initClocks() {
	const uint32_t temp = RCC->APB1RSTR & ~(RCC_APB1RSTR_PWRRST | RCC_APB1RSTR_CRSRST);
	// If we crashed due to software, set the sysflag
	if (RCC->CSR & (RCC_CSR_IWDGRSTF | RCC_CSR_SFTRSTF | RCC_CSR_WWDGRSTF))
		sysFlags = FLAG_CRASHED;
	RCC->CSR |= RCC_CSR_RMVF;
	// Enable clock to PWR module and CRS module
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_CRSEN;
	// Enable clock to SYSCFG module
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Reset the CRS, do not reset SYSCFG or PWR!
	RCC->APB1RSTR = temp | RCC_APB1RSTR_CRSRST;
	__dsb();
	RCC->APB1RSTR = temp;
	// Clear interrupt pending bits
	RCC->CICR = 0x000001FF;
	// Reset Sleep Control register to zero to avoid unwanted deep sleep
	SCB->SCR = 0x00000000;
	// Enable access to RTC domain to set up
	PWR->CR |= PWR_CR_DBP;
	__dsb();
	// Turn on the HSI (16 MHz)
	RCC->CR |= RCC_CR_HSI16ON;
#ifdef PLL
	// Select Voltage Range 1 (1.8 V) required for 32 MHz
	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | PWR_CR_VOS_0;
	// Wait until the voltage regulator is ready
	while (PWR->CSR & PWR_CSR_VOSF);
	// Wait for HSI to start up
	while (!(RCC->CR & RCC_CR_HSI16RDYF));
	// 1 wait state
	FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE;
	// PLL enabled from HSI = 16 MHz * 2 = 32 MHz, use the HSI16 when waking up from STOP
	RCC->CFGR = RCC_CFGR_PLLDIV_2 | RCC_CFGR_PLLMUL_4 | RCC_CFGR_STOPWUCK;
	switchToPLL();
#else
	// Select Voltage Range 2 (1.5 V)
	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | PWR_CR_VOS_1;
	// Wait until the voltage regulator is ready
	while (PWR->CSR & PWR_CSR_VOSF);
	// Wait for HSI to start up
	while (!(RCC->CR & RCC_CR_HSI16RDYF));
	// 1 wait state
	FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE;
	// Select HSI as system clock, use the HSI16 when waking up from STOP
	RCC->CFGR = RCC_CFGR_STOPWUCK | RCC_CFGR_SW_HSI;
	// Wait for system clock to become the HSI
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
#endif
	// Turn off the MSI now that we are no longer using it to save juice!
	RCC->CR &= ~RCC_CR_MSION;
#ifdef WATCHDOG_ON
	// Start the independent watchdog
	IWDG->KR = IWDG_KR_UNLOCK;
	IWDG->PR = IWDG_PR_PR_DIV256;
	// 40 KHz / 256 / 256 = 0.6 Hz < 1 Hz
	IWDG->RLR = 0xFFU;
	IWDG->KR = IWDG_KR_START;
	__dsb();
#endif
	feedWatchdog();
}

// Initializes the digital-to-analog converter
static INLINE void initDAC() {
	const uint32_t temp = RCC->APB1RSTR & ~RCC_APB1RSTR_DACRST;
	// Turn on DAC clock and reset DAC
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_DACRST;
	__dsb();
	RCC->APB1RSTR = temp;
	// Load a zero into data registers
	DAC->DHR12R1 = 0U;
	// Enable trigger
	DAC->CR = DAC_CR_TEN1 | DAC_CR_TSEL1_SOFT;
}

// Initializes DMA (direct memory access)
static INLINE void initDMA() {
	const uint32_t temp = RCC->AHBRSTR & ~RCC_AHBRSTR_DMARST;
	// Turn on DMA clock and reset DMA
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	RCC->AHBRSTR = temp | RCC_AHBRSTR_DMARST;
	__dsb();
	RCC->AHBRSTR = temp;
	// DMA Ch1 handles ADC1
	DMA1_Channel1->CCR = DMA_CCR_SRC | DMA_CCR_DST_HWORD | DMA_CCR_SRC_HWORD | DMA_CCR_PRI_MED |
		DMA_CCR_MEMINC | DMA_CCR_TCIE;
	DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
	// DMA Ch2 handles SPI1 RX
	DMA1_Channel2->CCR = DMA_CCR_SRC | DMA_CCR_DST_BYTE | DMA_CCR_SRC_BYTE | DMA_CCR_PRI_MED |
		DMA_CCR_MEMINC | DMA_CCR_TCIE;
	DMA1_Channel2->CPAR = (uint32_t)&(SPI1->DR);
	// DMA Ch3 handles SPI1 TX
	DMA1_Channel3->CCR = DMA_CCR_DST | DMA_CCR_DST_BYTE | DMA_CCR_SRC_BYTE | DMA_CCR_PRI_MED |
		DMA_CCR_MEMINC | DMA_CCR_TCIE;
	DMA1_Channel3->CPAR = (uint32_t)&(SPI1->DR);
	// DMA Ch4 handles DAC1
	DMA1_Channel4->CCR = DMA_CCR_DST | DMA_CCR_DST_HWORD | DMA_CCR_SRC_HWORD | DMA_CCR_PRI_MAX |
		DMA_CCR_MEMINC | DMA_CCR_TCIE | DMA_CCR_HTIE | DMA_CCR_CIRC;
	DMA1_Channel4->CPAR = (uint32_t)&(DAC->DHR12R1);
	// DMA Ch6 handles I2C1 TX
	DMA1_Channel6->CCR = DMA_CCR_DST | DMA_CCR_DST_BYTE | DMA_CCR_SRC_BYTE | DMA_CCR_PRI_HIGH |
		DMA_CCR_MEMINC;
	DMA1_Channel6->CPAR = (uint32_t)&(I2C1->TXDR);
	// DMA Ch7 handles I2C1 RX
	DMA1_Channel7->CCR = DMA_CCR_SRC | DMA_CCR_DST_BYTE | DMA_CCR_SRC_BYTE | DMA_CCR_PRI_HIGH |
		DMA_CCR_MEMINC;
	DMA1_Channel7->CPAR = (uint32_t)&(I2C1->RXDR);
	// Reset all interrupt flags
	DMA1->IFCR = 0x0FFFFFFFU;
	// Set remaps
	DMA1_Request->CSELR = DMA_CSELR_C1S_ADC | DMA_CSELR_C2S_SPI1 | DMA_CSELR_C3S_SPI1 |
		DMA_CSELR_C4S_TIM2 | DMA_CSELR_C6S_I2C1 | DMA_CSELR_C7S_I2C1;
}

// Initializes the external interrupts on the button pins
static INLINE void initEXTI() {
	// EXTI 0 -> A (!snooze)
	// EXTI 1 -> B (!alarm)
	// EXTI 5 -> A (!up)
	// EXTI 8 -> A (!down)
	SYSCFG->EXTICR[0] = 0x0010U;
	SYSCFG->EXTICR[1] = 0x0000U;
	SYSCFG->EXTICR[2] = 0x0000U;
	// Enable falling-edge interrupts for UP, DN, SNOOZE, and ALARM
	EXTI->FTSR = BTN_ALL_BIT;
	// Configure EXTI line 17 (RTC) to interrupt in rising edge mode
	EXTI->RTSR = RTC_BIT;
	// Clear pending interrupts
	EXTI->PR = 0x7BFFFFU;
	// Unmask the interrupts
	EXTI->IMR = BTN_ALL_BIT | RTC_BIT;
}

// Initializes the new I2C peripheral on L0 chips, different from I2C on other ST chips!
static INLINE void initI2C() {
	const uint32_t temp = RCC->APB1RSTR & ~RCC_APB1RSTR_I2C1RST;
	// Turn on I2C clock and reset I2C
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB1RSTR = temp | RCC_APB1RSTR_I2C1RST;
	__dsb();
	RCC->APB1RSTR = temp;
	// RM0376 p. 629 @ 400 KHz, p. 656 has bitfield
	// PRESC = 0x1, SCLL = 0x9, SCLH = 0x3, SDADEL = 0x2, SCLDEL = 0x3
	I2C1->TIMINGR = (0x1 << I2C_TIMINGR_PRESC_S) | (0x3 << I2C_TIMINGR_SCLDEL_S) |
		(0x2 << I2C_TIMINGR_SDADEL_S) | (0x3 << I2C_TIMINGR_SCLH_S) | 0x9;
	// Not relevant in master mode
	I2C1->OAR1 = 0x00;
	I2C1->OAR2 = 0x00;
	// Cannot configure timeout unless using as SMBus?
	I2C1->CR1 = I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN | I2C_CR1_ERRIE | I2C_CR1_NACKIE |
		I2C_CR1_STOPIE | I2C_CR1_PE;
}

// Initializes the NVIC (interrupt system)
static INLINE void initInterrupts() {
	SCB->VTOR = FLASH_BASE;
#ifdef LSE
	// IRQ channel 2 (RTC alarm) enable
	intSetPriority(RTC_IRQn, 1);
	intEnable(RTC_IRQn);
#endif
	// IRQ channel 5 (EXTI 0 and 1) enable
	intSetPriority(EXTI1_0_IRQn, 2);
	intEnable(EXTI1_0_IRQn);
	// IRQ channel 7 (EXTI 4-15) enable
	intSetPriority(EXTI15_4_IRQn, 2);
	intEnable(EXTI15_4_IRQn);
	// IRQ channel 9 (DMA channel 1) enable
	intSetPriority(DMA1_Channel1_IRQn, 3);
	intEnable(DMA1_Channel1_IRQn);
	// IRQ channel 10 (DMA channels 2 and 3) enable
	intSetPriority(DMA1_Channel3_2_IRQn, 0);
	intEnable(DMA1_Channel3_2_IRQn);
	// IRQ channel 23 (I2C1) enable
	intSetPriority(I2C1_IRQn, 2);
	intEnable(I2C1_IRQn);
	// IRQ channel 11 (DMA channels 4-7) enable
	intSetPriority(DMA1_Channel7_4_IRQn, 0);
	intEnable(DMA1_Channel7_4_IRQn);
	// IRQ channel 12 (ADC1 and COMP) enable
	intSetPriority(ADC_COMP_IRQn, 3);
	intEnable(ADC_COMP_IRQn);
	// IRQ channel 27 (USART1) enable
	intSetPriority(USART1_IRQn, 2);
	intEnable(USART1_IRQn);
	// IRQ channel 31 (USB) enable
	intSetPriority(USB_IRQn, 1);
	intEnable(USB_IRQn);
#ifdef PLL
	// SysTick fires every 40000 clock cycles (32M / 8 / 40K = 100/s = 10 ms)
	SysTick->LOAD = 39999U;
#else
	// SysTick fires every 20000 clock cycles (16M / 8 / 20K = 100/s = 10 ms)
	SysTick->LOAD = 19999U;
#endif
	// Reset counter to zero
	SysTick->VAL = 0;
	// Interrupt is only enabled on demand to save power and CPU cycles
	sysTickDisable();
	__enable_irq();
}

// Initializes the GPIO ports
static INLINE void initPorts() {
	const uint32_t temp = RCC->IOPRSTR & ~(RCC_IOPRSTR_IOPARST | RCC_IOPRSTR_IOPBRST |
		RCC_IOPRSTR_IOPCRST);
	// Enable clocks to I/O ports and reset them
	RCC->IOPENR |= RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN | RCC_IOPENR_IOPCEN;
	RCC->IOPRSTR = temp | RCC_IOPRSTR_IOPARST | RCC_IOPRSTR_IOPBRST | RCC_IOPRSTR_IOPCRST;
	__dsb();
	RCC->IOPRSTR = temp;
	// Disable the amp
	ioSetOutput(PIN_AMP_SD, false);
	ioSetDirection(PIN_AMP_SD, DDR_OUTPUT);
	// Set up USB as alternate function output push pull, select some arbitrary function to
	// avoid SPI1 trying to use these as inputs
	ioSetDirection(PIN_USBDM, DDR_AFO);
	ioSetDirection(PIN_USBDP, DDR_AFO);
	ioSetAlternateFunction(PIN_USBDM, GPIO_AF2_EVENTOUT);
	ioSetAlternateFunction(PIN_USBDP, GPIO_AF2_EVENTOUT);
	// Set USB, I2C, and SPI to high speed, leave A12 and A13 as they are in reset state
	GPIOA->OSPEEDR = 0x0FC00000U;
	GPIOB->OSPEEDR = 0x0000FFC0U;
#if 0
	// PA2 and PA3 handle USART1 (for now)
	ioSetDirection(GPIOA, 9, DDR_AFO);
	ioSetDirection(GPIOA, 10, DDR_AFO);
	ioSetAlternateFunction(GPIOA, 9, GPIO_AF4_USART1);
	ioSetAlternateFunction(GPIOA, 10, GPIO_AF4_USART1);
#endif
	// PA0, PA5, PA8, and PB1 handle general purpose buttons
	ioSetDirection(PIN_ALARM, DDR_INPUT_PULLUP);
	ioSetDirection(PIN_DOWN, DDR_INPUT_PULLUP);
	ioSetDirection(PIN_SNOOZE, DDR_INPUT_PULLUP);
	ioSetDirection(PIN_UP, DDR_INPUT_PULLUP);
	// Analog pins
	ioSetDirection(PIN_PHOTOCELL, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_POWER_DET, DDR_INPUT_ANALOG);
	// PA6 and PA7 are timer outputs for LED and piezo respectively
	ioSetDirection(PIN_LED, DDR_AFO_OD);
	ioSetDirection(PIN_PIEZO, DDR_AFO);
	ioSetAlternateFunction(PIN_LED, GPIO_AF5_TIM22);
	ioSetAlternateFunction(PIN_PIEZO, GPIO_AF5_TIM22);
	// SPI pins
	ioSetDirection(PIN_SCK, DDR_AFO);
	ioSetDirection(PIN_MISO, DDR_AFO_PU);
	ioSetDirection(PIN_MOSI, DDR_AFO);
	// CS pin
	ioSetOutput(PIN_CS, 1);
	ioSetDirection(PIN_CS, DDR_OUTPUT);
	// I2C pins
	ioSetDirection(PIN_SCL, DDR_AFO_OD);
	ioSetDirection(PIN_SDA, DDR_AFO_OD);
	ioSetAlternateFunction(PIN_SCL, GPIO_AF1_I2C1);
	ioSetAlternateFunction(PIN_SDA, GPIO_AF1_I2C1);
	// DAC
	ioSetDirection(PIN_DAC, DDR_INPUT_PULLUP);
	// Lock all pin modes except the DAC pin
	lockPort(GPIOA, 0xFFEFU);
	lockPort(GPIOB, 0xFFFFU);
}

// Initializes the RTC
static INLINE void initRTC() {
#ifdef LSE
#ifdef LSI
	// Turn on the LSI
	RCC->CSR |= RCC_CSR_LSION;
	// Wait for the LSI to be ready (there is no point in coming up if the LSI does not start!)
	while (!(RCC->CSR & RCC_CSR_LSIRDY)) feedWatchdog();
	// Set up the RTC to use LSI
	RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_LSI;
#else
	// Turn on the LSE
	RCC->CSR |= RCC_CSR_LSEON | RCC_CSR_LSEDRV_0;
	// Wait for the LSE to be ready (there is no point in coming up if the LSE does not start!)
	feedWatchdog();
	while (!(RCC->CSR & RCC_CSR_LSERDY));
	// Set up the RTC to use LSE
	RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_LSE;
#endif
	__dsb();
	// Enter initialization mode
	RTC->WPR = RTC_WPR_INIT1;
	RTC->WPR = RTC_WPR_INIT2;
	__dsb();
	// Clears all event flags
	RTC->ISR = RTC_ISR_INIT;
	feedWatchdog();
	while (!(RTC->ISR & RTC_ISR_INITF));
	// Configure RTC prescalars for 1 Hz clock (32.768 KHz / (127 + 1) / (255 + 1) = 1 Hz)
	RTC->PRER = (RTC->PRER & ~RTC_PRER_PREDIV_A) | (uint32_t)(127U << 16);
	RTC->PRER = (RTC->PRER & ~RTC_PRER_PREDIV_S) | (uint32_t)(255U);
	// Set LSE adjustment (RM0376 p. 586)
#if defined(LSE_COMP) && LSE_COMP != 0
#if LSE_COMP > 0
	// Positive, set CALP, load complement into CALM
	RTC->CALR = RTC_CAL_CALP | ((uint32_t)(512U - (LSE_COMP)) & 0x1FFU);
#else
	// Negative, set CALM
	RTC->CALR = ((uint32_t)-(LSE_COMP) & 0x1FFU);
#endif
#endif
	// Reset clock to Thu 6/5/2014 8:07 PM PST (1000 bonus points if you get the reference)
	if (!(RTC->ISR & RTC_ISR_INITS)) {
		//eeWrite(EE_REG_AL_1, (7U << 8) | 40U);
		eeWrite(EE_REG_AL_1, (20U << 8) | 8U);
		eeWrite(EE_REG_AL_2, (8U << 8) | 40U);
#ifdef RESET_RTC_ON_BOOT
	}
#endif
		RTC->TR = (uint32_t)((7U << RTC_TR_MNU_S) | (2U << RTC_TR_HT_S));
		RTC->DR = (uint32_t)(5U | (6U << RTC_DR_MU_S) | (4U << RTC_DR_WDU_S) |
			(4U << RTC_DR_YU_S) | (1U << RTC_DR_YT_S));
#ifndef RESET_RTC_ON_BOOT
	}
#endif
	// Disable alarm A and B
	RTC->CR &= ~(RTC_CR_ALRAE | RTC_CR_ALRAIE | RTC_CR_ALRBIE | RTC_CR_ALRBE | RTC_CR_WUTE);
	__dsb();
#if 1
	// Set up alarm B to trip every second
	RTC->ALRMBR = RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1;
	RTC->ALRMBSSR = 0x00U;
	RTC->CR |= RTC_CR_ALRAIE | RTC_CR_ALRBIE | RTC_CR_ALRBE;
	// Exit initialization mode and enable write protection
	RTC->ISR = RTC_ISR_NOP & ~(RTC_ISR_INIT | RTC_ISR_ALRAF | RTC_ISR_ALRBF);
#else
	// Use wakeup timer instead which is designed for 1s wakeups
	// 32768 Hz / 16 = 2048
	RTC->WUTR = 2047U;
	RTC->CR |= RTC_CR_ALRAIE | RTC_CR_WUTIE | RTC_CR_WUTE | RTC_CR_WUCKSEL_16;
	// Exit initialization mode and enable write protection
	RTC->ISR = RTC_ISR_NOP & ~(RTC_ISR_INIT | RTC_ISR_ALRAF | RTC_ISR_WUTF);
#endif
	RTC->WPR = RTC_WPR_LOCK;
#endif
}

// Initializes the SPI for SD card (does not actually start the card)
static INLINE void initSD() {
	const uint32_t temp = RCC->APB2RSTR & ~RCC_APB2RSTR_SPI1RST;
	// Turn on SPI clock and reset SPI
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->APB2RSTR = temp | RCC_APB2RSTR_SPI1RST;
	__dsb();
	RCC->APB2RSTR = temp;
	// SPI set up for CPHA 0, CPOL 0, no slave select, master mode, divide by 4, enable
	// (observed on this board that /2 with no PLL generated less than full clock swing)
	SPI1->CR2 = 0x0U;
#ifdef PLL
	SPI1->CR1 = SPI_CR1_NSS_SOFT | SPI_CR1_DIV8 | SPI_CR1_MSTR | SPI_CR1_SPE;
#else
	SPI1->CR1 = SPI_CR1_NSS_SOFT | SPI_CR1_DIV4 | SPI_CR1_MSTR | SPI_CR1_SPE;
#endif
}

// Initializes the timers, TIM2 for DAC, TIM22 for PWM
static INLINE void initTIM() {
	const uint32_t temp1 = RCC->APB1RSTR & ~RCC_APB1RSTR_TIM2RST, temp2 =
		RCC->APB2RSTR & ~RCC_APB2RSTR_TIM22RST;
	// Turn on TIM2 and TIM22 clock and reset both
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM22EN;
	RCC->APB1RSTR = temp1 | RCC_APB1RSTR_TIM2RST;
	RCC->APB2RSTR = temp2 | RCC_APB2RSTR_TIM22RST;
	__dsb();
	RCC->APB1RSTR = temp1;
	RCC->APB2RSTR = temp2;
#ifdef PLL
	// Set TIM2 up for a divider of 8 and a reload of 125 (32K / 500 = 32000)
	TIM2->PSC = 7U;
#else
	// Set TIM2 up for a divider of 4 and a reload of 125 (16K / 500 = 32000)
	TIM2->PSC = 3U;
#endif
	TIM2->ARR = 124U;
	// Fire the DMA Ch4 through CCR4 (ugly hack to avoid channel clash!)
	TIM2->CCR4 = 0U;
	TIM2->DIER = TIM_DIER_CC4DE;
	// Make the update event fire the DAC
	TIM2->CR2 = TIM_CR2_MMS_UPDATE;
	TIM2->CR1 = TIM_CR1_ARPE;
	// TIM22 init for a reload of 65535 (maximum control!) and a divider of 1
	TIM22->ARR = 0xFFFFU;
	TIM22->PSC = 0U;
	TIM22->DIER = 0U;
	// LED is on channel 1, PA6, set as output, PWM mode 1
	// Piezo buzzer is on channel 2, PA7, set as output, PWM mode 1
	TIM22->CCMR1 = TIM_CCMR1_CC1S_OUT | TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE |
		TIM_CCMR1_CC2S_OUT | TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_PWM1;
	TIM22->CCR1 = 0U;
	TIM22->CCR2 = 0U;
	// Set LED to active low, enable both channels
	TIM22->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC2E;
	// Turn it on and away we go
	TIM22->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
}

// Initializes the MCU
void initMCU() {
	sysFlags = 0U;
	initClocks();
	initPorts();
	initEXTI();
	initTIM();
	initSD();
	initI2C();
	initDAC();
	initDMA();
	initADC();
	initRTC();
	bkupClearAll();
	initInterrupts();
}

#ifdef PLL
// Switches the system clock to the PLL
void switchToPLL() {
	// Turn PLL on
	RCC->CR |= RCC_CR_PLLON;
	// Wait for PLL to start up
	while (!(RCC->CR & RCC_CR_PLLRDY));
	// Select PLL as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait for system clock to become the PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
#endif
