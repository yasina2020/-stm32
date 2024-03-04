#ifndef __PWM_1_H
#define __PWM_1_H
#include "stm32f10x.h"

void PWM_Init(int period, int prescaler );
void servo_setangle(uint16_t angle);


#endif
