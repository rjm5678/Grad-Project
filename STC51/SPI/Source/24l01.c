#include "24l01.h"
#include "SPI.h"

const unsigned char TX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};
const unsigned char RX_ADDRESS[5]={0x34,0x43,0x10,0x10,0x01};

/****
	д����
	��ָ�� reg д����Ϊ len ������ 
****/
unsigned char NRF24L01_Write_Buf(unsigned char reg, unsigned char *pbuf, unsigned char len)
{
	unsigned char status;
	unsigned char i;
	
	NRF_CSN = 0;
				
	status = SPI_RW(reg); //��ȡ�Ĵ���״̬
	for (i=0; i<len; i++) SPI_RW(*pbuf++); //д

	NRF_CSN = 1;

	return status;
}

/****
	������
	��ָ�� reg ������Ϊ len ������ 
****/
unsigned char NRF24L01_Read_Buf(unsigned char reg, unsigned char *pbuf, unsigned char len)
{
	unsigned char status;
	unsigned char i;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //��ȡ�Ĵ���״̬ 
	for (i=0; i<len; i++) *pbuf++ = SPI_RW(0xFF); //��

	NRF_CSN = 1;

	return status;	
}

/* check nrf24l01 ��ȷ�򷵻�0�����󷵻�1 */
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

/* ������ һ���ֽ� */
unsigned char NRF24L01_Read_Reg(unsigned char reg)
{
	unsigned char status, reg_data;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //��ȡ�Ĵ���״̬ 
	reg_data = SPI_RW(0xFF); //��

	NRF_CSN = 1;

	return reg_data;		
}

/* д���� һ���ֽ� */
unsigned char NRF24L01_Write_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
	
	NRF_CSN = 0;

	status = SPI_RW(reg); //��ȡ�Ĵ���״̬ 
	SPI_RW(value); //д

	NRF_CSN = 1;

	return status;		
}
/* NRF24l01 RX��ʼ�� */
void NRF24L01_RX_Init()
{
	NRF_CE=0;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(unsigned char *)RX_ADDRESS,5);//D��RX?����?��??��
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //��1?�������̨�0��?��??����|��e    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //��1?�������̨�0��??����?��??��  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	        //����??RF����D??��?��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,32);//????�����̨�0��?��DD�쨺y?Y?��?�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //����??TX���騦?2?��y,0db??��?,2Mbps,�̨�??����??��??a??   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);     //?????����?1�����?�꨺?��?2?��y;PWR_UP,EN_CRC,16BIT_CRC,?����??�꨺? 
  	NRF_CE=1; //CE?a??,??��??����??�꨺? 
}

/* NRF24L01 RX�� ��ָ��rxbuf���գ� ����0������ */
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

/* NRF24l01 TX��ʼ�� */
void NRF24L01_TX_Init()
{
	NRF_CE = 0; //����

	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0E);//���÷���ģʽ
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);//ʹ���Զ�Ӧ��
	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);//ʹ�ܽ��յ�ַ
	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH,40);
	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR,0x1a);//����??��??��??����????����??:500us + 86us;��?�䨮��??��??�����?��y:10��?
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);//TX��ַ
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, 5);//RX��ַ

	NRF_CE = 1;
}
/* NRF24L01 TX�� ��ָ��rxbuf�� ����0������ */
bit NRF24L01_TxPacket(unsigned char *txbuf)  
{
	unsigned char sta;
  
	NRF_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf, 32);
 	NRF_CE=1;	   
	while(NRF_IRQ!=0);//�̨���y����?������3��
	sta=NRF24L01_Read_Reg(STATUS);  //?����?���䨬???��??�¦�??��	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??3yTX_DS?��MAX_RT?D??����??
	if(sta&MAX_TX)//��?��?��?�䨮??�����?��y
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//??3yTX FIFO??��??�� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//����?������3��
	{
		return TX_OK;
	}
	return 0xff;//?????-��������?������㨹
}
                                                                                 