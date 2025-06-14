#include "relay.h" 
#include "usart.h" 

void Relay_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_12); 
	GPIO_ResetBits(GPIOE,GPIO_Pin_13); 
}

void Relay1_ON(void)
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_12); 

}
void Relay1_OFF(void)
{
  GPIO_SetBits(GPIOE,GPIO_Pin_12); 

}
void Relay2_ON(void)
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_13); 

}
void Relay2_OFF(void)
{
  GPIO_SetBits(GPIOE,GPIO_Pin_13); 

}

