#ifndef __WWDG_H
#define __WWDG_H	    

#include "stm32f10x_wwdg.h"

//保存WWDG计数器的设置值,默认为最大. 
#define WWDG_CNT 0x7f

//初始化窗口看门狗 	
//tr   :T[6:0],计数器值 
//wr   :W[6:0],窗口值 
//fprer:分频系数（WDGTB）,仅最低2位有效 
//Fwwdg=PCLK1/(4096*2^fprer). 
void WWDG_Init(u8 tr,u8 wr,u32 fprer);
//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt);
//窗口看门狗中断服务程序
void WWDG_NVIC_Init();


#endif

 
