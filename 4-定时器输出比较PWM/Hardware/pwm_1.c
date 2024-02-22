





//TIM3CH1-----PA6
#include "pwm_1.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include <stdint.h>

// uint16_t angle_chn;//舵机角度选择

// 该函数功能，初始化TIM3OC1，即PA6，伺服电机（舵机）接在这个引脚上
void PWM_DJ_Init()
{
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);

    //配置PA6
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PA6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3);
    // 配置TIM3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=20000-1;// timeout=（period+1）*（Prescaler+1）/timeclockfren  
    TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;// timeout单位us   72mhz=72timeclockfren  这里设置后是计数20000次是20ms
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

    // 配置PWM输出通道   输入捕获
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; // .初始的ccr值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性
    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // 初始化TIM3的通道1

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能TIM3的通道1预装载寄存器

    TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能TIM3的自动重装载寄存器

    TIM_Cmd(TIM3, ENABLE); // 启动TIM3
    PWM_Set_Angle(3);
}


//根据给定的angle_chn选择合适的占空比
void PWM_Set_Angle(uint16_t angle_chn)
{
    // 根据传入的通道参数设置占空比
    switch (angle_chn)
    {
        case 0:
            TIM_SetCompare1(TIM3,500); // PWM脉冲宽度，占空比0.5ms
            break;
        case 1:
            TIM_SetCompare1(TIM3,1000); // PWM脉冲宽度，占空比1ms
            break;
        case 2:
            TIM_SetCompare1(TIM3,1500); // PWM脉冲宽度，占空比1.5ms
            break;
        case 3:
            TIM_SetCompare1(TIM3,2000); // PWM脉冲宽度，占空比2ms
            break;
        case 4:
            TIM_SetCompare1(TIM3,2500); // PWM脉冲宽度，占空比2.5ms
            break;
        default:
            TIM_SetCompare1(TIM3,500); // 默认占空比为0.5ms
            break;
    }
}
