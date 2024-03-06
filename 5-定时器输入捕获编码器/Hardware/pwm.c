#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
// #include "sys.h"
#include "pwm.h"
#include "stm32f10x_conf.h"
#include <stdint.h>


/*
初始化PWM
PA0——TIM2CH1
单独设置两个函数，用于更改频率和占空比
1、设置频率--预分频计数器PSC
2、设置占空比---输出比较寄存器CCR
PS：ARR保持不变，设为100，方便计算占空比和更改频率
*/ 
void PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;  

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	                                                                    
         
    // 初始化 GPIO   PA0  为复用推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化端口   

    // 初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = 100-1; //设置自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置定时器预分频 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//上升计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器
    
    // 初始化 TIM3 channel1、2、3、4 为 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择 PWM 模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //设置输出极性
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //初始化 PWM

    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);  //
    
}

void PWM_SetCompare1(uint16_t compare)
{
    TIM_SetCompare1(TIM2, compare);
}

void PWM_SetPrescaler(uint16_t psc_value)
{
    TIM_PrescalerConfig(TIM2, psc_value, TIM_PSCReloadMode_Immediate);
}




