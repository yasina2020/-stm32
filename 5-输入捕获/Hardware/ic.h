#ifndef __IC_H
#define __IC_H
#include "stm32f10x.h"

void IC_Init();
void IC_PWMI_Init();
uint32_t IC_GetFreq();
uint32_t IC_GetDutyCycle();
#endif