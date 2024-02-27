#include "stm32f10x.h"
#include "mykey.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 开启时钟
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
    // 设置 PB1 引脚为下拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStruct);


	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		//TODO
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

}
