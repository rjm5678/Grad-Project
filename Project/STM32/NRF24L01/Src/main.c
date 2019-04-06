#include "main.h"
#include "sys.h"
#include "led.h"
#include "spi.h"
#include "NRF24L01.h"
#include "oled.h"

#define TX

int main(void)
{
	u8 RxData=1,TxData=0,sta=0;
	
  HAL_Init();

  SystemClock_Config();
	delay_init(64);
	SPI1_Init();
  LED_Init();
  NRF24L01_Init();
	OLED_Init();
	OLED_Clear();


  while (NRF24L01_Check())
	{
		OLED_ShowString(32,0, "NRF ERROR");
  }
	OLED_ShowString(0,0, "NRF OK");
	LED1_RESET;

#ifdef TX
		NRF24L01_TX_Init();
while(1)
{
	sta = NRF24L01_TxPacket(&TxData);
	OLED_ShowString(0, 2, "Tx:");
	OLED_ShowNum(32, 2, TxData, 1, 16);
	OLED_ShowString(0, 4, "sta:");
	OLED_ShowNum(32, 4, sta, 2, 16);

}

#endif	

#ifdef RX	
	NRF24L01_RX_Init();
while(1)
{
	sta = NRF24L01_RxPacket(&RxData);
	OLED_ShowString(0, 2, "Rx:");
	OLED_ShowNum(32, 2, RxData, 1, 16);
	OLED_ShowString(0, 4, "sta:");
	OLED_ShowNum(32, 4, sta, 1, 16);

}
#endif

}
