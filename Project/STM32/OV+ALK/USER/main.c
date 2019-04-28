#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"
#include "usmart.h" 
#include "lcd.h" 
#include "ov7670.h" 
#include "ov7725.h"
#include "exti.h" 
#include "timer.h" 
#include "M8266WIFIDrv.h"
#include "M8266WIFI_ops.h"
#include "brd_cfg.h"
#include "misc.h"
#include "M8266HostIf.h"

//ALIENTEK Mini STM32��������չʵ��9
//����ͷʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  

#define USE_WIFI

//���������ֶ���
#define  OV7725 1
#define  OV7670 2

//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//��exit.c���涨��
extern u8 ov_frame;	//��timer.c���涨��
extern u8 CONNECTED;

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;
	if(ov_sta==2)
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������ 
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		OV7725_CS=0;		
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK=0;
		OV7725_RCK=1;
		OV7725_RCK=0;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK=1; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				GPIOB->CRL=0X88888888;
				OV7725_RCK=0;
				color=OV7725_DATA;	//������
				OV7725_RCK=1; 
				color<<=8;  
				OV7725_RCK=0;
				color|=OV7725_DATA;	//������
				OV7725_RCK=1;
				GPIOB->CRL=0X33333333;
				LCD_WR_DATA(color); 
			}
		}
		OV7725_CS=1;
		OV7725_RCK=0;
		OV7725_RCK=1;
		EXTI->PR=1<<15;
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}

 int main(void)
 {	


	u8 buf[10]="Hi"; 
	u8 send_len = 0;
	u16 status;
	//u8 sensor=0;
	//u8 i;	
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);
	//OV7670_Init();	
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD
	usmart_dev.init(72);	//��ʼ��USMART	
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	 
	#ifdef USE_WIFI
	WIFI_Init();
	while(1)
	{
		send_len = M8266WIFI_SPI_Send_Data(buf, sizeof(buf), 0, &status);
		
		delay_ms(1000);
	}
#endif
#ifdef USE_OV7725
	LCD_ShowString(30,50,200,200,16,"Mini STM32");	
	LCD_ShowString(30,70,200,200,16,"OV7725_OV7670 TEST");	
	LCD_ShowString(30,90,200,200,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,200,16,"2017/11/1");  	 
	LCD_ShowString(30,130,200,200,16,"Use USMART To Set!");	 
  LCD_ShowString(30,150,200,200,16,"OV7725_OV7670 Init...");	  
	while(1)//��ʼ��OV7725_OV7670
	{
		if(OV7725_Init()==0)
		{
			sensor=OV7725;
			LCD_ShowString(30,150,200,16,16,"OV7725 Init OK       ");				
			delay_ms(1500);
			OV7725_Light_Mode(0);
			OV7725_Color_Saturation(0);
			OV7725_Brightness(0);
			OV7725_Contrast(0);
			OV7725_Special_Effects(0);
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���
			OV7725_CS=0;
			break;
		}
		else
		{
			LCD_ShowString(30,150,200,16,16,"OV7725 Error!!");
			delay_ms(200);
			LCD_Fill(30,150,200,246,WHITE);
			delay_ms(200);
		}
	} 	  
	TIM3_Int_Init(10000,7199);			//TIM3,10Khz����Ƶ��,1�����ж�									  
	EXTI15_Init();						//ʹ�ܶ�ʱ������					 	 
 	LCD_Clear(BACK_COLOR);
	while(1)
	{	
		if(sensor==OV7725)
			OV7725_camera_refresh();
 		if(i!=ov_frame)		//DS0��˸.
		{
			i=ov_frame;
			LED0=!LED0;
 		}
	} 
#endif  


	
}

