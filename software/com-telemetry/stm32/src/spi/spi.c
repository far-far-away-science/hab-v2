#include "signals/signals.h"
#include "stm32l0xx_hal.h"
#include "spi.h"
#include "mxconstants.h"

static DMA_HandleTypeDef g_dmaSpiRx;
static DMA_HandleTypeDef g_dmaSpiTx;
static SPI_HandleTypeDef g_spiInit;

void spiInit(void)
{
    SIGNAL_TIMER_CHANNEL_GPIO_PORT();
    SPI_CLK_ENABLE();

    GPIO_InitTypeDef spiGpioInit = { 0 };

    // SPI has GPIOB 3..5
    spiGpioInit.Pin  = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    spiGpioInit.Mode = GPIO_MODE_AF_PP;
    spiGpioInit.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_GPIO_PORT, &spiGpioInit);

    // Initialize SPI1 module
    g_spiInit.Instance = SPI;
    g_spiInit.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_8;
    g_spiInit.Init.CLKPhase            = SPI_PHASE_1EDGE;
    g_spiInit.Init.CLKPolarity         = SPI_POLARITY_LOW;
    g_spiInit.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;
    g_spiInit.Init.DataSize            = SPI_DATASIZE_8BIT;
    g_spiInit.Init.FirstBit            = SPI_FIRSTBIT_MSB;
    g_spiInit.Init.Mode                = SPI_MODE_MASTER;
    g_spiInit.Init.NSS                 = SPI_NSS_SOFT;
    g_spiInit.Init.TIMode              = SPI_TIMODE_DISABLE;
    if (HAL_SPI_Init(&g_spiInit) != HAL_OK)
    {
        ERROR_SPI(ED_SPI_FAILED_TO_INITIALIZE_SPI);
    }

    // Initialize DMA for SPI1
    g_dmaSpiRx.Instance                 = SPI_DMA_RX_INSTANCE;
    g_dmaSpiRx.Init.Request             = DMA_REQUEST_1;
    g_dmaSpiRx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    g_dmaSpiRx.Init.PeriphInc           = DMA_PINC_DISABLE;
    g_dmaSpiRx.Init.MemInc              = DMA_MINC_ENABLE;
    g_dmaSpiRx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dmaSpiRx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    g_dmaSpiRx.Init.Mode                = DMA_NORMAL;
    g_dmaSpiRx.Init.Priority            = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&g_dmaSpiRx) != HAL_OK)
    {
        ERROR_SPI(ED_SPI_FAILED_TO_INITIALIZE_SPI);
    }

    g_dmaSpiTx.Instance                 = SPI_DMA_TX_INSTANCE;
    g_dmaSpiTx.Init.Request             = DMA_REQUEST_1;
    g_dmaSpiTx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    g_dmaSpiTx.Init.PeriphInc           = DMA_PINC_DISABLE;
    g_dmaSpiTx.Init.MemInc              = DMA_MINC_ENABLE;
    g_dmaSpiTx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_dmaSpiTx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    g_dmaSpiTx.Init.Mode                = DMA_NORMAL;
    g_dmaSpiTx.Init.Priority            = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&g_dmaSpiTx) != HAL_OK)
    {
        ERROR_SPI(ED_SPI_FAILED_TO_INITIALIZE_SPI);
    }
}

void spiWriteReadBytes(void *inValue, const void *outValue, uint32_t length)
{
    if (HAL_SPI_TransmitReceive_DMA(&g_spiInit, (void *)outValue, inValue, length) != HAL_OK)
    {
        ERROR_SPI(ED_SPI_FAILED_TO_START_SPI_DMA);
    }
}
