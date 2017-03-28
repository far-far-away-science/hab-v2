#include <crc/crc.h>
#include <signals/signals.h>
#include <stm32l0xx_hal_rcc.h>
#include <stm32l0xx_hal_crc.h>
#include <stm32l0xx_hal_crc_ex.h>

static uint16_t g_crcValue;
static CRC_HandleTypeDef g_crcHandle = { 0 };

void resetCrc(uint32_t polynomial)
{
    g_crcValue = 0;

    g_crcHandle.Instance = CRC;
    g_crcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
    g_crcHandle.Init.GeneratingPolynomial    = polynomial;
    g_crcHandle.Init.CRCLength               = CRC_POLYLENGTH_16B;
    g_crcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;
    g_crcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_HALFWORD;
    g_crcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
    g_crcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;

    if (HAL_CRC_DeInit(&g_crcHandle) != HAL_OK)
    {
        ERROR_CRC(ED_CRC_FAILED_TO_DEINITIALIZE);
    }

    if (HAL_CRC_Init(&g_crcHandle) != HAL_OK)
    {
        ERROR_CRC(ED_CRC_FAILED_TO_INITIALIZE);
    }
}

void calculateCrc(uint8_t data)
{
    g_crcValue = (uint16_t) HAL_CRC_Accumulate(&g_crcHandle, (uint32_t*) &data, 1);
}

uint16_t getCalculatedCrc()
{
    return g_crcValue;
}

void disableCrc()
{
    if (HAL_CRC_DeInit(&g_crcHandle) != HAL_OK)
    {
        ERROR_CRC(ED_CRC_FAILED_TO_DEINITIALIZE);
    }
}

void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
    __HAL_RCC_CRC_CLK_ENABLE();
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
    __HAL_RCC_CRC_CLK_DISABLE();
}
