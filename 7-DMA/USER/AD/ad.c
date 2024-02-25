#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>
void AD_Init()
{
    // PA0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 设置ADC
    ADC_DeInit(ADC1);  //复位ADC1 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC内部时钟，PCLK2意为APB2总线时钟
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC独立模式
    ADC_InitStruct.ADC_ScanConvMode =DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStruct);
    
    // 配置规则组 ADC1 通道0的优先级是1，采样时间1.5   如果是多个通道，就多次配置该函数
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_1Cycles5);
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    // ADC软件触发转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

//获取ADC转化后的原始数值
uint16_t AD_GetValue()
{
    
    // 获取ADC的EOC标志位，转换完成置SET
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    // 获取值，读ADC的DR寄存区，读完后，会自动降EOC置RESET
    return  ADC_GetConversionValue(ADC1);
}

//获取电压
float AD_GetVolite()
{
    uint16_t v = AD_GetValue();
    return 0 + (float)v / 4095 * (3.3 - 0);
}

//利用非连续非扫描实现手动多通道
void AD_Init_CHS()
{
    // PA0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 设置ADC
    ADC_DeInit(ADC1);  //复位ADC1 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC内部时钟，PCLK2意为APB2总线时钟
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC独立模式
    ADC_InitStruct.ADC_ScanConvMode =DISABLE;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStruct);
    
    // 
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    

}

//轮流多次配置规则组已达到多通道效果
uint16_t AD_GetValue_CHS(uint8_t ADC_Channel)
{
    // 配置规则组 ADC1 通道0的优先级是1，采样时间1.5   如果是多个通道，就多次配置该函数
    ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_1Cycles5);
    // ADC软件触发转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 获取ADC的EOC标志位，转换完成置SET
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    // 获取值，读ADC的DR寄存区，读完后，会自动降EOC置RESET
    return  ADC_GetConversionValue(ADC1);
}


