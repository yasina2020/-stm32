#include "stm32f10x.h"
#include "myled.h"
  

// 初始化 PA12 用于控制 LED
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
 
void LED_ON()
{
    GPIO_SetBits(GPIOB,GPIO_Pin_2);		
}

void LED_OFF()
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}