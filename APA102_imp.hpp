#ifndef APA102_IMP_H
#define APA102_IMP_H

#include <stdint.h>

void APA102_SPI_TX(uint8_t *TXData, uint8_t size, bool ToBeContinued);

#endif
