#include "stm32f10x.h"                  // Device header
#include "pwm_led.h"
#include "pwm_1.h"
#include "led_breath.h"
#include "exti.h"
#include "Delay.h"
/*
输出PWM、改变占空比
app1:呼吸灯  舍弃
app2:舵机
app3:直流电机
*/



void app2()
{
  // 初始化PWM频率为50Hz
    // 周期 =  1)*(psc + 1) / Tclk = 20000 * 72 / 72000000=20ms
    PWM_Init(20000-1,72-1);
    uint8_t angle = 0;
    while(1)
	{
    if (angle > 180) {
      angle = 0;
    }
		delay_ms(100);
	    // 舵盘转动到 180°
    servo_setangle(angle);
    angle += 10;
	}	 

}


// extern uint8_t Speed_Dir;
void app3()
{
  momter_init();
  uint16_t speed = 0;
  while(1)
  {
      momter_set_speed(speed);
      speed = speed + 100;
      if(speed > 1800)
      {
        momter_turn_dir();
        speed = 0;
      }
      Delay_ms(50);
  }
}

int main()
{
    app3();
}

