#ifndef __24l01_H
#define __24l01_H
#include <reg52.h>

#define NRF_READ_REG    0x00  //
#define NRF_WRITE_REG   0x20  //



#define CONFIG          0x00
#define EN_AA			0x01
#define EN_RXADDR		0x02
#define SETUP_AW        0x03
#define SETUP_RETR      0x04
#define RF_CH           0x05  //RF¨ª¡§¦Ì¨¤,bit6:0,1¡è¡Á¡Â¨ª¡§¦Ì¨¤?¦Ì?¨º;
#define RF_SETUP        0x06
#define RX_ADDR_P0      0x0A
#define RX_PW_P0        0x11

#define STATUS          0x07
#define RX_OK			0x40
#define RD_RX_PLOAD     0x61
#define FLUSH_RX 		0xE2


#define TX_ADDR         0x10
#define WR_TX_PLOAD     0xA0
#define FLUSH_TX        0xE1

#define MAX_TX  		0x10  //¡ä?¦Ì?¡Á?¡ä¨®¡¤¡é?¨ª¡ä?¨ºy?D??
#define TX_OK   		0x20  //TX¡¤¡é?¨ª¨ª¨º3¨¦?D?

sbit NRF_IRQ = P3^2;
sbit NRF_CE = P3^7;

extern bit NRF24L01_Check(void);
extern bit NRF24L01_RxPacket(unsigned char *rxbuf);
extern void NRF24L01_RX_Init();
extern void NRF24L01_TX_Init();
extern bit NRF24L01_TxPacket(unsigned char *txbuf) ;

#endif