#include "stm32f10x.h"                  // Device header

//TIM2初始化（1s）
void Timer_2_Init()
{
	//开启定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//TIM_TimeBaseInit()
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//72m/10000 = 72k
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;//72k/7200 = 1s
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//只有高级定时器有这个
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//TIM_TimeBaseInit()默认出发更新终中断和更新事件，
										//这里把更新终端标志位给清除，防止对后续中断事件造成影响。
	//NVIC_Init
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
}

//使能TIM2
void TIM_2_Start()
{
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler()
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==SET)
	{
		//TODO
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
	}
}














