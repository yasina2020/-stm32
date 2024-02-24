#include "stm32f10x.h"
#include "delay.h"
#include "stepper_motor.h"
#include <stm32f10x_conf.h>

// 初始化步进电机控制引脚
// PB12,PB13,PB14,PB15	
void motor_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

// 步进电机正转
// 四相八拍控制
// 输入参数控制转速大小，参数越大，转动越慢
void motor_control_F(int n)
{
	
	IN1_H;
	delay_us(n);
	IN4_L;
	delay_us(n);
	IN2_H;
	delay_us(n);
	IN1_L;
	delay_us(n);
	IN3_H;
	delay_us(n);
	IN2_L;
	delay_us(n);
	IN4_H;
	delay_us(n);
	IN3_L;
	delay_us(n);
}

// 步进电机反转
// 四相四拍控制
// 输入参数控制转速大小，参数越大，转动越慢
void motor_control_Z(int n)
{
	IN2_L;
	IN3_L;
	IN4_L;	
	IN1_H;	
	delay_us(n);
	IN1_L;
	IN2_L;
	IN3_L;
	IN4_H;
	delay_us(n);
	IN4_L;
	IN1_L;
	IN2_L;
	IN3_H;
	delay_us(n);
	IN3_L;
	IN4_L;
	IN1_L;
	IN2_H;
	delay_us(n);
}

// 关闭电机，防止因接入时间过长而发热
void Motor_Ctrl_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

