#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 

// 将 GPIO A1 配置为 GPIO 下拉输入功能
// 当 按键按下时， PA1 输入高电平
// 当 按键弹起时， PA1 输入低电平
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 开启时钟
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    // 设置 PA1 引脚为下拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //选择对应的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
}

//扫描按键
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
