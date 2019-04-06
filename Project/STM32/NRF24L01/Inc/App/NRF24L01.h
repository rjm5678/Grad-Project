#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "main.h"

//24L01²Ù×÷Ïß
//#define NRF_CE     PAout(4)  //24L01Æ¬Ñ¡ÐÅºÅ
//#define NRF_CSN    PCout(4)  //SPIÆ¬Ñ¡ÐÅºÅ	   
//#define NRF_IRQ    PCin(5)   //IRQÖ÷»úÊý¾ÝÊäÈë

#define NRF_READ_REG    0x00  //
#define NRF_WRITE_REG   0x20  //

#define CONFIG          0x00
#define EN_AA						0x01
#define EN_RXADDR				0x02
#define SETUP_AW        0x03
#define SETUP_RETR      0x04
#define RF_CH           0x05  //RFí¨µà,bit6:0,1¤×÷í¨µà?µ?ê;
#define RF_SETUP        0x06
#define RX_ADDR_P0      0x0A
#define RX_PW_P0        0x11

#define STATUS          0x07
#define RX_OK						0x40
#define RD_RX_PLOAD     0x61
#define FLUSH_RX 				0xE2


#define TX_ADDR         0x10
#define WR_TX_PLOAD     0xA0
#define FLUSH_TX        0xE1

#define MAX_TX  		0x10  //´ïµ½×î´ó·¢ËÍ´ÎÊýÖÐ¶Ï
#define TX_OK   		0x20  //TX·¢ËÍÍê³ÉÖÐ¶Ï

#define NRF_CSN_CLR		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)
#define NRF_CSN_SET 	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)

#define NRF_CE_CLR		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define NRF_CE_SET 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define NRF_IRQ				HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)



void NRF24L01_Init(void);
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len);
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len);
u8 NRF24L01_Check(void);
u8 NRF24L01_RxPacket(u8 *rxbuf);
u8 NRF24L01_TxPacket(u8 *txbuf);
void NRF24L01_RX_Init(void);
void NRF24L01_TX_Init(void);


#endif
