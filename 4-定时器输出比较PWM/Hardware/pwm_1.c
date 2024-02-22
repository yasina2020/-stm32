





//TIM3CH1-----PA6
#include "pwm_1.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdint.h>

// uint16_t angle_chn;//����Ƕ�ѡ��

// �ú������ܣ���ʼ��TIM3OC1����PA6���ŷ������������������������
void PWM_DJ_Init()
{
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);

    //����PA6
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);
    // ����TIM3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=20000-1;// timeout=��period+1��*��Prescaler+1��/timeclockfren  
    TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;// timeout��λus   72mhz=72timeclockfren  �������ú��Ǽ���20000����20ms
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

    // ����PWM���ͨ��   ���벶��
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ���ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0; // .��ʼ��ccrֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // �������
    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // ��ʼ��TIM3��ͨ��1

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // ʹ��TIM3��ͨ��1Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ��TIM3���Զ���װ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE); // ����TIM3
    PWM_Set_Angle(3);
}


//���ݸ�����angle_chnѡ����ʵ�ռ�ձ�
void PWM_Set_Angle(uint16_t angle_chn)
{
    // ���ݴ����ͨ����������ռ�ձ�
    switch (angle_chn)
    {
        case 0:
            TIM_SetCompare1(TIM3,500); // PWM�����ȣ�ռ�ձ�0.5ms
            break;
        case 1:
            TIM_SetCompare1(TIM3,1000); // PWM�����ȣ�ռ�ձ�1ms
            break;
        case 2:
            TIM_SetCompare1(TIM3,1500); // PWM�����ȣ�ռ�ձ�1.5ms
            break;
        case 3:
            TIM_SetCompare1(TIM3,2000); // PWM�����ȣ�ռ�ձ�2ms
            break;
        case 4:
            TIM_SetCompare1(TIM3,2500); // PWM�����ȣ�ռ�ձ�2.5ms
            break;
        default:
            TIM_SetCompare1(TIM3,500); // Ĭ��ռ�ձ�Ϊ0.5ms
            break;
    }
}
