#include "uart.h"

HAL_StatusTypeDef EnableUart2ReceiveData(UART_HandleTypeDef* pUart) {
    if((pUart->State == HAL_UART_STATE_READY) || (pUart->State == HAL_UART_STATE_BUSY_TX)) {
        __HAL_LOCK(pUart);

        UART_MASK_COMPUTATION(pUart);

        pUart->ErrorCode = HAL_UART_ERROR_NONE;
        pUart->State = HAL_UART_STATE_BUSY_RX;

        __HAL_UART_ENABLE_IT(pUart, UART_IT_PE);
        __HAL_UART_ENABLE_IT(pUart, UART_IT_ERR);
        __HAL_UNLOCK(pUart);
        __HAL_UART_ENABLE_IT(pUart, UART_IT_RXNE);

        return HAL_OK;
    } else {
        return HAL_BUSY;
    }
}

void DisableUart2ReceiveData(UART_HandleTypeDef* pUart) {
    while(HAL_IS_BIT_SET(pUart->Instance->ISR, UART_FLAG_RXNE)) { }

    __HAL_UART_DISABLE_IT(pUart, UART_IT_RXNE);

    /* Check if a transmit Process is ongoing or not */
    if(pUart->State == HAL_UART_STATE_BUSY_TX_RX) {
        pUart->State = HAL_UART_STATE_BUSY_TX;
    } else {
        /* Disable the UART Parity Error Interrupt */
        __HAL_UART_DISABLE_IT(pUart, UART_IT_PE);

        /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        __HAL_UART_DISABLE_IT(pUart, UART_IT_ERR);

        pUart->State = HAL_UART_STATE_READY;
    }

    HAL_UART_RxCpltCallback(pUart);
}

void UART_ProcessStatus(UART_HandleTypeDef* pUart) {
    /* UART parity error interrupt occurred ------------------------------------*/
    if ((__HAL_UART_GET_IT(pUart, UART_IT_PE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_PE) != RESET))
    {
        __HAL_UART_CLEAR_IT(pUart, UART_CLEAR_PEF);
        pUart->ErrorCode |= HAL_UART_ERROR_PE;
    }
    /* UART frame error interrupt occurred --------------------------------------*/
    if ((__HAL_UART_GET_IT(pUart, UART_IT_FE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_ERR) != RESET))
    {
        __HAL_UART_CLEAR_IT(pUart, UART_CLEAR_FEF);
        pUart->ErrorCode |= HAL_UART_ERROR_FE;
    }
    /* UART noise error interrupt occurred --------------------------------------*/
    if ((__HAL_UART_GET_IT(pUart, UART_IT_NE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_ERR) != RESET))
    {
        __HAL_UART_CLEAR_IT(pUart, UART_CLEAR_NEF);
        pUart->ErrorCode |= HAL_UART_ERROR_NE;
    }
    /* UART Over-Run interrupt occurred -----------------------------------------*/
    if ((__HAL_UART_GET_IT(pUart, UART_IT_ORE) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_ERR) != RESET))
    {
        __HAL_UART_CLEAR_IT(pUart, UART_CLEAR_OREF);
        pUart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    /* Call UART Error Call back function if need be --------------------------*/
    if (pUart->ErrorCode != HAL_UART_ERROR_NONE) {
        HAL_UART_ErrorCallback(pUart);
    }

    /* UART Wake Up interrupt occurred ------------------------------------------*/
    if ((__HAL_UART_GET_IT(pUart, UART_IT_WUF) != RESET) &&
        (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_WUF) != RESET))
    {
        __HAL_UART_CLEAR_IT(pUart, UART_CLEAR_WUF);
        HAL_UARTEx_WakeupCallback(pUart);
    }
}

bool UART_GetCharacter(UART_HandleTypeDef* pUart, uint8_t* pChar) {
    UART_ProcessStatus(pUart);

    /* UART in mode Receiver ---------------------------------------------------*/
    if((__HAL_UART_GET_IT(pUart, UART_IT_RXNE) != RESET) &&
       (__HAL_UART_GET_IT_SOURCE(pUart, UART_IT_RXNE) != RESET))
    {
        if((pUart->State == HAL_UART_STATE_BUSY_RX) || (pUart->State == HAL_UART_STATE_BUSY_TX_RX)) {
            *pChar = (uint8_t) (pUart->Instance->RDR & (uint8_t) pUart->Mask);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
