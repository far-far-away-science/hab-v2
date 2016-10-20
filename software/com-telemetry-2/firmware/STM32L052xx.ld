/* Memory space definitions */
MEMORY {
	RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
}

/* Higher address of the user mode stack */
EXTERN ( _estack );
PROVIDE ( _estack = ORIGIN(RAM) + LENGTH(RAM) );

/* This sends all unreferenced IRQHandlers to reset. */
PROVIDE ( ISR_SWI = 0 );
PROVIDE ( ISR_IRQ = 0 );
PROVIDE ( ISR_Prefetch = 0 );
PROVIDE ( ISR_Abort = 0 );
PROVIDE ( ISR_FIQ = 0 );

PROVIDE ( ISR_NMI = 0 );
PROVIDE ( ISR_HardFault = 0 );
PROVIDE ( ISR_MemManage = 0 );
PROVIDE ( ISR_BusFault = 0 );
PROVIDE ( ISR_UsageFault = 0 );

PROVIDE ( ISR_SVC = 0 );
PROVIDE ( ISR_DebugMon = 0 );
PROVIDE ( ISR_PendSV = 0 );
PROVIDE ( ISR_SysTick = 0 );

PROVIDE ( ISR_WWDG = 0 );
PROVIDE ( ISR_PVD = 0 );
PROVIDE ( ISR_RTC = 0 );
PROVIDE ( ISR_FLASH = 0 );
PROVIDE ( ISR_RCC = 0 );
PROVIDE ( ISR_EXTI1_0 = 0 );
PROVIDE ( ISR_EXTI3_2 = 0 );
PROVIDE ( ISR_EXTI15_4 = 0 );
PROVIDE ( ISR_TSC = 0 );
PROVIDE ( ISR_DMA1_Channel1 = 0 );
PROVIDE ( ISR_DMA1_Channel3_2 = 0 );
PROVIDE ( ISR_DMA1_Channel7_4 = 0 );
PROVIDE ( ISR_ADC_COMP = 0 );
PROVIDE ( ISR_LPTIM1 = 0 );
PROVIDE ( ISR_TIM2 = 0 );
PROVIDE ( ISR_TIM6_DAC = 0 );
PROVIDE ( ISR_TIM21 = 0 );
PROVIDE ( ISR_TIM22 = 0 );
PROVIDE ( ISR_I2C1 = 0 );
PROVIDE ( ISR_I2C2 = 0 );
PROVIDE ( ISR_SPI1 = 0 );
PROVIDE ( ISR_SPI2 = 0 );
PROVIDE ( ISR_USART1 = 0 );
PROVIDE ( ISR_USART2 = 0 );
PROVIDE ( ISR_LPUART1_AES_RNG = 0 );
PROVIDE ( ISR_USB = 0 );
