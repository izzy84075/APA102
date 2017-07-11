#include "APA102_imp.hpp"

#include "stm32f0xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "mxconstants.h"

extern SPI_HandleTypeDef hspi1;

void APA102_SPI_TX(uint8_t *TXData, uint8_t size, bool ToBeContinued) {
    HAL_SPI_Transmit(&hspi1, TXData, size, 200);
}
