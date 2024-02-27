#include "HX711.h"
#include "delay.h"

//****************************************************
// 初始化 HX711 引脚
//****************************************************
void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

	 //HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);  			
}



//****************************************************
// 读取 HX711 数据
//****************************************************
u32 HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  //	HX711_DOUT=1; 
	delay_us(1);
	HX711_SCK=0; 
	count=0; 
	while(HX711_DOUT); 
	for(i=0;i<24;i++)
	{ 
		HX711_SCK=1; 
		count=count<<1; 
		delay_us(1);
		HX711_SCK=0; 
		if(HX711_DOUT)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK=1; 
	delay_us(1);
	HX711_SCK=0;  // 通道A 增益128
//	delay_us(1);
//	 
//	HX711_SCK=1; 
//	delay_us(1);
//	HX711_SCK=0;  // 通道B 增益32
//	delay_us(1);
//	 
//	HX711_SCK=1; 
//	delay_us(1);
//	HX711_SCK=0;  // 通道A 增益64
	
	count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	return(count);
}

