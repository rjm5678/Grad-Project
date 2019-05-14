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
#include "spi.h"
#include "24l01.h"   

//ALIENTEK Mini STM32开发板扩展实验9
//摄像头实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  

#define USE_WIFI
#define TX

//传感器名字定义
#define  OV7725 1
#define  OV7670 2

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

u8 RGB565toRGB24(u16 RGB565, u8* RGB24);

extern u8 ov_sta;	//在exit.c里面定义
extern u8 ov_frame;	//在timer.c里面定义
extern u8 CONNECTED;
u8 img[OV7725_WINDOW_WIDTH][3]={0};
u8 buf[OV7725_WINDOW_WIDTH*3]={0};

#ifdef USE_OV7725
//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j,x,y;
 	u16 color;
	u16 status;
	u16 RGB565 = 0;

	
	if(ov_sta==2)
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右 
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		OV7725_CS=0;		
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK=0;
		OV7725_RCK=1;
		OV7725_RCK=0;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK=1; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				GPIOB->CRL=0X88888888;
				OV7725_RCK=0;
				color=OV7725_DATA;	//读数据
				OV7725_RCK=1; 
				
				RGB565 = color;
				color <<= 8;  

				
				OV7725_RCK=0;
				color|=OV7725_DATA;	//读数据
				OV7725_RCK=1;
				GPIOB->CRL=0X33333333;
				LCD_WR_DATA(color); 
				
				RGB565 <<= 8;
				RGB565 = color;
				
				RGB565toRGB24(RGB565, img[j]);
				

			
			}
			for (x=0; x<320; x++)
				for (y=0; y<3; y++)
					buf[3*x+y]=img[x][y];
			M8266WIFI_SPI_Send_Data(buf, 320*3, 0, &status);
			delay_ms(5);
		}
		OV7725_CS=1;
		OV7725_RCK=0;
		OV7725_RCK=1;
		EXTI->PR=1<<15;
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 

	} 
}
#endif
 int main(void)
 {	

	u8 i = 0;
	u8 buf[1]={0}; 
	u8 send_len = 0;
	u16 status;
	u8 sensor=0;
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);
//	OV7670_Init();	
	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD
	usmart_dev.init(72);	//初始化USMART	
//	 	NRF24L01_Init();    	//初始化NRF24L01  
// 	POINT_COLOR=RED;//设置字体为红色 
	 
//		while(NRF24L01_Check())
//		{
//			
//		}
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		
#ifdef TX
		
//	NRF24L01_TX_Mode();		
		
#endif
		
		
		
	#ifdef USE_WIFI
	if(WIFI_Init())
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	
	//if(M8266WIFI_SPI_Send_Data(buf, 1, 0, &status))
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	while(1)
	{		
		M8266WIFI_SPI_RecvData(buf, 1, 1000, 0, &status);
		if(buf[0] == '0')
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		if(buf[0] == '5')
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		
		if(buf[0] == 'n')
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
		}
//		NRF24L01_TxPacket(buf);
//		LCD_ShowString(0,140,200,200,16,buf);
		
	}
	while(1);
#endif
	
#ifdef USE_OV7725
	LCD_Clear(WHITE);
	LCD_ShowString(30,50,200,200,16,"Mini STM32");	
	LCD_ShowString(30,70,200,200,16,"OV7725_OV7670 TEST");	
	LCD_ShowString(30,90,200,200,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,200,16,"2017/11/1");  	 
	LCD_ShowString(30,130,200,200,16,"Use USMART To Set!");	 
  LCD_ShowString(30,150,200,200,16,"OV7725_OV7670 Init...");	  
	while(1)//初始化OV7725_OV7670
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
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGA模式输出
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
	TIM3_Int_Init(10000,7199);			//TIM3,10Khz计数频率,1秒钟中断									  
	EXTI15_Init();						//使能定时器捕获					 	 
 	LCD_Clear(BACK_COLOR);
	while(1)
	{	
		if(sensor==OV7725)
		{
			OV7725_camera_refresh();
		}
		M8266WIFI_SPI_RecvData(buf, 1, 1000, 0, &status);
		if (buf[0] == '1')
			while(1);
 		if(i!=ov_frame)		//DS0闪烁.
		{
			i=ov_frame;
			LED0=!LED0;
 		}
	} 
#endif  


	
}

