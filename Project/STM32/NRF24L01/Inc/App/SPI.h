#ifndef __SPI_H
#define __SPI_H

#include "main.h"


void SPI1_Init(void);
unsigned char SPI1_RW(unsigned char TxData);

extern SPI_HandleTypeDef hspi1;

#endif
