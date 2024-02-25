#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"


// LED 输出宏定义
#define LED PAout(12) // PA12

// 初始化 PA12 用于控制 LED
void LED_Init(void);

		 				    
#endif
