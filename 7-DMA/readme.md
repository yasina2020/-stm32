# DMA

直接存储器存取：可以提供外设和存储器或存储器之间的高速运转，无需CPU。DMA可以直接访问芯片的ROM、RAM、寄存器。

12个独立可配置的通道：DMA1（7个通道），DMA2（5个通道）

每个通道都支持软件触发和特定的硬件触发

![image-20240225103323114](readme.assets/image-20240225103323114.png)

STM32内部的存储器 

![image-20240225120625011](readme.assets/image-20240225120625011.png)

DMA框图：DMA1通过DMA总线----总线矩阵----APB---访问各个外设的寄存器，各个外设通过DMA请求来触发DMA操作，CPU通过系统---总线矩阵---AHB从设备来设置DMA。DMA中的诸多通道服用一个DMA总线，通过仲裁器来决定

![image-20240225121050177](readme.assets/image-20240225121050177.png)

DMA从外设起始地址找数据搬运到存储器起始地址，其中外设地址可以设置为Flash、SRAM，这样就是存储器转运到存储器。

传输计数器：转运一次，计数器-1，为0时结束本轮的转运，自增的地址会回到起始地址；如果设置了自动重装器，计数器到0后，会自动重装，然后开始下一轮的转运。

M2M设置触发方式，这里的软件触发会连续不断地触发。硬件触发可以选择ADC、串口等外设。外设发送DMA信号触发。

PS：写传输计数器时，必须线disable  DMA_Cmd()



## DMA数据转运

从内存转到内存，设置两个数组A、B，使用DMA将A数据搬运至B

设置外设、存储器的起始地址、数据宽度、自增

```c
DMA_InitStruct.DMA_PeripheralBaseAddr = ADDR_P;
DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
DMA_InitStruct.DMA_MemoryBaseAddr = ADDR_M;
DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
```

设置方向

```c
DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//方向选择  P-》M
```

设置计数器、自动重装载

```C
DMA_InitStruct.DMA_BufferSize = buffer_size;//计数器
DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//自动化重装： 否
```

设置触发方式、优先级

```C
DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//软件触发
DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
```

DMA_Cmd使能开始搬运

```C
DMA_Cmd(DMA1_Channel1, ENABLE);
```

## DMA+ADC多通道