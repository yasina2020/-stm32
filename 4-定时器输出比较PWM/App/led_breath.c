
#include "stm32f10x.h"
#include "Delay.h"

void Led_breath()
{
	for (uint16_t i = 0; i <= 100; i++)
	{
		PWM_SetCompare3(i);			//依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
		Delay_ms(10);				//延时10ms
	}
	for (uint16_t i = 0; i <= 100; i++)
	{
		PWM_SetCompare3(100 - i);	//依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐减小，LED逐渐变暗
		Delay_ms(10);				//延时10ms
	}

}
