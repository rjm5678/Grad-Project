#include "SPI.h"

/* 读一个字节， 写一个字节 */
unsigned char SPI_RW(unsigned char TxData)
{
	unsigned char mask = 0;
	unsigned char RxData=0;

	for (mask=0x80; mask!=0; mask>>=1)
	{		
		MOSI = mask&TxData;

		NRF_SCK = 1;
		if (MISO)  RxData |= mask;

		NRF_SCK = 0; 
	}


	return RxData;
}
/*
unsigned char SPI_RW(unsigned char byte)
{
	unsigned char bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
	{
		MOSI = (byte & 0x80); // output 'byte', MSB to MOSI
		byte = (byte << 1); // shift next bit into MSB..
		NRF_SCK = 1; // Set SCK high..
		byte |= MISO; // capture current MISO bit
		NRF_SCK = 0; // ..then set SCK low again
	}
return(byte); // return read byte
}		 */
