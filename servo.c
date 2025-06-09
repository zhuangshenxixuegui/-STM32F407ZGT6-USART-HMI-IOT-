#include "stm32f4xx.h "                  // Device header
#include "PWM.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
void Servo_Init(void)
{
void	TIM4_PWM_Init(u32 arr,u32 psc);
}

void Servo_SetAngle2(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle1(float Angle1)
{
	PWM_SetCompare1(Angle1 / 180 * 2000 + 500);
}

