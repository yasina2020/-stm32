
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>

//�ⲿ�жϳ�ʼ��
//KEY�����ⲿ�жϣ�����PA1  LED����PD0�����¸ı�LED״̬
void EXTI_KEY_LED_Init()
{
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

//���ⲿ�ж���дһ��������ʹ��ÿ��һ��key��cnt+1����Χ��0-4��
//�����Ӧ���Ƕ���ĽǶ�
/*
�Ƕ�	angle_chn	PWMռ�ձȣ�����20ms��
0		0			0.5
45		1			1
90		2			1.5
135		3			2
180		4			2.5

*/ 
uint16_t cnt = 0;
uint16_t DJ_Angle()
{
	cnt++;
	if (cnt >= 5) {
		cnt -= 5;
	};

	return  cnt;
}
#include "pwm_1.h"

uint16_t angle_chn;

//���жϷ������иı�״̬
void EXTI1_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line1)==SET) {
        GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_0)==SET?GPIO_ResetBits(GPIOD,GPIO_Pin_0):GPIO_SetBits(GPIOD,GPIO_Pin_0);
		angle_chn = DJ_Angle();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}





