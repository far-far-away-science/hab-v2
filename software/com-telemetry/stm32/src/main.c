#include "stm32l073xx.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_conf.h"
#include "stm32l0xx_hal_gpio_ex.h"

#include "stm32l0xx_hal_dac.h"
#include "stm32l0xx_hal_dac_ex.h"

#include "stm32l0xx_hal_uart.h"
#include "stm32l0xx_hal_uart_ex.h"

#include "uart.h"
#include "test/test.h"
#include "gps/nmea_buffer.h"
#include "aprs/aprs_board.h"

#define HALF_BUFFER_LENGTH 128
#define FULL_BUFFER_LENGTH ((HALF_BUFFER_LENGTH) * 2)

GpsData g_gpsData;
NmeaRingBuffer g_nmeaRingBuffer;
UART_HandleTypeDef g_copernicusUartHandle;

bool g_aprsMessageTransmitting;
DAC_HandleTypeDef g_hx1DacHandle;
DAC_ChannelConfTypeDef g_hx1DacConfig;
AprsEncodedMessage g_aprsEncodedMessage;
uint16_t g_DacBuffer[FULL_BUFFER_LENGTH];

void ErrorHandler(void);

void ConfigurateMcu();

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_STLink_Init(void);
void Copernicus_Uart_Init(void);

void stopHX1(void);
void HX1_Timer_Init(void);
void HX1_Dac_Init(void);

void transmitAprsMessage(void);

int main(void) {
    ConfigurateMcu();

#ifdef TEST
    EXECUTE_TESTS();

    for (;;)
    {
    }
#else
    bool hasGpsMessage = true;
    NmeaMessage nmeaMessage = { 0 };

    for (;;) {
        // TODO distinguish between modes:
        // - ascending close to the ground
        // - ascending close to balloon popping event
        // - descending close to the ground
        // - landed not moving
        // - landed moving (water landing or on the truck :) )

        if (nmeaReadMessage(&g_nmeaRingBuffer, &nmeaMessage)) {
            hasGpsMessage = true;
            parseNmeaMessageIfValid(&nmeaMessage, &g_gpsData);
        }

        // TODO get/parse telemetry data

        if (!g_aprsMessageTransmitting) {
            if (hasGpsMessage) {
                if (encodeGpsAprsMessage(&CALLSIGN_SOURCE, &g_gpsData, &g_aprsEncodedMessage)) {
                    transmitAprsMessage();
                }
            }
        }

        // TODO send aprs telemetry message
    }
#endif
}

void ConfigurateMcu(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_STLink_Init();
    Copernicus_Uart_Init();
    HX1_Dac_Init();
    HX1_Timer_Init();
}

void ErrorHandler(void)
{
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_SET);
    for (;;)
    {
    }
}

/*
 * System Clock Configuration
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI |
                                            RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1 |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    __HAL_RCC_PWR_CLK_DISABLE();

    /* LSE is configured to provide clock to the LPUART */
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin   = ERROR_LD_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(ERROR_LD_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_RESET);
}

void MX_STLink_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Copernicus_Uart_Init(void) {
    g_copernicusUartHandle.Instance                    = COPERNICUS_UART;
    g_copernicusUartHandle.Init.BaudRate               = 4800;
    g_copernicusUartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
    g_copernicusUartHandle.Init.StopBits               = UART_STOPBITS_1;
    g_copernicusUartHandle.Init.Parity                 = UART_PARITY_NONE;
    g_copernicusUartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    g_copernicusUartHandle.Init.Mode                   = UART_MODE_RX;
    g_copernicusUartHandle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    g_copernicusUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&g_copernicusUartHandle) != HAL_OK) {
        ErrorHandler();
    }

    if (EnableUart2ReceiveData(&g_copernicusUartHandle) != HAL_OK) {
        ErrorHandler();
    }
}

void HX1_Timer_Init(void) {
    const uint32_t period = (uint32_t) (SystemCoreClock / APRS_SIGNAL_GENERATION_FREQUENCY) - 1;

    static TIM_HandleTypeDef hx1TimerHandle;

    hx1TimerHandle.Instance           = HX1_TIMER;
    hx1TimerHandle.Init.Period        = period - 1;
    hx1TimerHandle.Init.Prescaler     = 0;
    hx1TimerHandle.Init.ClockDivision = 0;
    hx1TimerHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&hx1TimerHandle);

    TIM_MasterConfigTypeDef masterConfig = { 0 };
    masterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    masterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&hx1TimerHandle, &masterConfig);

    HAL_TIM_Base_Start(&hx1TimerHandle);
}

void stopHX1(void) {
    HAL_DAC_DeInit(&g_hx1DacHandle);
    g_aprsMessageTransmitting = false;
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* pDac) {
    if (g_aprsMessageTransmitting) {
        // fill in 1st half of the buffer
        g_aprsMessageTransmitting = encodeAprsMessageAsAfsk(&g_aprsEncodedMessage, g_DacBuffer, HALF_BUFFER_LENGTH);
        // continue transmission as we filled 2nd half of the buffer (this is 1/2 completion event after all)
        g_DacBuffer[0] = 1000;
        g_DacBuffer[1] = 1000;
        g_DacBuffer[2] = 1000;
        g_DacBuffer[3] = 1000;
        g_DacBuffer[0] = 0;
        g_DacBuffer[1] = 0;
        g_DacBuffer[2] = 0;
        g_DacBuffer[3] = 0;
    } else {
        stopHX1();
    }
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* pDdac) {
    if (g_aprsMessageTransmitting) {
        // fill in 2nd half of the buffer
        g_aprsMessageTransmitting = encodeAprsMessageAsAfsk(&g_aprsEncodedMessage, g_DacBuffer + HALF_BUFFER_LENGTH, HALF_BUFFER_LENGTH);
        // continue transmission as we filled 1st half of the buffer
        g_DacBuffer[4] = 3095;
        g_DacBuffer[5] = 3095;
        g_DacBuffer[6] = 3095;
        g_DacBuffer[7] = 3095;
        g_DacBuffer[4] = 4095;
        g_DacBuffer[5] = 4095;
        g_DacBuffer[6] = 4095;
        g_DacBuffer[7] = 4095;
    } else {
        stopHX1();
    }
}

void HX1_Dac_Init(void) {
    g_hx1DacHandle.Instance = HX1_DAC;
}

void transmitAprsMessage(void) {
    stopHX1();

    if (isAprsMessageEmtpy(&g_aprsEncodedMessage)) {
        return;
    }

    // TODO enable HX1

    if (HAL_DAC_Init(&g_hx1DacHandle) != HAL_OK) {
        ErrorHandler();
    }

    g_hx1DacConfig.DAC_Trigger      = HX1_DAC_TRIGGER;
    g_hx1DacConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&g_hx1DacHandle, &g_hx1DacConfig, HX1_DAC_CHANNEL) != HAL_OK) {
        ErrorHandler();
    }

    if (encodeAprsMessageAsAfsk(&g_aprsEncodedMessage, g_DacBuffer, FULL_BUFFER_LENGTH)) {
        if (HAL_DAC_Start_DMA(&g_hx1DacHandle, HX1_DAC_CHANNEL, (uint32_t*) g_DacBuffer, FULL_BUFFER_LENGTH, HX1_DAC_ALIGN) != HAL_OK) {
            stopHX1();
            ErrorHandler();
        }
        g_aprsMessageTransmitting = true;
    } else {
        stopHX1();
    }
}
