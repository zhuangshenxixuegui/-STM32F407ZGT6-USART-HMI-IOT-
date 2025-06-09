#include "sys.h"
#include "esp8266.h"
#include <string.h>
#include "delay.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

unsigned char Property_Data[2];//


//extern const char* WIFI ="iPhone";
//extern const char* WIFIASSWORD="66666666";
extern const char* WIFI ="Redmi K40";
extern const char* WIFIASSWORD="a1458769";
extern const char* ClintID="k1futjFUIht.openmv|securemode=2\\,signmethod=hmacsha256\\,timestamp=1718207470428|";
extern const char* username="openmv&k1futjFUIht";
extern const char* passwd="4872025fb2c76f1f13ebffeb13b3cadf12d56af05a063efdf980f241f91e2c9a";
extern const char* Url="iot-06z00dlpgothqe0.mqtt.iothub.aliyuncs.com";
extern const char* ID="openmv";
extern const char* subtopic="/sys/k1futjFUIht/openmv/thing/event/property/post_reply";

#if SYSTEM_SUPPORT_OS
#include "includes.h"			  
#endif
 
#if 1
#pragma import(__use_no_semihosting)             
    
int usart_flag=2;

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	if(usart_flag==1)
	{
	while((USART1->SR&0X40)==0);  
	USART1->DR = (u8) ch;      
	
	}
	
	if(usart_flag==2)
	{
	while((USART2->SR&0X40)==0);  
	USART2->DR = (u8) ch;      
	}
	if(usart_flag==3)
	{
	while((USART3->SR&0X40)==0);  
	USART3->DR = (u8) ch;      
	}
	if(usart_flag==6)
	{
	while((USART6->SR&0X40)==0);  
	USART6->DR = (u8) ch;      
	}
	return ch;
}
#endif
 
#if EN_USART2_RX   
 	
uint8_t USART2_RX_BUF[USART2_REC_LEN];    

u16 USART2_RX_STA=0;      



void esp_Init(void)
{
	
	memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
	usart_flag=2;
	printf("AT+RST\r\n");  //??
	delay_ms(1000);
	
	usart_flag=2;
	printf("ATE0\r\n");    //????
	delay_ms(1000);
	
	
	usart_flag=2;
	printf("AT+CWMODE=1\r\n"); //Station
	delay_ms(1000);


	usart_flag=2;
	printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD); 
	delay_ms(3000);

	usart_flag=2;
	printf("AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ID,username,passwd);//AT+MQTTUSERCFG=0,1,"openmv","openmv&k17pbJQUojP","485b68e58fd25ad16f6d488b3e30566782456904e784ea9a3753cc62a285268a",0,0,""
	delay_ms(2000);
	printf("%s\r\n",MQTT_Client); // AT+MQTTCLIENTID=0,"k17pbJQUojP.openmv|securemode=2\,signmethod=hmacsha256\,timestamp=1715479345787|"
	delay_ms(2000);
	printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url);   // AT+MQTTCONN=0,"iot-06z00gtz069jwyp.mqtt.iothub.aliyuncs.com",1883,1
	delay_ms(2000);

	memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));

	
}


void uart2_init(u32 bound){
   //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//Ê¹ÄÜUSART2Ê±ÖÓ
 
	//´®¿Ú1¶ÔÓ¦Òý½Å¸´ÓÃÓ³Éä
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9¸´ÓÃÎªUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10¸´ÓÃÎªUSART1
	
	//USART1¶Ë¿ÚÅäÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9ÓëGPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊ¼»¯PA9£¬PA10

   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú2
	
  USART_Cmd(USART2, ENABLE);  //Ê¹ÄÜ´®¿Ú2 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï

	//Usart1 NVIC ÅäÖÃ
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
#endif
	
}


void USART2_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	u8 Res2;
#if SYSTEM_SUPPORT_OS 		//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
	{
		Res2 =USART_ReceiveData(USART2);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		
		if((USART2_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART2_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
			{
				if(Res2!=0x0a)USART2_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART2_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 

			}
			else //»¹Ã»ÊÕµ½0X0D
			{	
				if(Res2==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res2 ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòÐèÒªÖ§³ÖOS.
	OSIntExit();  											 
#endif
} 

void ESP8266_Received(char *PRO)
{
    unsigned char *ret = 0;
    char *property = 0;
    unsigned char i;
    if(PRO == NULL);
    else if(USART2_RX_STA & 0x8000)			//´®¿Ú2½ÓÊÕÍêÒ»Ö¡Êý¾Ý
    {
        ret = USART2_RX_BUF;
        if(ret != 0)
        {
            property = strstr((const char *)ret, (const char *)PRO);
            if(property != NULL)
            {
                for(i = 0; i < 2; i++)
                {
                    Property_Data[i] = *(property + strlen(PRO) + 2 + i);
                }
                if(Property_Data[1] == '}')Property_Data[1] = 0;
                USART2_RX_STA = 0;
            }
            else
            {
                USART2_RX_STA = 0;
            }
        }
						else
        {
								USART2_RX_STA = 0;
        }
    }
		
}



#endif	

 



