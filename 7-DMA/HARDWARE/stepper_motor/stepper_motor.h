#ifndef __stepper_motor_H
#define __stepper_motor_H	 
#include "sys.h"
#include <stm32f10x_conf.h>

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <string.h>

#define IN1_H GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define IN1_L GPIO_ResetBits(GPIOB, GPIO_Pin_15);

#define IN2_H GPIO_SetBits(GPIOB, GPIO_Pin_14);
#define IN2_L GPIO_ResetBits(GPIOB, GPIO_Pin_14);

#define IN3_H   GPIO_SetBits(GPIOB, GPIO_Pin_13);
#define IN3_L	 GPIO_ResetBits(GPIOB, GPIO_Pin_13);

#define IN4_H   GPIO_SetBits(GPIOB, GPIO_Pin_12);
#define IN4_L   GPIO_ResetBits(GPIOB, GPIO_Pin_12);

void motor_configuration(void);  // 初始化步进电机控制引脚

void motor_control_F(int n);     // 正转，参数为控制转速大小
void motor_control_Z(int n);	 // 反转，参数为控制转速大小
void Motor_Ctrl_Off(void);		 // 关闭电机，防止因接入时间过长而发热
			    
#endif
