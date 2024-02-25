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


extern u16 ADC_VALUE[4];
// 利用DMA完成多通道
void ADC_DMA_Init()
{
    //-------------------ADC配置------------------------------------------------
    //  1.  初始化GPIO    模拟输入模式
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    // 2.  设置ADC时钟 `RCC_ADCCLKConfig()
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    // 3.  设置ADC  `ADC_Init()` 独立模式、软件触发
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ADC软件触发，即ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;//扫描模式
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//连续转换：
    ADC_InitStruct.ADC_NbrOfChannel = 4;//扫描前四个
    ADC_Init(ADC1,&ADC_InitStruct);
    // 4.  设置规则组 `ADC_RegularChannelConfig`
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
    // 5.1  使能ADC
    ADC_Cmd(ADC1, ENABLE);
    // 5.2  ADC的DMA信号使能
    ADC_DMACmd(ADC1, ENABLE);
    // 6.  校准（4个函数）
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));

    

    // -------------------DMA配置-------------------------------------------------------

    //1、DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //2、DMA搬运源（外设）和目的（存储器）设置
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//源是ADC1的DR寄存器
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)ADC_VALUE;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;


    // 3、DMA模式配置
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//方向选择  P-》M
    DMA_InitStruct.DMA_BufferSize = 4;//计数器
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//自动化重装： 是
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//ADC的DMA信号触发DMA转换
    
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1,&DMA_InitStruct);

    // DMA使能:先暂时关闭，等getvalue时再开启
    DMA_Cmd(DMA1_Channel1, ENABLE);


    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//AD软件触发使ADC开始工作

    // 获取ADC的EOC标志位，转换完成置SET
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

}

void ADC_DMA_GetValue()
{
    // 1、设置ADC，使其开始工作
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//AD软件触发使ADC开始工作


    // 2、设置DMA，使其开始工作
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1,4);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    // while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//等待转换完成，完成后TC1置SET
    // DMA_ClearFlag(DMA1_FLAG_TC1);//手动清除标志位
}
