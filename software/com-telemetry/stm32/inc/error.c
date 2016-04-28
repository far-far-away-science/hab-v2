#include "errors.h"

#include "mxconstants.h"

void TurnOnGreenLed(void)
{
    HAL_GPIO_WritePin(ERROR_LD_GPIO_Port, ERROR_LD_Pin, GPIO_PIN_SET);
}
