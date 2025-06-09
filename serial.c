#include "stm32f4xx.h "          // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h" 
#include "usart.h"	
uint16_t Serial_RxData;
uint8_t Serial_RxFlag;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#if SYSTEM_SUPPORT_OS
#include "includes.h"					 
#endif
 
#if 1
#pragma import(__use_no_semihosting)             
               
// struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout; 
// 
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 


//int fputc(int ch, FILE *f)
//{ 	
//	while((USART3->SR&0X40)==0);
//	USART3->DR = (u8) ch;      
//	return ch;
//}
#endif
 
#if EN_USART3_RX   
   	
u8 USART3_RX_BUF[USART3_REC_LEN];     

u16 USART3_RX_STA=0;  
/**
  * @brief  Serial初始化，通信协议为USART
  * @param  无
  * @retval 无
  */
void Serial_Init(u32 bound)
{
	

  	GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOC10¸??USART1_TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOC11¸??USART1_RX

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	
	//USART???
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	
  USART_Init(USART3, &USART_InitStructure); 
	
  USART_Cmd(USART3, ENABLE);  
//	USART_ClearFlag(USART3, USART_FLAG_TC);

//#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

//#endif


}

/**
  * @brief  发送一个数据
  * @param  Byte 要发送的数据
  * @retval 无
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/**
  * @brief  发送一组数据
  * @param  Array 要发送的一组数据
  * @param  Length 数组大小
  * @retval 无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
  * @brief  发送一串字符
  * @param  String 要发送的字符串
  * @retval 无
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

/**
  * @brief  Bluetooth次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t Serial_POW(uint32_t X,uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
/**
  * @brief  发送一个数字
  * @param  Number 要发送的数字
  * @param  Length 数字长度
  * @retval 无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_POW(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  printf重定向到蓝牙 
  */
int Serial_fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  * @brief  封装Sprintf,直接向蓝牙发送字符串 
  */
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}

/**
  * @brief  接收响应
  * @param  无
  * @retval 1--接收成功，0--接收失败
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  接收数据
  * @param  无
  * @retval Bluetooth_RxData 蓝牙发送过来的数据
  */
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

/**
  * @brief  接收中断函数
  * @param  无
  * @retval 无
  */
void USART3_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//{
//	Serial_RxData = USART_ReceiveData(USART3);
//	Serial_RxFlag = 1;
//	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//}
	u8 Res3;
#if SYSTEM_SUPPORT_OS 		//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res3 =USART_ReceiveData(USART3);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		
		if((USART3_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART3_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				if(Res3!=0x0a)USART3_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART3_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(Res3==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res3 ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntExit();  											 
#endif
} 
#endif	
