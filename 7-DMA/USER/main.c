#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "iic_oled.h"
#include "DMA/dma.h"
#include "AD/ad.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"

//DMA连续多轮搬运
void foo1()
{
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	
	MyDMA_init((u32)DataA, (u32)DataB, 4);
	MyDMA_Start();


	while (1)
	{
		OLED_ShowNum(0, 0, DataA[0], 1, 12);
      	OLED_ShowNum(6, 0, DataA[1], 1, 12);
      	OLED_ShowNum(12, 0, DataA[2], 1, 12);
      	OLED_ShowNum(18, 0, DataA[3], 1, 12);
		
		OLED_ShowNum(0, 1, DataA[0], 1, 12);
		OLED_ShowNum(6, 1, DataA[1], 1, 12);
		OLED_ShowNum(12, 1, DataA[2], 1, 12);
		OLED_ShowNum(18, 1, DataA[3], 1, 12);
		MyDMA_Transfer();

		DataA[0] ++;		//变换测试数据
		DataA[1] ++;
		DataA[2] ++;
		DataA[3] ++;
		
	}
}

// ADC_USE_DMA
u16 ADC_VALUE[4];
void foo2()
{
	OLED_Init();
	ADC_DMA_Init();

	while (1) {
		// ADC_DMA_GetValue();
		OLED_ShowNum(0, 0, ADC_VALUE[0], 4, 12);
		OLED_ShowNum(0, 1, ADC_VALUE[1], 4, 12);
		OLED_ShowNum(0, 2, ADC_VALUE[2], 4, 12);
		OLED_ShowNum(0, 3, ADC_VALUE[3], 4, 12);
	}

}

int main(void)
{
	foo2();

}
