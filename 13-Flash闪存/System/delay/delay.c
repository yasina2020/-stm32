#include "delay.h"

// 使用 SysTick 实现延时，比较准确
void delay_us(uint32_t nus)
{
    // 设置 SysTick 的计数周期
    SysTick_Config(SystemCoreClock/1000000); // 每周期 1us
    // 关闭 SysTick 的中断，不可省略
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    // 开始计数
    for(uint32_t i=0; i<nus; i++)
    {   // 循环一次就是 1us
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // 关闭 SysTick 定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// 使用 SysTick 实现延时，比较准确
void delay_ms(uint32_t nms)
{
    // 设置 SysTick 的计数周期
    SysTick_Config(SystemCoreClock/1000); // 每周期 1ms
    // 关闭 SysTick 的中断，不可省略
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    // 开始计数
    for(uint32_t i=0; i<nms; i++)
    {   // 循环一次就是 1ms
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // 关闭 SysTick 定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// 使用死循环实现延时
// 注意函数前面防止编译优化的标识不能省略
__attribute__((optimize("-O0")))							   
void delay(uint32_t nus)
{
	while (nus--);
}

// 使用死循环实现延时
// 注意函数前面防止编译优化的标识不能省略
// __attribute__((optimize("-O0")))							   
// void delay_us(uint32_t nus)
// {
// 	while (nus--)
// 	{
// 		for(uint32_t i = 0; i<10; i++);
// 	}
// }

// 使用死循环实现延时
// 注意函数前面防止编译优化的标识不能省略
// void delay_ms(uint32_t nms)
// {	 		  	  
//     while (nms--)
// 	{
// 		delay_us(1000);
// 	}
// } 


