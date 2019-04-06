#include <reg52.h>
#include "spi.h"
#include "24l01.h"
#include "led.h"

#define RX

void main()
{
	unsigned char Rxbuf[33]={1};
	unsigned char Txbuf[33] = {0};
	InitLcd1602();
	while (NRF24L01_Check())
	{
		LcdShowStr(3, 0, "NRF ERROR");	
	}
#ifdef TX	
	
	LcdShowStr(3, 0, "NRF Sending");
	NRF24L01_TX_Init();
	while(1)
	{			
		if(TX_OK == NRF24L01_TxPacket(Txbuf))
		{
			LcdShowStr(3, 0, "NRF Send OK");
			LcdShowStr(3, 1, Txbuf);
		}	

	}
#endif

#ifdef RX
	NRF24L01_RX_Init();	
	LcdShowStr(2, 0, "NRF Receving");
	
	while(1)
	{	
		if(!NRF24L01_RxPacket(Rxbuf) )
		{
			LcdShowStr(1, 0, "NRF Recevie OK");
			Rxbuf[32] = '\0';
			Rxbuf[0] += '0';
			LcdShowStr(0, 1, Rxbuf);
		}
	}
#endif	
}