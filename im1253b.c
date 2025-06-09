#include "sys.h"
#include "usart.h"	
#include "im1253b.h"
#include "string.h"
#include "delay.h"
#include "esp8266.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#if 1
#pragma import(__use_no_semihosting)             

#endif
 
#if EN_USART6_RX  
 

u8 USART6_RX_BUF[USART6_REC_LEN];     
unsigned  char Tx_Buffer[8];
unsigned 	long Voltage_data,Current_data,Power_data,Energy_data,Pf_data,CO2_data,frequence_data;
double Voltage,Current,RealTimePower,ActiveEnergy,electric_pfa,electric_fr,co2;


u16 USART6_RX_STA=0;    
void uart6_init(u32 bound){
   //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //Ê¹ÄÜGPIOCÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//Ê¹ÄÜUSART^Ê±ÖÓ
 
	//´®¿Ú1¶ÔÓ¦Òý½Å¸´ÓÃÓ³Éä
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA6¸´ÓÃÎªUSART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA7¸´ÓÃÎªUSART6
	
	//USART1¶Ë¿ÚÅäÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA9ÓëGPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
	GPIO_Init(GPIOC,&GPIO_InitStructure); //³õÊ¼»¯PC6 PC7

   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  USART_Init(USART6, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
	
  USART_Cmd(USART6, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï

	//Usart1 NVIC ÅäÖÃ
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

#endif
	
}

void read_data(void)
{
		

		int j;
    Tx_Buffer[0]=0x01;	 
    Tx_Buffer[1]=0x03;
    Tx_Buffer[2]=0x00;
    Tx_Buffer[3]=0x48;
    Tx_Buffer[4]=0x00;
    Tx_Buffer[5]=0x08;
    Tx_Buffer[6]=0xC4;
    Tx_Buffer[7]=0x1A;

//		usart_flag=6;
//	printf("%02X%02X%02X%02X%02X%02X%02X%02X", Tx_Buffer[0], Tx_Buffer[1], Tx_Buffer[2], Tx_Buffer[3], Tx_Buffer[4], Tx_Buffer[5], Tx_Buffer[6], Tx_Buffer[7]);

    for(j=0; j<8; j++)
    {
      USART_SendData(USART6,Tx_Buffer[j]); 
      while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);
			delay_us(10);
    }
			USART6_RX_STA=0;
}



void Analysis_data(void)
{
  
    Voltage_data=(((unsigned long)(USART6_RX_BUF[3]))<<24)|(((unsigned
                 long)(USART6_RX_BUF[4]))<<16)|(((unsigned long)(USART6_RX_BUF[5]))<<8)|USART6_RX_BUF[6];
    Current_data=(((unsigned long)(USART6_RX_BUF[7]))<<24)|(((unsigned
                 long)(USART6_RX_BUF[8]))<<16)|(((unsigned long)(USART6_RX_BUF[9]))<<8)|USART6_RX_BUF[10];
    Power_data=(((unsigned long)(USART6_RX_BUF[11]))<<24)|(((unsigned
               long)(USART6_RX_BUF[12]))<<16)|(((unsigned long)(USART6_RX_BUF[13]))<<8)|USART6_RX_BUF[14];		
    Energy_data=(((unsigned long)(USART6_RX_BUF[15]))<<24)|(((unsigned
                long)(USART6_RX_BUF[16]))<<16)|(((unsigned long)(USART6_RX_BUF[17]))<<8)|USART6_RX_BUF[18];
    Pf_data=(((unsigned long)(USART6_RX_BUF[19]))<<24)|(((unsigned
            long)(USART6_RX_BUF[20]))<<16)|(((unsigned long)(USART6_RX_BUF[21]))<<8)|USART6_RX_BUF[22];
    CO2_data=(((unsigned long)(USART6_RX_BUF[23]))<<24)|(((unsigned
             long)(USART6_RX_BUF[24]))<<16)|(((unsigned long)(USART6_RX_BUF[25]))<<8)|USART6_RX_BUF[26];
		frequence_data=(((unsigned long)(USART6_RX_BUF[31]))<<24)|(((unsigned
             long)(USART6_RX_BUF[32]))<<16)|(((unsigned long)(USART6_RX_BUF[33]))<<8)|USART6_RX_BUF[34];
		
		 
		 delay_us(10);
		 
		Voltage	=	Voltage_data*0.0001;
		Current=Current_data*0.0001;
		RealTimePower=Power_data*0.0001;
		ActiveEnergy=Energy_data*0.0001;
		electric_pfa=Pf_data*0.001;
		co2=CO2_data*0.0001;
		electric_fr=frequence_data*0.01;
	
		
}

void clear_data(void)
{
    int i=0;

    Tx_Buffer[0]=0xFF;
    Tx_Buffer[1]=0x03;
    Tx_Buffer[2]=0x00;
    Tx_Buffer[3]=0x48;
    Tx_Buffer[4]=0x00;
    Tx_Buffer[5]=0x08;
    Tx_Buffer[6]=0xD1;
    Tx_Buffer[7]=0xC4;

    for(i=0; i<8; i++)
    {
        USART_SendData(USART6,Tx_Buffer[i]);
        while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);
				delay_us(10);
    }
    USART6_RX_STA=0;
}


void USART6_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{

	u8 Res6;
#if SYSTEM_SUPPORT_OS 		//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res6 =USART_ReceiveData(USART6);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		
		if((USART6_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART6_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				if(Res6!=0x0a)USART6_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART6_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(Res6==0x0d)USART6_RX_STA|=0x4000;
				else
				{
					USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res6 ;
					USART6_RX_STA++;
					if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntExit();  											 
#endif
} 
#endif	


