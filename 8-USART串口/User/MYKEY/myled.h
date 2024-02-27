#ifndef __MYLED_H
#define __MYLED_H

#include "stm32f10x.h"

// 初始化 PB2 用于控制 LED
void LED_Init(void);
void LED_ON();
void LED_OFF();
		 				    
#endif