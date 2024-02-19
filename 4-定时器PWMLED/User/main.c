#include "stm32f10x.h"                  // Device header
#include "pwm_led.h"


int main()
{
  
	PWM_LED_Init();
	while (1) {
		Led_breath();
	}
}

