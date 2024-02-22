#ifndef __PWN_LED_H
#define __PWN_LED_H
#include "stm32f10x.h"

void LED_PA2_Init();

void LED_ON();

void LED_OFF();

void LEN_Togle();

void PWM_LED_Init();

void PWM_SetCompare3(uint16_t Compare);

#endif
