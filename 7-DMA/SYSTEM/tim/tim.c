#include "sys.h"
#include "pwm.h"
#include "stm32f10x_conf.h"

// 初始化 TIM1
void TIM1_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 10; 
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);               // 设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 // 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 8;             // 重复计数器，高级定时器更新计数 8 次才中断一次
	                                                             // 可用于超长时间定时中断
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(      // 使能或者失能指定的TIM中断
		TIM1,          // TIM1
		TIM_IT_Update, // TIM 中断源
		ENABLE         // 使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;         // TIM1 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         // 响应优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
    
}

// 初始化 TIM2
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 10; 
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(      // 使能或者失能指定的TIM中断
		TIM2,          // TIM2
		TIM_IT_Update, // TIM 中断源
		ENABLE         // 使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         // 响应优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
    
}
