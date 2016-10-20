/*
 * startup.s - STM32L0 C setup and load routines
 */

.section .text.ISR_Reset, "x"
.global ISR_Reset
.syntax unified
.thumb
.thumb_func

ISR_Reset:
	/* Initialize RAM data */
	ldr r2, =_sidata
	ldr r3, =_sdata
	ldr r1, =_edata
	b InitCheck
InitLoop:
	ldr r0, [r2]
	adds r2, r2, #4
	str r0, [r3]
	adds r3, r3, #4
InitCheck:
	cmp	r3, r1
	bcc InitLoop
	/* Clear BSS data to zero */
	ldr r3, =_sbss
	ldr r2, =_ebss
	movs r1, #0
	b BSSCheck
BSSLoop:
	str r1, [r3]
	adds r3, r3, #4
BSSCheck:
	cmp	r3, r2
	bcc BSSLoop
	/* Set up program */
	bl initMCU
	/* Run main(), b in thumb mode has 2K limit so bl is used instead */
	bl main

/* Vector table */
.section .isr_vector, "a", %progbits
.global VectorTable

VectorTable:
	.word _estack
	.word ISR_Reset
	.word ISR_NMI
	.word ISR_HardFault
	.word ISR_MemManage
	.word ISR_BusFault
	.word ISR_UsageFault
	.word 0
	.word 0
	.word 0
	.word 0
	.word ISR_SVC
	.word ISR_DebugMon
	.word 0
	.word ISR_PendSV
	.word ISR_SysTick
	.word ISR_WWDG
	.word ISR_PVD
	.word ISR_RTC
	.word ISR_FLASH
	.word ISR_RCC
	.word ISR_EXTI1_0
	.word ISR_EXTI3_2
	.word ISR_EXTI15_4
	.word ISR_TSC
	.word ISR_DMA1_Channel1
	.word ISR_DMA1_Channel3_2
	.word ISR_DMA1_Channel7_4
	.word ISR_ADC_COMP
	.word ISR_LPTIM1
	.word ISR_USART_4_5
	.word ISR_TIM2
	.word ISR_TIM3
	.word ISR_TIM6_DAC
	.word ISR_TIM7
	.word 0
	.word ISR_TIM21
	.word ISR_I2C3
	.word ISR_TIM22
	.word ISR_I2C1
	.word ISR_I2C2
	.word ISR_SPI1
	.word ISR_SPI2
	.word ISR_USART1
	.word ISR_USART2
	.word ISR_RNG_LPUART1
	.word 0
	.word ISR_USB
