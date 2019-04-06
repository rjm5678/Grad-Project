#ifndef __SPI_H
#define __SPI_H
#include <reg52.h>

sbit MOSI 	 = 	P1^2;
sbit MISO  	 = 	P1^3;
sbit NRF_SCK = 	P3^5;
sbit NRF_CSN =  P1^7;

extern unsigned char SPI_RW(unsigned char data_write);

#endif