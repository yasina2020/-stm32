





//TIM3CH1-----PA6
#include "pwm_1.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdint.h>


// ��ʼ����ʱ�� 3 CH 4 ----PB1  Ϊ PWM ���
void PWM_Init(int period, int prescaler )
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	                                                                    
    // ��ʼ�� GPIO PB1  Ϊ��������
     GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //ѡ���Ӧ������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ���˿�   

    // ��ʼ��TIM3ʱ�䵥Ԫ
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 20000-1; //�����Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //���ö�ʱ��Ԥ��Ƶ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    
    // ��ʼ�� TIM3 channel4������Ƚ�ģ��
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ�� PWM ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�����������
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // TIM_ARRPreloadConfig(TIM3, ENABLE);

    // ʹ�ܶ�ʱ��
    TIM_Cmd(TIM3, ENABLE);  //
}


void servo_setangle(uint16_t angle)
{
    // �ߵ�ƽʱ�䣺����ʱ�� = RCC �� ARR
    // (angle/180*2+0.5)/20 = RCC/20000
    // uint16_t rcc = 1000*(angle/180*2+0.5)
    float temp = (float)angle / 180 * 2000;
    uint16_t rcc = (uint16_t)temp + 500;
    TIM_SetCompare4(TIM3, rcc);
}


