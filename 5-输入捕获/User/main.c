#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "ic.h"
#include "pwm.h"
#include "iic_oled.h"

/*
存在问题，测的不准
*/
int main()
{
	OLED_Init();
	PWM_Init();
	IC_PWMI_Init();
	
	while(1)
	{
		OLED_ShowNum(0,0,IC_GetFreq(),4,12);
		OLED_ShowNum(0,1,IC_GetDutyCycle(),4,12);
	}
}
