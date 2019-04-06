#include "main.h"

#ifndef __OLED_H
#define __OLED_H			  	 
 
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//???
#define OLED_DATA 1	//???

#define OLED_SCL_CLR HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)
#define OLED_SCL_SET HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)

#define OLED_SDIN_CLR HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
#define OLED_SDIN_SET HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)

#define OLED_DC_CLR HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET) 
#define OLED_DC_SET HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET) 

#define OLED_CS_CLR HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define OLED_CS_SET HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)


#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  				   


		   
void OLED_WR_Byte(u8 dat,u8 cmd);	    
	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
//void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



