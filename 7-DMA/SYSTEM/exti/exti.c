#include "exti.h"

// 将 GPIO A0 A1 配置为外部中断
// 按键按下时，触发外部中断
// 外部中断触发条件为 上升沿中断
void EXTI_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  


    EXTI_InitStructure.EXTI_Line    = EXTI_Line0 | EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
  	NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
  	NVIC_Init(&NVIC_InitStructure);
}


void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}