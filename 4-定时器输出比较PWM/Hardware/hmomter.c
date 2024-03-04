#include "hmomter.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdint.h>
#include <stdlib.h>


uint8_t Speed_Dir;
// PA1 TIM2CH2---PWM1   PA2 TIM2CH3----PWM2
void momter_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	                                                                    
    // ��ʼ�� GPIO PA1 PA2  Ϊ��������
     GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; //ѡ���Ӧ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ���˿�   

    // ��ʼ��TIM2ʱ�䵥Ԫ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1000-1; // 1M / 1000 = 1000Hz   
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //72M/72 = 1M
    // PWM  Ƶ��1KHz������1/1M*1000 = 1 / Fpsc * ARR  = 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    
    // ��ʼ�� TIM2 channel4������Ƚ�ģ��
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ�� PWM ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�����������
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // ʹ�ܶ�ʱ��
    TIM_Cmd(TIM2, ENABLE);  //
    Speed_Dir = 1;
}

uint16_t my_abs(uint16_t speed)
{
    if (speed < 0) {
        speed = -speed;
    }
    return speed;
}

// speed  0~1000
void momter_set_speed(uint16_t speed)
{
    if (Speed_Dir == 1) {
        TIM_SetCompare2(TIM2, 0);
        TIM_SetCompare3(TIM2, my_abs(speed));
    }else if (Speed_Dir == 0) {
        TIM_SetCompare2(TIM2, my_abs(speed));
        TIM_SetCompare3(TIM2, 0);
    }
}

void momter_turn_dir()
{
    if (Speed_Dir == 1)
        Speed_Dir = 0;
    else
        Speed_Dir = 1;
}