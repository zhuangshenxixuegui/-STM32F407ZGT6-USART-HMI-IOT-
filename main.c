#include "stm32f4xx.h "  
#include "sys.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "adc.h"
#include "HMI.h" 
#include "myiic.h"
#include "Serial.h"  
#include "pcf8591.h"
#include "Servo.h"
#include "relay.h" 
#include "esp8266.h"	
#include "im1253b.h"	
#include "math.h"
float AD1,AD2,AD3,AD4,Val,Acurrent,AP,Ls,AL;   //¶æ»ú½Ç¶È£¬¹âÕÕÖµ
float adc_Data[10];		
int i,j,x,y;      																						//forÑ­»·
int h=0;
int m=0;
int n=0;
uint16_t  adc1,adc2,adc3,adc4;    														//AD²É¼¯µÄadcµÄÖµ
uint16_t l,r,u,d;   																					//×óÓÒÉÏÏÂ
uint16_t AT;																									
uint16_t cnt;																									//¼ÆÊýÆ÷
char SendString[100];																					//À¶ÑÀ
u8 t;
u8 len,Byte,ly;	
u16 times=0; 
int Temperature;																							//ÎÂ¶È
int LightLux;																									//¹âÕÕÇ¿¶È
int LightVolt;																								//µçÑ¹
uint8_t Tdetect;																			
uint8_t Vdetect;																							//µçÑ¹¼ì²â(func5)
uint8_t servo2;																								//¶æ»ú2   func3
uint8_t servo1;																								//¶æ»ú1   func2
uint8_t fan;																									//·çÉÈ
extern double  Voltage,Current,RealTimePower,ActiveEnergy,electric_pfa,electric_fr,co2;

const char* func1="temperature";															//ÎÂ¶È
const char* func2="LightLux";																	//¹âÕÕÇ¿¶È
const char* func3="LightVolt";																//µ¥Æ¬»ú¹¤×÷µçÑ¹
const char* func6= "servo1";
const char* func7 ="servo2";

const char*	func10="Voltage";																	//Êä³öµçÑ¹
const char*	func11="Current";																	//Êä³öµçÁ÷
const char*	func12="RealTimePower";														//ÊµÊ±¹¦ÂÊ
const char*	func13="ActiveEnergy";														//µçÄÜ
const char*	func14="electric_pfa";														//¹¦ÂÊÒòËØ
const char*	func15="co2";																			//¶þÑõ»¯Ì¼Å¨¶È
const char*	func16="electric_fr";															//ÆµÂÊ
const char* pubtopic="/sys/k1futjFUIht/openmv/thing/event/property/post";
extern unsigned 	long Voltage_data,Current_data,Power_data,Energy_data,Pf_data,CO2_data;
float Angle=20;
float Angle1=20;

int main(void)
{ 
	
	
	TIM4_PWM_Init(20000 - 1,72 - 1);  									//pwmÊä³ö³õÊ¼»¯
	Servo_Init();																				//¶æ»ú³õÊ¼»¯
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//ÉèÖÃÏµÍ³ÖÐ¶ÏÓÅÏÈ¼¶·Ö×é2
	delay_init(168);  																	//³õÊ¼»¯ÑÓÊ±º¯Êý
	uart_init(115200);																	//³õÊ¼»¯´®¿Ú²¨ÌØÂÊÎª115200
	Adc_Init();         																//³õÊ¼»¯ADC
	HMISendstart();																			//´®¿ÚÆÁ³õÊ¼»¯
	pcf8591_Init();																			//pcf³õÊ¼»¯

	Serial_Init(115200);																//À¶ÑÀ³õÊ¼»¯£¨´®¿Ú3£©
	Relay_Init();																				//¼ÌµçÆ÷³õÊ¼»¯
	uart2_init(115200);																	//´®¿Ú2³õÊ¼»¯£¨WIFI£©
	uart6_init(4800);																  	//´®¿Ú6³õÊ¼»¯ÒªÉèÖÃÎª4800
	read_data();																				//Ïòim1253Ä£¿é·¢ËÍÊý¾ÝËû»áÒ»Ö±·µ»Ø
	esp_Init();																					//Ïòesp8266·¢ËÍATÖ¸ÁîÐèµÈ´ý10Ãë


	
   while(1) 
	{	 
		

h++;
m++;
if((h==100)||(USART3_RX_BUF[0]==0x66)||(Property_Data[0] == '7'))
{	
		read_data();
		delay_ms(10);
		Analysis_data();
//		delay_ms(10);
		usart_flag=2;
		delay_ms(10);
		printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%1f}}\",0,0\r\n",pubtopic,func10,Voltage,func11,Current,func12,RealTimePower,func13,ActiveEnergy);
		delay_ms(10);
		printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%f}}\",0,0\r\n",pubtopic,func14,electric_pfa,func15,co2,func16,electric_fr,func2,Ls);
//		delay_ms(10);
//		printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%1f\\,\\\"%s\\\":%f}}\",0,0\r\n",pubtopic,func7,Angle1,func6,Angle,func2,Ls,func2,Ls);
		h=0; 
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));		
		delay_ms(10);				
		memset(Property_Data,0,2);	
}


	
//********************************ADC²É¼¯****************************		
		adc1=Get_Adc_Average(ADC_Channel_4,2);//»ñÈ¡Í¨µÀ5µÄ×ª»»Öµ£¬2´ÎÈ¡Æ½¾ù
		adc2=Get_Adc_Average(ADC_Channel_5,2);
		adc3=Get_Adc_Average(ADC_Channel_6,2);
		adc4=Get_Adc_Average(ADC_Channel_7,2);
		
		AD1=adc1;
		AD2=adc2;
		AD3=adc3;
		AD4=adc4;	
		Ls=(adc1+adc2+adc3+adc4)/4;
//*****************************PCF8591Ä£¿é****************************************************************

//***********´®¿ÚÆÁ°´ÏÂ×Ô¶¯¼ì²â²Å¿ªÊ¼¸ø¶¨Ò»¸öãÐÖµ£¨ºóÐø»¹Ðèµ÷ÊÔ£©***************************************

							if(AD1<5000&&AD1>3000){
											AD1=3000;
										}
							if(AD2<5000&&AD2>3000){
											AD2=3000;
										}					
							if(AD3<5000&&AD3>3000){
											AD3=3000;
										}
							if(AD4<5000&&AD4>3000){
											AD4=3000;
										}
							if(AD1<3000&&AD1>2000){
											AD1=2500;
										}
							if(AD2<3000&&AD2>2000){
											AD2=2500;
										}				
							if(AD3<3000&&AD3>2000){
											AD3=2500;
										}
							if(AD4<3000&&AD4>2000){
											AD4=2500;
										}
							if(AD1<2000&&AD1>1000){
											AD1=1500;
										}
							if(AD2<2000&&AD2>1000){
											AD2=1500;
										}				
							if(AD3<2000&&AD3>1000){
											AD3=1500;
										}
							if(AD4<2000&&AD4>1000){
											AD4=1500;
										}
								
							if(AD1<1000){
											AD1=500;
										}
							if(AD2<1000){
											AD2=500;
										}							
							if(AD3<1000){
											AD3=500;
										}
							if(AD4<1000){
											AD4=500;
										}
			
//*************************ESP8266Ä£¿é*******************************************************************

		delay_ms(10);																			//±ØÐëÒªÓÐÑÓÊ±		
		ESP8266_Received(func9);													
		if((Property_Data[0] == '2')||(USART3_RX_BUF[0]==0x70))	
	{
		Relay1_ON();
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	
		delay_ms(10);				
		memset(Property_Data,0,2);		
	}
		if((Property_Data[0] == '1')||(USART3_RX_BUF[0]==0x69))	
	{

		Relay1_OFF();
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	
		delay_ms(10);				
		memset(Property_Data,0,2);	
	}		

	


	if(Property_Data[0] == '3')	
	{
		u=1;
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));		
		delay_ms(10);				
		memset(Property_Data,0,2);	
	}
		if(Property_Data[0] == '4')	
	{
		d=1;
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));			
		memset(Property_Data,0,2);	
	}	


													//Ö¸¶¨¶ÁÈ¡µÄ¹¦ÄÜ6¶æ»ú2
	if(Property_Data[0] == '5')	
	{
		l=1;
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));		
		delay_ms(10);				
		memset(Property_Data,0,2);	
				
	}
		if(Property_Data[0] == '6')	
	{
		r=1;
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));		
		delay_ms(10);				
		memset(Property_Data,0,2);	
	}	
	
											

	


	if((Property_Data[0] == '8')||(USART_RX_BUF[0]==0x37)||(USART3_RX_BUF[0]==0x68))		
{
		Relay2_ON();
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
		delay_ms(10);		
		memset(Property_Data,0,2);		
	
}	
	
	if((Property_Data[0] == '9')||(USART_RX_BUF[0]==0x38)||(USART3_RX_BUF[0]==0x67))		
{

		Relay2_OFF();
		memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));	
		delay_ms(10);		
		memset(Property_Data,0,2);	
}	
	
      	            		
		delay_ms(10);
//**********************À¶ÑÀÄ£¿é*********************************************************************************
	

//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART3, USART3_RX_BUF[t]);         //Ïò´®¿Ú1·¢ËÍÊý¾Ý
//				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
//			}
		if(USART3_RX_BUF[0]==0x70)
		{
			ly=1;
			n=2;
		}
			if(ly==1)	//À¶ÑÀ¿ØÖÆ¶æ»úÄ£¿é
			{
						if(USART3_RX_BUF[0]==0x61)									
						{
							u=1;
							USART3_RX_BUF[0]=0x55;
						}
						if(USART3_RX_BUF[0]==0x62)
						{
							l=1;
							USART3_RX_BUF[0]=0x55;
						}
						if(USART3_RX_BUF[0]==0x64)
						{
							d=1;	
							USART3_RX_BUF[0]=0x55;
						}
						if(USART3_RX_BUF[0]==0x63)
						{
							r=1;
							USART3_RX_BUF[0]=0x55;
						}
			}			
					memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));			
//*****************************´®¿ÚÆÁ£¨»¹Ðèµ÷ÊÔ½«forÑ­»·È¥µô²»È»ÑÓÊ±Ì«³¤£©****************************************************************
					
		
							

//*****************************»¹Ðè×ª»»º¯Êý************************************
							

	if(m==50)
	{
			m=0;
//			read_data();
//			delay_ms(10);
//			Analysis_data();
//			delay_ms(10);
//	
			usart_flag=1;	
			printf("page1.t4.txt=\"%1f\"\xff\xff\xff",Ls);


			printf("page1.t2.txt=\"%1f\"\xff\xff\xff",Current);	
			printf("page1.t3.txt=\"%1f\"\xff\xff\xff",Voltage);	
			printf("page1.t5.txt=\"%1f\"\xff\xff\xff",RealTimePower);
			printf("page1.t6.txt=\"%1f\"\xff\xff\xff",ActiveEnergy);			
			printf("page1.t8.txt=\"%1f\"\xff\xff\xff",electric_fr);	
			printf("page1.t9.txt=\"%1f\"\xff\xff\xff",electric_pfa);	
		
			delay_ms(10);			//ÑÓÊ±1s
			HMISend(0xff);
			USART_RX_BUF[0]=0x55;	
			USART_RX_STA = 0;						
	}						
												
//*****************************¶æ»úÄ£¿é*******************************************			
		if(USART3_RX_BUF[0]==0x65)
		{
			n=1;
			ly=2;
		}
					if(n==1)
					{
				if(_fabsf(AD1-AD2)==0)
					{	
					Angle=Angle;
					}
					else if(AD1>AD2)
					{
						Angle += 1 ;
						delay_us(10);

					if((_fabsf(AD1-AD2)==0))	
					{
							Angle=Angle;
						
						delay_us(10);	
					}
					if(Angle>160)
					{
					Angle = 160 ;
					}
				}
		
					else if(AD2>AD1)
				{

					Angle -= 1;
					delay_us(10);	
					if((_fabsf(AD1-AD2)==0))	
					{	
						Angle=Angle;
						delay_us(10);	
					}
					if(Angle<20)
					{
					Angle = 20;
					}
				}

	     		if((_fabsf(AD3-AD4)==0))
					{	
						Angle=Angle;
					}
					else if(AD3>AD4)
				{
					Angle1 += 1 ;
					delay_us(10);

		
					if((_fabsf(AD3-AD4)==0))	
					{
					Angle1=Angle1;
						delay_us(10);	
					}
					if(Angle1>160)
					{
					Angle1 = 160 ;
					}
				}
		
					else if(AD4>AD3)
				{

					Angle1-= 1;
					delay_us(10);
					
					if((_fabsf(AD3-AD4)==0))	
					{
						Angle1=Angle1;
						delay_us(10);	
					}
					
					if(Angle1<20)
					{
					Angle1 = 20;
					}                                                                                                                
				}
			
			}
					if (r == 1){
										Angle1+= 25;
										r=2;
										if (Angle1> 170){
										Angle1 = 170;
										}				
						}
						if (l == 1){
										l=2;
										Angle1 -= 25;
										if (Angle1 < 10){
											Angle1 = 10;
										}
						}
						if (u == 1){
										u=2;
											Angle -= 25;
										if (Angle < 10){
											Angle = 10;
										}
						}
						if (d == 1){
										d=2;
											Angle += 25;
										if (Angle > 170){
											Angle = 170;
										}
					}
						
					

		Servo_SetAngle1(Angle1);																		//¶æ»ú1ÉèÖÃ½Ç¶È
		Servo_SetAngle2(Angle);																			//¶æ»ú2ÉèÖÃ½Ç¶È
		USART_RX_STA = 0;	
		USART2_RX_STA = 0;	
		USART3_RX_STA = 0;																					//Çå³ý
		USART6_RX_STA = 0;		
	}
}



