#include "stm32f10x.h"                  // Device header




//IWDG
void iwdg_foo()
{
	//判断复位来源
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)
	{
		//TODO IWDG RESET	
	}else
	{
		//正常复位
	}
	
		/*IWDG初始化*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//独立看门狗写使能，解除保护
	//LSI = 40K，
	/*
	
	超时时间公式：Ti = Tlsi X PR预分频系数 X (RL + 1)
	想设置超时时间1000ms，查表的PR需要设置为16分频     IWDG_SetPrescaler(IWDG_Prescaler_16);
	Tlsi = 1/40 单位是ms
	故得出PL = 2499     IWDG_SetReload(2500 - 1)
	
	*/
	IWDG_SetPrescaler(IWDG_Prescaler_16);			//设置预分频为16
	IWDG_SetReload(2500 - 1);							//设置重装值为2499，独立看门狗的超时时间为1000ms
	IWDG_ReloadCounter();							//重装计数器，喂狗
	IWDG_Enable();									//独立看门狗使能
	
	while(1)
	{
		//喂狗
		IWDG_ReloadCounter();						//重装计数器，喂狗
	}
}

//WWDG
void wwdg_foo()
{
	//判断复位来源
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)
	{
	//TODO WWDG RESET	
	}else
	{
	//正常复位
	}
	
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	//开启WWDG的时钟
	
	/*WWDG初始化*/
	//APB1 36MHz  
	/*
	想设置超时时间50ms，看表得出WDGTB=3    WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	超时时间公式：Tw = Tplck1 X 4096 X 2^WDGTB X (T[5:0] + 1)
	Tw:50ms
	Tplck1:1/36k(这里以毫秒为单位)
	WDGTB:3
	得出T[5:0] = 54     WWDG_Enable(0x40 | 54);
	
	窗口超时公式：Tw = Tplck1 X 4096 X 2^WDGTB X (T[5:0] -W[5:0])
	想设置窗口时间30ms
	得出W[5:0] = 21     WWDG_SetWindowValue(0x40 | 21);
	*/
	WWDG_SetPrescaler(WWDG_Prescaler_8);			//设置预分频为8
	WWDG_SetWindowValue(0x40 | 21);					//设置窗口值，窗口时间为30ms
	WWDG_Enable(0x40 | 54);							//使能并第一次喂狗，超时时间为50ms
	
	while(1)
	{
		//喂狗
		WWDG_SetCounter(0x40 | 54);					//重装计数器，喂狗
	}
}

int main()
{
	void wwdg_foo();
	//void iwdg_foo();

}
