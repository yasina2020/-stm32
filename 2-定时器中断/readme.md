## 定时器中断

定一个时，时间到了产生一次中断。

1.  开启定时器时钟
2.  选择内部时钟源
3.  配置时钟计数器
4.  使能时钟
5.  配置NVIC
6.  定时器中断函数编写

``` c
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//10k/10000 = 1s
TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;//72m/7200 = 10k
TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//只有高级定时器有这个
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
TIM_ClearFlag(TIM2,TIM_FLAG_Update);
```
TIM_Prescaler这是定时器的预分频值。它用来减慢定时器的时钟频率，从而使定时器的计数速度变慢。这里设置成7200，本来stm32f103c8是72mHZ，就是72m次跳变会在一秒内完成，通过分频使其在10k完成。

TIM_Period这是定时器的周期值。它决定了定时器溢出（从计数器值归零）的时间间隔。预分频后10k次计数是1s，这里令其技术到（10000-1），正好是1s。

这样就完成了1s的定时
