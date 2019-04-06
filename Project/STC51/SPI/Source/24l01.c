#include "24l01.h"
#include "SPI.h"

const unsigned char TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};
const unsigned char RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};

/****
	写函数
	向指定 reg 写长度为 len 的数据 
****/
unsigned char NRF24L01_Write_Buf(unsigned char reg, unsigned char *pbuf, unsigned char len)
{
	unsigned char status;
	unsigned char i;
	
	NRF_CSN = 0;
				
	status = SPI_RW(reg); //读取寄存器状态
	for (i=0; i<len; i++) SPI_RW(*pbuf++); //写

	NRF_CSN = 1;

	return status;
}

/****
	读函数
	向指定 reg 读长度为 len 的数据 
****/
unsigned char NRF24L01_Read_Buf(unsigned char reg, unsigned char *pbuf, unsigned char len)
{
	unsigned char status;
	unsigned char i;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //读取寄存器状态 
	for (i=0; i<len; i++) *pbuf++ = SPI_RW(0xFF); //读

	NRF_CSN = 1;

	return status;	
}

/* check nrf24l01 正确则返回0，错误返回1 */
bit NRF24L01_Check(void)
{
	unsigned char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	unsigned char i;

	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //  
	
	for(i=0;i<5;i++) if(buf[i]!=0XA5) break;	 							   
	if(i!=5)return 1;//	

	return 0;		 //	
}                      

/* 读函数 一个字节 */
unsigned char NRF24L01_Read_Reg(unsigned char reg)
{
	unsigned char status, reg_data;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //读取寄存器状态 
	reg_data = SPI_RW(0xFF); //读

	NRF_CSN = 1;

	return reg_data;		
}

/* 写函数 一个字节 */
unsigned char NRF24L01_Write_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //读取寄存器状态 
	SPI_RW(value); //写

	NRF_CSN = 1;

	return status;		
}
/* NRF24l01 RX初始化 */
void NRF24L01_RX_Init()
{
	NRF_CE=0;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(unsigned char *)RX_ADDRESS,5);//D′RX?úμ?μ??·
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //ê1?üí¨μà0μ?×??ˉó|′e    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //ê1?üí¨μà0μ??óê?μ??·  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	        //éè??RFí¨D??μ?ê		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,32);//????í¨μà0μ?óDD§êy?Y?í?è 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //éè??TX·￠é?2?êy,0db??ò?,2Mbps,μí??éù??ò??a??   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);     //?????ù±?1¤×÷?￡ê?μ?2?êy;PWR_UP,EN_CRC,16BIT_CRC,?óê??￡ê? 
  	NRF_CE=1; //CE?a??,??è??óê??￡ê? 
}

/* NRF24L01 RX， 用指针rxbuf接收， 返回0则正常 */
bit NRF24L01_RxPacket(unsigned char *rxbuf)
{
	unsigned char status;
	
	status = NRF24L01_Read_Reg(STATUS);
	if (status & RX_OK)
	{
		NRF24L01_Read_Buf (RD_RX_PLOAD, rxbuf, 32);
		NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, status);
		
		NRF24L01_Write_Reg(FLUSH_RX, 0xFF);

		return 0;
	}
	return 1;	
}

/* NRF24l01 TX初始化 */
void NRF24L01_TX_Init()
{
	NRF_CE = 0; //待机

	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);//配置发送模式
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);//使能自动应答
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);//使能接收地址
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH,40);
	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR,0x1a);//éè??×??ˉ??·￠????ê±??:500us + 86us;×?′ó×??ˉ??·￠′?êy:10′?
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);//TX地址
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, 5);//RX地址

	NRF_CE = 1;
}
/* NRF24L01 TX， 用指针rxbuf， 返回0则正常 */
bit NRF24L01_TxPacket(unsigned char *txbuf)  
{
	unsigned char sta;
  
	NRF_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf, 32);
 	NRF_CE=1;	   
	while(NRF_IRQ!=0);//μè′y·￠?ííê3é
	sta=NRF24L01_Read_Reg(STATUS);  //?áè?×′ì???′??÷μ??μ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??3yTX_DS?òMAX_RT?D??±ê??
	if(sta&MAX_TX)//′?μ?×?′ó??·￠′?êy
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//??3yTX FIFO??′??÷ 
		return MAX_TX; 
	}
	if(sta&TX_OK)//·￠?ííê3é
	{
		return TX_OK;
	}
	return 0xff;//?????-òò·￠?íê§°ü
}
                                                                                 