/********************************************************************
 * M8266HostIf.c
 * .Description
 *     Source file of M8266WIFI Host Interface 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#include "main.h"
#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"	
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
//#include "oled.h"


u8 CONNECTED = 0;
SPI_HandleTypeDef   hspi2;
/***********************************************************************************
 * M8266HostIf_GPIO_SPInCS_nRESET_Pin_Init                                         *
 * Description                                                                     *
 *    To initialise the GPIOs for SPI nCS and nRESET output for M8266WIFI module   *
 *    You may update the macros of GPIO PINs usages for nRESET from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_GPIO_CS_RESET_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

	//Initial STM32's GPIO for M8266WIFI_SPI_nCS
	__HAL_RCC_GPIOB_CLK_ENABLE(); 														// enable nCS GPIO clock
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);		// nCS output high initially	
		/*Configure GPIO pin : PB12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//Initial STM32's GPIO for M8266WIFI_nRESET
  __HAL_RCC_GPIOA_CLK_ENABLE();  															// enable nRESET GPIO clock
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);		  // nRESET output high initially	
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
/***********************************************************************************
 * M8266HostIf_SPI_Init                                                            *
 * Description                                                                     *
 *    To initialise the SPI Interface for M8266WIFI module                         *
 *    You may update the macros of SPI usages for nRESET from brd_cfg.h            *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
#if defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) || defined(MCU_IS_STM32H7XX)
	SPI_HandleTypeDef hspi;
#endif
void M8266HostIf_SPI_Init(void)
{
////////////////////////////////////////////////////////////////////////////////////
///////////////  STM32F1  //////////////////////////////////////////////////////////
#if 	defined(MCU_IS_STM32F1XX)
  GPIO_InitTypeDef  GPIO_InitStruct;

#if (M8266WIFI_SPI_INTERFACE_NO == 1)  
	#if 1 // if use GPIOA5/6/7 for SPI1 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);										// no need to Enable GPIO Alternate for SPI1, since GPIOA5/6/7 for SPI1 is the default alternative
	#elif 0 // if use GPIOB3/4/5 for SPI1 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI1 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);											// need to Enable GPIO Alternate for SPI1, since GPIOB3/4/5 for SPI1 is not the default alternative
	#else
	#error You should specify the GPIO pins used on STM32F1 SPI1 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);						  //  Enable SPI1 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);							//  Start to Reset SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);							//  Stop reseting SPI1	
	
	// end of if (M8266WIFI_SPI_INTERFACE_NO == 1) for STM32F1

#elif (M8266WIFI_SPI_INTERFACE_NO == 2)  
	#if 1 // if use GPIOB13/14/15 for SPI2 SCK/MISO/MOSI
			__HAL_RCC_SPI2_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();  					// enable the SPI2 GPIO CLOCK
		
		GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//GPIO_PinRemapConfig(GPIO_Remap_SPI2, ENABLE);										//  no need to Enable GPIO Alternate for SPI2, since GPIOB13/14/15 for SPI2 is the default alternative
	#else  // STM32F10x has only GPIOB13/14/15 mapped for SPI2
	#error You should specify the GPIO pins used on STM32F1 SPI2 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

		/*Configure GPIO pin : PC4 */
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// end of if (M8266WIFI_SPI_INTERFACE_NO == 2) for STM32F1
	
#elif (M8266WIFI_SPI_INTERFACE_NO == 3)  
	#if 1 // if use GPIOB3/4/5 for SPI3 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI3 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);														
	//GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);										//  no need to Enable GPIO Alternate for SPI3, since GPIOB3/4/5 for SPI3 is the default alternative

	#elif 0 // if use GPIOC10/11/12 for SPI3 SCK/MISO/MOSI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 						// enable the SPI2 GPIO CLOCK
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;									// SET GPIOs for SPI2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);										  //  need to Enable GPIO Alternate for SPI3, since GPIOC10/11/12 for SPI3 is not the default alternative
	#else  // STM32F10x has only GPIOB13/14/15 mapped for SPI2
	#error You should specify the GPIO pins used on STM32F1 SPI3 in function M8266HostIf_SPI_Init() fromf the file M8266HostIf_STM32Fxx.c
	#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);						  //  Enable SPI2 CLOCK
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);							//  Start to Reset SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);							//  Stop reseting SPI2

	// end of if (M8266WIFI_SPI_INTERFACE_NO == 3) for STM32F1
		
#else
#error Invalid M8266WIFI_SPI_INTERFACE_NO defined in brd_cfg.h 
#endif

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
	 if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {

  }															//	Enable SPI Interface
	//M8266HostIf_SPI_ReadWriteByte(0xff);																	//  Start Trasmission	
#ifdef M8266WIFI_SPI_ACCESS_USE_DMA	
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Rx, ENABLE);			//  Enable SPI Rx DMA
	SPI_I2S_DMACmd(M8266WIFI_INTERFACE_SPI, SPI_I2S_DMAReq_Tx, ENABLE);			//  Enable SPI Tx DMA
#endif
		
#endif
	
}
/***********************************************************************************
 * M8266HostIf_SPI_SetSpeed                                                        *
 * Description                                                                     *
 *    To setup the SPI Clock Speed for M8266WIFI module                            *
 * Parameter(s):                                                                   *
 *    SPI_BaudRatePrescaler: SPI BaudRate Prescaler                                *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_SPI_SetSpeed(u32 SPI_BaudRatePrescaler)
{
#if  defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)	
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	M8266WIFI_INTERFACE_SPI->CR1&=0XFFC7;
	M8266WIFI_INTERFACE_SPI->CR1|=SPI_BaudRatePrescaler;
	//SPI_Cmd(M8266WIFI_INTERFACE_SPI,ENABLE);
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {

  }	
#elif defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
	__HAL_SPI_DISABLE(&hspi);
  hspi.Instance->CR1&=0XFFC7;
  hspi.Instance->CR1|=SPI_BaudRatePrescaler;
  __HAL_SPI_ENABLE(&hspi);
#elif defined(MCU_IS_STM32H7XX)
	__HAL_SPI_DISABLE(&hspi);
	hspi.Instance->CFG1 &= ~(7UL<<28);
	hspi.Instance->CFG1 |= SPI_BaudRatePrescaler;
  __HAL_SPI_ENABLE(&hspi);
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif	
} 

/***********************************************************************************
 * M8266HostIf_Init                                                                *
 * Description                                                                     *
 *    To initialise the Host interface for M8266WIFI module                        *
 * Parameter(s):                                                                   *
 *    baud: baud rate to set                                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/ 
void M8266HostIf_Init(void)
{
	 M8266HostIf_GPIO_CS_RESET_Init();
	
	 M8266HostIf_SPI_Init();
//	 M8266HostIf_SPI_SetSpeed(SPI_BaudRatePrescaler_8);
	
}

//////////////////////////////////////////////////////////////////////////////////////
// BELOW FUNCTIONS ARE REQUIRED BY M8266WIFIDRV.LIB. 
// PLEASE IMPLEMENTE THEM ACCORDING TO YOUR HARDWARE
//////////////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
 * M8266HostIf_Set_nRESET_Pin                                                      *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI nRESET                *
 *    You may update the macros of GPIO PIN usages for nRESET from brd_cfg.h       *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to nRESET pin                                         *
 *              0 = output LOW  onto nRESET                                        *
 *              1 = output HIGH onto nRESET                                        *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_nRESET_Pin(u8 level)
{
	  if(level!=0)
		//GPIO_SetBits(M8266WIFI_nRESET_GPIO, M8266WIFI_nRESET_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_nRESET_GPIO->BSRRL = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif

		else
		//GPIO_ResetBits(M8266WIFI_nRESET_GPIO, M8266WIFI_nRESET_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_nRESET_GPIO->BRR   = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_nRESET_GPIO->BSRRH = M8266WIFI_nRESET_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
		  M8266WIFI_nRESET_GPIO->BSRR  = M8266WIFI_nRESET_PIN<<16;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif
}
/***********************************************************************************
 * M8266HostIf_Set_SPI_nCS_PIN                                                     *
 * Description                                                                     *
 *    To Outpout HIGH or LOW onto the GPIO pin for M8266WIFI SPI nCS               *
 *    You may update the macros of GPIO PIN usages for SPI nCS from brd_cfg.h      *
 *    You are not recommended to modify codes below please                         *
 * Parameter(s):                                                                   *
 *    1. level: LEVEL output to SPI nCS pin                                        *
 *              0 = output LOW  onto SPI nCS                                       *
 *              1 = output HIGH onto SPI nCS                                       *
 * Return:                                                                         *
 *    None                                                                         *
 ***********************************************************************************/
void M8266HostIf_Set_SPI_nCS_Pin(u8 level)
{
	  if(level!=0)
		//GPIO_SetBits(M8266WIFI_SPI_nCS_GPIO, M8266WIFI_SPI_nCS_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRRL = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) 
		  M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN;
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif

		else
		//GPIO_ResetBits(M8266WIFI_SPI_nCS_GPIO, M8266WIFI_SPI_nCS_PIN);
#if   defined(MCU_IS_STM32F1XX)
		  M8266WIFI_SPI_nCS_GPIO->BRR   = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX) || defined(MCU_IS_STM32H7XX)
		  M8266WIFI_SPI_nCS_GPIO->BSRRH = M8266WIFI_SPI_nCS_PIN;
#elif defined(MCU_IS_STM32F3XX) || defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX)
      M8266WIFI_SPI_nCS_GPIO->BSRR  = M8266WIFI_SPI_nCS_PIN<<16;		
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif
}

/***********************************************************************************
 * M8266WIFIHostIf_delay_us                                                        *
 * Description                                                                     *
 *    To loop delay some micro seconds.                                            *
 * Parameter(s):                                                                   *
 *    1. nus: the micro seconds to delay                                           *
 * Return:                                                                         *
 *    none                                                                         *
 ***********************************************************************************/
void M8266HostIf_delay_us(u8 nus)
{
   delay_us(nus);
}

/***********************************************************************************
 * M8266HostIf_SPI_ReadWriteByte                                                   *
 * Description                                                                     *
 *    To write a byte onto the SPI bus from MCU MOSI to the M8266WIFI module       *
 *    and read back a byte from the SPI bus MISO meanwhile                         *
 *    You may update the macros of SPI usage from brd_cfg.h                        *
 * Parameter(s):                                                                   *
 *    1. TxdByte: the byte to be sent over MOSI                                    *
 * Return:                                                                         *
 *    1. The byte read back from MOSI meanwhile                                    *                                                                         *
 ***********************************************************************************/
u8 M8266HostIf_SPI_ReadWriteByte(u8 TxdByte)
{
#if  defined(MCU_IS_STM32F1XX) || defined(MCU_IS_STM32F2XX) || defined(MCU_IS_STM32F4XX)	
//	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_TXE) == RESET){} 	//wait SPI TXD Buffer Empty
//	SPI_I2S_SendData(M8266WIFI_INTERFACE_SPI, TxdByte); 			//Write the byte to the TXD buffer and then shift out via MOSI
//	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_RXNE) == RESET){} 	//wait the SPI RXD buffer has received the data from MISO
//	return SPI_I2S_ReceiveData(M8266WIFI_INTERFACE_SPI);  		//return the data received from MISO
	u8 RxData;
	
	HAL_SPI_TransmitReceive(&hspi2, &TxdByte, &RxData, 1, 1000);
	
	return RxData;
#elif defined(MCU_IS_STM32F3XX)	
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_TXE) == RESET){} 	//wait SPI TXD Buffer Empty
	SPI_SendData8(M8266WIFI_INTERFACE_SPI, TxdByte); 			//Write the byte to the TXD buffer and then shift out via MOSI
	while(SPI_I2S_GetFlagStatus(M8266WIFI_INTERFACE_SPI, SPI_I2S_FLAG_RXNE) == RESET){} 	//wait the SPI RXD buffer has received the data from MISO
	return SPI_ReceiveData8(M8266WIFI_INTERFACE_SPI);  		//return the data received from MISO
#elif defined(MCU_IS_STM32F7XX) || defined(MCU_IS_STM32L1XX) || defined(MCU_IS_STM32L4XX) || defined(MCU_IS_STM32H7XX)
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&hspi,&TxdByte,&Rxdata,1, 1000);       
 	return Rxdata; 
#else
#error YOU SHOULD DEFINED MCU_IS_STM32F?XX in brd_cfg.h
#endif	
}
/***********************************************************************************
 * WIFI Init                                                                       *
 * Description                                                                     *
 *    Set AP Mode                                                           	     *
 *    Set Udp Mode                                                           	     *
 *    SSID: Anylinkin                                                       	     *
 *    PSW:  1234567890                                                       	     *
 * Parameter(s):                                                                   *
 *    None                                                                         *
 * Return:                                                                         *
 *    0. failed                                                                    *
 *    1. success                                                                   *
 ***********************************************************************************/
u8 WIFI_Init(void)
{
	u8 success=0;

	char sta_ip[10]={0};
	u16 status;
	
//	LCD_ShowString(0,0,200,200,16,"ALK Init...");

	M8266HostIf_Init();

	/************************ Init WIFI Module via SPI ************************/
	if(M8266WIFI_Module_Init_Via_SPI())
//			OLED_ShowString(0, 2, "SPI success");
	;
	else
	{		//OLED_ShowString(0, 2, "Failed");
//		LCD_ShowString(0,20,200,200,16,"ALK Init SPI error");
//		return 0;
	}
	
	/*********************** Set STA Mode ****************************/
	if(M8266WIFI_SPI_Set_Opmode(STA_MODE, UNSAVED, &status))
//	OLED_ShowString(0, 4, "Set success");
	;
	else
	{
//		LCD_ShowString(0,40,200,200,16,"WIFI Set STA Mode error");
		return 0;
	}
	
	/***************************** STA Connect to Ap *************************************/
	success = M8266WIFI_SPI_STA_Connect_Ap("RJM", "1234567890", UNSAVED, 10, &status);
	if(success)
	{
//		M8266WIFI_SPI_Get_STA_Connection_Status(&connect_status, &status);
//		if (connect_status == 5)
//			LCD_ShowString(0,60,200,200,16,"STA Connect To Ap success");	
		
		
	}
	else
	{
//		LCD_ShowString(0,60,200,200,16,"STA Connect To Ap faied");
//		LCD_ShowxNum(0,80,status,10,16,0);	
		return 0;		
	}
		if(M8266WIFI_SPI_Get_STA_IP_Addr(sta_ip, &status))
//			LCD_ShowString(0,100,200,200,16,sta_ip);
	
	
	
	success = M8266WIFI_SPI_Setup_Connection(UDP, 9090, "192.168.137.1", 9090, 0, 255, &status);
	if(success)
	{
//		OLED_ShowString(0, 6, "Connected");
		CONNECTED = 1;
	}
	else
	{
//		LCD_ShowString(0,120,200,200,16,"WIFI Connect Faied");
		return 0;
	}
	return 1;
}
