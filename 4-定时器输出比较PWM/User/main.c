#include "stm32f10x.h"                  // Device header
#include "pwm_led.h"
#include "pwm_1.h"
#include "led_breath.h"
#include "exti.h"

extern uint16_t angle_chn;

int main()
{
  
	PWM_LED_Init();//1、led呼吸
	
	PWM_DJ_Init();//2、按键改变舵机角度
  
	EXTI_KEY_LED_Init();
	
	while (1) {
		Led_breath();//1、led呼吸
		PWM_Set_Angle(angle_chn);
	}
}

