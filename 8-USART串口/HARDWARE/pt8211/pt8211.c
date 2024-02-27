#include "pt8211.h"
#include "delay.h"

#define PT8211_WS 	PBout(12)
#define PT8211_BCK  PBout(13)
#define PT8211_DIN	PBout(14)


//初始化PT8211
void PT8211_Init(void)	   
{                 
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能端口时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;				 //PB12 PB13 PB14 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);						
}

//初始化PT8211
//right_ch lift_ch：输入的16位通道值	有符号数
//通道值为正数时，该通道输出电压大于 VCC/2
//通道值为负数时，该通道输出电压小于 VCC/2
//当通道值不足16位时，不足的位默认为0
//当通道值超过16位时，数据取前16位
void PT8211_Output(int16_t right_ch, int16_t lift_ch)
{
		int i = 0;
		//将负数转换为补码
		if(right_ch<0)
				right_ch = 0x10000 + right_ch;
		if(lift_ch<0)
				lift_ch = 0x10000 + lift_ch;
		
		
		PT8211_WS = 0;	//设置为右声道
		for(i = 15; i >=0; i--)
		{
				PT8211_BCK = 0;
				PT8211_DIN = (right_ch >> i)&0x1;
				PT8211_BCK = 1;
				delay_us(1);
		}
		
		PT8211_WS = 1;	//设置为左声道
		for(i = 15; i >=0; i--)
		{
				PT8211_BCK = 0;
				PT8211_DIN = (lift_ch >> i)&0x1;
				PT8211_BCK = 1;
				delay_us(1);
		}
}
