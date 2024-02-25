#include "sys.h"
#include "pwm.h"
#include "stm32f10x_conf.h"


// 初始化定时器 3 为 PWM 输出
void PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;  

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE); 
	                                                                    
         
    // 初始化 GPIO   PA6 PA7 PB0 PB1  为复用推挽
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化端口   

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   //选择对应的引脚   
    GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化端口 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //选择对应的引脚   
    GPIO_Init(GPIOB, &GPIO_InitStructure);      //初始化端口

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;   //选择对应的引脚   
    GPIO_Init(GPIOB, &GPIO_InitStructure);      //初始化端口   

    // 初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = 256; //设置自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler =72; //设置定时器预分频 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//上升计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化定时器
	
    // 允许TIM3中断
    // TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
    
    // 初始化 TIM3 channel1、2、3、4 为 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //设置输出极性
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //初始化 PWM
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //初始化 PWM
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //初始化 PWM
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //初始化 PWM

    // 使能 TIM3 在 CCR2 上的预装载寄存器
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    // 使能定时器
    TIM_Cmd(TIM3, ENABLE);  //
    
}

