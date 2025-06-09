#include "pcf8591.h" 
#include "delay.h"
#include "myiic.h"

void pcf8591_Init(void)
{
	IIC_Init();
}
#define PCF8591ADDR 0x90
#define ADCR 0x91





u8 PCF8591_DAC_Output(unsigned char WriteAddr,unsigned char c,unsigned char Val)
{
	IIC_Start();
	IIC_Send_Byte(WriteAddr);
IIC_Wait_Ack();
IIC_Send_Byte(c);
	IIC_Wait_Ack();
	IIC_Send_Byte(Val);
		IIC_Wait_Ack();
	 IIC_Stop();	
	return(1);
}
u8 PCF8591_DAC_Input(unsigned char WriteAddr,unsigned char Chanel)
{
		u8 temp=0;
		IIC_Start();
		IIC_Send_Byte(WriteAddr);
		IIC_Wait_Ack();
		IIC_Send_Byte(Chanel);
		IIC_Wait_Ack();
		IIC_Stop();	
		IIC_Start();
		IIC_Send_Byte(WriteAddr|0x01);
		IIC_Wait_Ack();
		temp=IIC_Read_Byte(0);
		IIC_Stop();	
	
	
	return(temp);
}



