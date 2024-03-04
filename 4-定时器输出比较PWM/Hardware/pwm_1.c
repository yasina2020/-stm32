





//TIM3CH1-----PA6
#include "pwm_1.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdint.h>


// 初始化定时器 3 CH 4 ----PB1  为 PWM 输出
void PWM_Init(int period, int prescaler )
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	                                                                    
    // 初始化 GPIO PB1  为复用推挽
     GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化端口   

    // 初始化TIM3时间单元
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 20000-1; //设置自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置定时器预分频 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    
    // 初始化 TIM3 channel4的输出比较模块
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //设置输出极性
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // TIM_ARRPreloadConfig(TIM3, ENABLE);

    // 使能定时器
    TIM_Cmd(TIM3, ENABLE);  //
}


void servo_setangle(uint16_t angle)
{
    // 高电平时间：周期时间 = RCC ： ARR
    // (angle/180*2+0.5)/20 = RCC/20000
    // uint16_t rcc = 1000*(angle/180*2+0.5)
    float temp = (float)angle / 180 * 2000;
    uint16_t rcc = (uint16_t)temp + 500;
    TIM_SetCompare4(TIM3, rcc);
}


