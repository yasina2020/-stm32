#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

void PWM_Init(void);

void PWM_SetCompare1(uint16_t compare);

void PWM_SetPrescaler(uint16_t psc_value);

#endif
