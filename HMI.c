#include "HMI.h" 

#include "delay.h" 
#include "usart.h" 

typedef unsigned          char uint8_t;

void HMISends(char *buf1)		  
{
	u8 i=0;
	while(1)
	{
		if(buf1[i] != 0)
	 	{
			USART_SendData(USART1,buf1[i]);  //·¢ËÍÒ»¸ö×Ö½Ú
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//µÈ´ý·¢ËÍ½áÊø
		 	i++;
		}
		else
		{
			return ;
		}
	}
}

void HMISendstart(void)
	{
	 	delay_ms(200);
		HMISend(0xff);
		delay_ms(200);
	}
	
void HMISend(uint8_t k)            //ÓÃÓÚ×Ö½Ú·¢ËÍº¯Êý0xff
{   
    uint8_t i; 
    for(i=0;i<3;i++)
    {
        if(k!=0)
        {  
					USART_SendData(USART1, k);//Ïò´®¿Ú1·¢ËÍÊý¾Ý
					delay_ms(100);
					while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET){};//µÈ´ý·¢ËÍ½áÊø  
          delay_ms(100);
        }
					else	
					{
						return ;
					}		
		}   
}


