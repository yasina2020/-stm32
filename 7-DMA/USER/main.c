#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "iic_oled.h"
#include "DMA/dma.h"
#include "stm32f10x_dma.h"

uint8_t DataA[] = {1, 2, 3, 4};				//定义测试数组DataA，为数据源
uint8_t DataB[] = {0, 0, 0, 0};							//定义测试数组DataB，为数据目的地

int main(void)
{
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);	//DMA初始化，把源数组和目的数组的地址传入
	
	
	DMA_Cmd(DMA1_Channel1, ENABLE);		

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
		// DataA[0] ++;		//变换测试数据
		// DataA[1] ++;
		// DataA[2] ++;
		// DataA[3] ++;
		
		// OLED_ShowHexNum(2, 1, DataA[0], 2);		//显示数组DataA
		// OLED_ShowHexNum(2, 4, DataA[1], 2);
		// OLED_ShowHexNum(2, 7, DataA[2], 2);
		// OLED_ShowHexNum(2, 10, DataA[3], 2);
		// OLED_ShowHexNum(4, 1, DataB[0], 2);		//显示数组DataB
		// OLED_ShowHexNum(4, 4, DataB[1], 2);
		// OLED_ShowHexNum(4, 7, DataB[2], 2);
		// OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		// delay_ms(10);		//延时1s，观察转运前的现象
		
		// // MyDMA_Transfer();	//使用DMA转运数组，从DataA转运到DataB
		
		// OLED_ShowHexNum(2, 1, DataA[0], 2);		//显示数组DataA
		// OLED_ShowHexNum(2, 4, DataA[1], 2);
		// OLED_ShowHexNum(2, 7, DataA[2], 2);
		// OLED_ShowHexNum(2, 10, DataA[3], 2);
		// OLED_ShowHexNum(4, 1, DataB[0], 2);		//显示数组DataB
		// OLED_ShowHexNum(4, 4, DataB[1], 2);
		// OLED_ShowHexNum(4, 7, DataB[2], 2);
		// OLED_ShowHexNum(4, 10, DataB[3], 2);

		// delay_ms(10);		//延时1s，观察转运后的现象
	}
}
