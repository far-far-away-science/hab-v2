/*
 * system.c - Low-level system control functions for interrupts, clocks, and peripheral init
 */

#include <main.h>
#include <bkup.h>
#include <comm.h>
#include <periph.h>
#include <stmtime.h>

// Switches the system clock to the HSI
static void switchToHSI();
// Switches the system clock to the PLL
static void switchToPLL();

// # of MSI cycles to wait for the HSE to start
#define HSE_WAIT 262144U
// # of core clock cycles (16 or 32 MHz) to wait for the LSE to start (feeds watchdog in loop!)
#define LSE_WAIT 524288U

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
	// Since conversions will occur infrequently, auto-off is a big savings
	ADC1->CFGR1 = ADC_CFGR1_AUTOFF | ADC_CFGR1_RES_12BIT;
	ADC1->CFGR2 = ADC_CFGR2_CKMODE_PCLK_4 | ADC_CFGR2_OVSE | ADC_CFGR2_OVSR_16 |
		ADC_CFGR2_OVSS_NONE;
	// Enable only these two channels for conversions
#ifdef PHOENIX
	ADC1->CHSELR = (1U << ADC_CHAN_LED_1) | (1U << ADC_CHAN_LED_2) | (1U << ADC_CHAN_LED_3) |
		(1U << ADC_CHAN_BAT);
#else
	ADC1->CHSELR = (1U << ADC_CHAN_TEMP_G) | (1U << ADC_CHAN_TEMP_B) | (1U << ADC_CHAN_BAT) |
		(1U << ADC_CHAN_PHOTO_1) | (1U << ADC_CHAN_PHOTO_2);
#endif
	// Enable interrupts for calibration complete
	ADC1->IER = ADC_IER_EOCAL;
	// Clear ADC interrupt flags
	ADC1->ISR = ADC_ISR_ADRDY | ADC_ISR_EOCAL | ADC_ISR_OVR;
	// Calibrate the ADC; when calibrated, the ADRDY bit will be asserted and our ISR will run
	ADC1->CR = ADC_CR_ADCAL;
}

// Starts the MCU clocks at the intended speed (16 MHz for HSI-mode, 32 MHz for PLL-mode)
// If the HSE or PLL times out, falls back automatically to the HSI
static INLINE void initClocks() {
	uint32_t temp = RCC->APB1RSTR & ~(RCC_APB1RSTR_PWRRST), count;
	// If we crashed due to software, set the sysflag
	if (RCC->CSR & (RCC_CSR_IWDGRSTF | RCC_CSR_SFTRSTF | RCC_CSR_WWDGRSTF))
		sysFlags = FLAG_CRASHED;
	RCC->CSR |= RCC_CSR_RMVF;
	// Enable clock to PWR module and CRS module
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
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
#ifdef HSE
	// Turn on the HSE (8 MHz)
	RCC->CR |= RCC_CR_HSEON;
#else
	// Turn on the HSI (16 MHz)
	RCC->CR |= RCC_CR_HSI16ON;
#endif
#ifdef HS32
	// Select Voltage Range 1 (1.8 V) required for 32 MHz
	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | PWR_CR_VOS_0;
#else
	// Select Voltage Range 2 (1.5 V)
	PWR->CR = (PWR->CR & ~PWR_CR_VOS) | PWR_CR_VOS_1;
#endif
	// Wait until the voltage regulator is ready
	while (PWR->CSR & PWR_CSR_VOSF);
#ifdef HSE
	// Wait for HSE to start up
	for (count = HSE_WAIT; count && !(RCC->CR & RCC_CR_HSERDY); count--);
	if (!count) {
		// HSE failed to start
		RCC->CR = (RCC->CR & ~RCC_CR_HSEON) | RCC_CR_HSI16ON;
		// Wait for HSI to start up
		while (!(RCC->CR & RCC_CR_HSI16RDYF));
		sysFlags |= FLAG_HSI;
	}
	// If HSE starts, we are at 8 MHz; else, HSI at 16 MHz
#else
	// Wait for HSI to start up
	while (!(RCC->CR & RCC_CR_HSI16RDYF));
	sysFlags |= FLAG_HSI;
#endif
	// 1 wait state
	FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE;
#ifdef HSE
#ifdef HS32
	// If HSE is on, multiply by 4; else, multiply by 2
	if (count)
		temp = RCC_CFGR_PLLDIV_2 | RCC_CFGR_PLLMUL_8 | RCC_CFGR_PLLSRC_HSE;
	else
		temp = RCC_CFGR_PLLDIV_2 | RCC_CFGR_PLLMUL_4;
#else
	// If HSE is on, multiply by 2; else, multiply by 1
	// PLL input between 2 and 24 MHz, but VCO (after multiplication) must be <= 48 MHz
	if (count)
		temp = RCC_CFGR_PLLDIV_2 | RCC_CFGR_PLLMUL_4 | RCC_CFGR_PLLSRC_HSE;
	else
		// Would use 2/2 but that is not supported
		temp = 0;
#endif
	// Use HSI when waking up from STOP (exitStopMode switches back to PLL)
	RCC->CFGR = temp | RCC_CFGR_STOPWUCK;
	if (temp)
		switchToPLL();
	else
		switchToHSI();
#else
	// Use HSI when waking up from STOP
#ifdef HS32
	// Always multiply HSI by 2
	RCC->CFGR = RCC_CFGR_PLLDIV_2 | RCC_CFGR_PLLMUL_4 | RCC_CFGR_STOPWUCK;
	switchToPLL();
#else
	// Use HSI directly
	RCC->CFGR = RCC_CFGR_STOPWUCK;
	switchToHSI();
#endif
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

// Initializes the CRC calculation module (used for AX.25 encoding)
static INLINE void initCRC() {
	const uint32_t temp = RCC->AHBRSTR & ~RCC_AHBRSTR_CRCRST;
	// Turn on CRC clock and reset CRC
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	RCC->AHBRSTR = temp | RCC_AHBRSTR_CRCRST;
	__dsb();
	RCC->AHBRSTR = temp;
	// 16-bit polynomial just like Pavel said it would be: input reverse halfword, output
	// reverse all
	CRC->CR = CRC_CR_POLYSIZE_16BIT | CRC_CR_REV_IN_HWORD | CRC_CR_REV_OUT;
	// Use default 0xFFFF init value
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
		DMA_CCR_MEMINC | DMA_CCR_TCIE;
	DMA1_Channel7->CPAR = (uint32_t)&(I2C1->RXDR);
	// Reset all interrupt flags
	DMA1->IFCR = 0x0FFFFFFFU;
	// Set remaps
	DMA1_Request->CSELR = DMA_CSELR_C1S_ADC | DMA_CSELR_C2S_SPI1 | DMA_CSELR_C3S_SPI1 |
		DMA_CSELR_C4S_TIM2 | DMA_CSELR_C6S_I2C1 | DMA_CSELR_C7S_I2C1;
}

// Initializes the external interrupts on the button pins
static INLINE void initEXTI() {
	// Configure EXTI line 17 (RTC) to interrupt in rising edge mode
	EXTI->RTSR = RTC_BIT;
	// Clear pending interrupts
	EXTI->PR = 0x7BFFFFU;
	// Unmask the interrupts
	EXTI->IMR = RTC_BIT;
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
#ifdef HS32
	I2C1->TIMINGR = (0x3 << I2C_TIMINGR_PRESC_S) | (0x3 << I2C_TIMINGR_SCLDEL_S) |
		(0x2 << I2C_TIMINGR_SDADEL_S) | (0x3 << I2C_TIMINGR_SCLH_S) | 0x9;
#else
	I2C1->TIMINGR = (0x1 << I2C_TIMINGR_PRESC_S) | (0x3 << I2C_TIMINGR_SCLDEL_S) |
		(0x2 << I2C_TIMINGR_SDADEL_S) | (0x3 << I2C_TIMINGR_SCLH_S) | 0x9;
#endif
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
#ifdef DEBUG_UART
	// IRQ channel 28 (USART2) enable
	intSetPriority(USART2_IRQn, 2);
	intEnable(USART2_IRQn);
#endif
#ifdef HS32
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
	// Disable the HX1
	ioSetOutput(PIN_HX1_EN, false);
	ioSetDirection(PIN_HX1_EN, DDR_OUTPUT);
	// Set USB, I2C, and SPI to high speed, leave A12 and A13 as they are in reset state
	GPIOA->OSPEEDR = 0x0FC00000U;
	GPIOB->OSPEEDR = 0x000F0FC0U;
#ifdef PHOENIX
	// PB6 and PB7 handle USART1
	ioSetDirection(PIN_UART_RX, DDR_AFO);
	ioSetDirection(PIN_UART_TX, DDR_AFO);
	ioSetAlternateFunction(PIN_UART_RX, GPIO_AF0_USART1);
	ioSetAlternateFunction(PIN_UART_TX, GPIO_AF0_USART1);
	// PA9 and PA10 handle USART1 (oops!)
#if 0
	ioSetDirection(PIN_GPS_2_RX, DDR_INPUT_PULLUP);
	ioSetDirection(PIN_GPS_2_TX, DDR_AFO);
	ioSetAlternateFunction(PIN_GPS_2_RX, GPIO_AF4_USART1);
	ioSetAlternateFunction(PIN_GPS_2_TX, GPIO_AF4_USART1);
#endif
#endif
	// Low power UART
	ioSetDirection(PIN_LPUART_RX, DDR_AFO);
	ioSetDirection(PIN_LPUART_TX, DDR_AFO);
	ioSetAlternateFunction(PIN_LPUART_RX, GPIO_AF4_LPUART1);
	ioSetAlternateFunction(PIN_LPUART_TX, GPIO_AF4_LPUART1);
	// Analog pins
#ifdef PHOENIX
	ioSetDirection(PIN_BATTERY, DDR_INPUT_ANALOG);
#ifdef DEBUG_UART
	ioSetDirection(PIN_LED_IN_1, DDR_INPUT_PULLUP);
	ioSetDirection(PIN_LED_IN_2, DDR_AFO);
	ioSetAlternateFunction(PIN_LED_IN_1, GPIO_AF4_USART2);
	ioSetAlternateFunction(PIN_LED_IN_2, GPIO_AF4_USART2);
#else
	ioSetDirection(PIN_LED_IN_1, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_LED_IN_2, DDR_INPUT_ANALOG);
#endif
	ioSetDirection(PIN_LED_IN_3, DDR_INPUT_ANALOG);
	// 1-wire temperature probes use open drain outputs
	ioSetDirection(PIN_OW_TEMP, DDR_OUTPUT_OD);
#else
	ioSetDirection(PIN_TEMP_G, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_TEMP_B, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_PHOTO_1, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_BATTERY, DDR_INPUT_ANALOG);
	ioSetDirection(PIN_PHOTO_2, DDR_INPUT_ANALOG);
#endif
	// SPI pins
	ioSetDirection(PIN_SCK, DDR_AFO);
	ioSetDirection(PIN_MISO, DDR_AFO_PU);
	ioSetDirection(PIN_MOSI, DDR_AFO);
	// CS pin (s)
#ifdef PHOENIX
	ioSetOutput(PIN_LORA_CS, true);
	ioSetDirection(PIN_LORA_CS, DDR_OUTPUT);
#else
	ioSetOutput(PIN_LATCH_CS, true);
	ioSetOutput(PIN_LATCH_A0, true);
	ioSetOutput(PIN_LATCH_A1, true);
	ioSetOutput(PIN_LATCH_A2, true);
	ioSetDirection(PIN_LATCH_CS, DDR_OUTPUT);
	ioSetDirection(PIN_LATCH_A0, DDR_OUTPUT);
	ioSetDirection(PIN_LATCH_A1, DDR_OUTPUT);
	ioSetDirection(PIN_LATCH_A2, DDR_OUTPUT);
#endif
	// I2C pins
	ioSetDirection(PIN_SCL, DDR_AFO_OD);
	ioSetDirection(PIN_SDA, DDR_AFO_OD);
	ioSetAlternateFunction(PIN_SCL, GPIO_AF4_I2C1);
	ioSetAlternateFunction(PIN_SDA, GPIO_AF4_I2C1);
	// LED pins
#ifdef PHOENIX
	// No PWM control (L05x)
	ioSetOutput(PIN_LED_R, true);
	ioSetOutput(PIN_LED_G, true);
	ioSetOutput(PIN_LED_B, true);
	ioSetDirection(PIN_LED_R, DDR_OUTPUT_OD);
	ioSetDirection(PIN_LED_G, DDR_OUTPUT_OD);
	ioSetDirection(PIN_LED_B, DDR_OUTPUT_OD);
#else
	// PWM control on TIM3 (L07x)
	ioSetDirection(PIN_LED_R, DDR_AFO_OD);
	ioSetDirection(PIN_LED_G, DDR_AFO_OD);
	ioSetDirection(PIN_LED_B, DDR_AFO_OD);
	ioSetAlternateFunction(PIN_LED_R, GPIO_AF2_TIM3);
	ioSetAlternateFunction(PIN_LED_G, GPIO_AF2_TIM3);
	ioSetAlternateFunction(PIN_LED_B, GPIO_AF2_TIM3);
#endif
	// DAC
	ioSetDirection(PIN_DAC, DDR_INPUT_ANALOG);
	// Lock all pin modes
	lockPort(GPIOA, 0xFFFFU);
	lockPort(GPIOB, 0x7FFFU);
}

// Initializes the RTC
static INLINE void initRTC() {
	uint32_t count, temp;
#ifdef LSE
	// Turn on the LSE
	RCC->CSR |= RCC_CSR_LSEON | RCC_CSR_LSEDRV_0;
	// Wait for the LSE to be ready
#ifdef LSI
	for (count = LSE_WAIT; count && !(RCC->CSR & RCC_CSR_LSERDY); count--)
		feedWatchdog();
	if (count)
		// LSE is ready
		temp = RCC_CSR_RTCSEL_LSE;
	else {
		temp = RCC_CSR_RTCSEL_LSI;
		// Turn on the LSI
		RCC->CSR = (RCC->CSR & ~RCC_CSR_LSEON) | RCC_CSR_LSION;
		// Wait for the LSI to be ready (no last resort if the LSI does not start!)
		while (!(RCC->CSR & RCC_CSR_LSIRDY))
			feedWatchdog();
		sysFlags |= FLAG_LSI;
	}
	// Set up the RTC to use the selected oscillator
	RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | temp;
#else
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
	// Reset clock to 1/1/2016 00:00:00 GMT
	RTC->TR = 0U;
	RTC->DR = (uint32_t)(1U | (1U << RTC_DR_MU_S) | RTC_DR_FRIDAY | (6U << RTC_DR_YU_S) |
		(1U << RTC_DR_YT_S));
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

// Initializes the SPI for SD card (does not actually start the card) and LoRA
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
#ifdef HS32
	SPI1->CR1 = SPI_CR1_NSS_SOFT | SPI_CR1_DIV8 | SPI_CR1_MSTR | SPI_CR1_SPE;
#else
	SPI1->CR1 = SPI_CR1_NSS_SOFT | SPI_CR1_DIV4 | SPI_CR1_MSTR | SPI_CR1_SPE;
#endif
#ifdef HABV2
	spiSelect(SPI_DEVICE_NONE);
#endif
}

// Initializes the timers, TIM2 for DAC, TIM3 for PWM on HAB only
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
#ifdef HS32
	// Set TIM2 up for a divider of 4 and a reload of 125 (32K / 500 = 64000)
	TIM2->PSC = 3U;
	TIM2->ARR = (((32000000U + (AUDIO_FREQ >> 1)) / AUDIO_FREQ) + 3U) >> 2;
#else
	// Set TIM2 up for a divider of 2 and a reload of 125 (16K / 250 = 64000)
	TIM2->PSC = 1U;
	TIM2->ARR = (((16000000U + (AUDIO_FREQ >> 1)) / AUDIO_FREQ) + 1U) >> 1;
#endif
	// Fire the DMA Ch4 through CCR4 (ugly hack to avoid channel clash!)
	TIM2->CCR4 = 0U;
	TIM2->DIER = TIM_DIER_CC4DE;
	// Make the update event fire the DAC
	TIM2->CR2 = TIM_CR2_MMS_UPDATE;
	TIM2->CR1 = TIM_CR1_ARPE;
#ifdef HABV2
	// LED dimming timers only on HABv2
	// TIM3 init for a reload of 65535 (maximum control!) and a divider of 1
	TIM3->ARR = 0xFFFFU;
	TIM3->PSC = 0U;
	TIM3->DIER = 0U;
	// LED is on channel 2-4
	TIM3->CCMR1 = TIM_CCMR1_CC2S_OUT | TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
	TIM3->CCMR2 = TIM_CCMR2_CC4S_OUT | TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE |
		TIM_CCMR2_CC3S_OUT | TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
	// PA7: green
	TIM3->CCR2 = 0U;
	// PB0: red
	TIM3->CCR3 = 0U;
	// PB1: blue
	TIM3->CCR4 = 0U;
	// Set LEDs to active low, enable all channels
	TIM3->CCER = TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC4E |
		TIM_CCER_CC4P;
	// Turn it on and away we go
	TIM3->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;
#endif
}

// Exits STOP mode and restores the system clock if necessary
void exitStopMode() {
#ifdef HSE
	// If HSE was used, determine whether PLL is configured; if it was, then switch back
	// The right settings were already set in the CFGR register for source and speed
	if (RCC->CFGR & RCC_CFGR_PLLMUL)
		switchToPLL();
#elif defined(HS32)
	// Restart PLL (not HSE, so unconditional *2)
	switchToPLL();
#endif
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
	initCRC();
	serialInit();
	initInterrupts();
}

// Switches the system clock to the HSI
static void switchToHSI() {
	// Select HSI as system clock
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	// Wait for system clock to become the HSI
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
}

// Switches the system clock to the PLL
static void switchToPLL() {
	// Turn PLL on
	RCC->CR |= RCC_CR_PLLON;
	// Wait for PLL to start up
	while (!(RCC->CR & RCC_CR_PLLRDY));
	// Select PLL as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait for system clock to become the PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
