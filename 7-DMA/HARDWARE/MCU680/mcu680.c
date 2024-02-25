
#include "mcu680.h"
#include "delay.h"
/*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-25Z---STM32
1、GY-MCU680_RX---STM32_TX2(PIN_A2),STM32复位将发送 输出数据配置和自动输出指令 给模块
2、GY-MCU68_TX---STM32_RX2(PIN_A3)
3、STM32_TX1(PIN_A9)---FT232_RX,STM32将串口打印出来，可通过串口助手查看

软件说明:
该程序采用串口方式获取模块数据，波特率9600

注：模块波特率需和该程序波特率一样为9600，中断函数位于stm32f10x_it.c
*/
uint8_t str_temperature[10] = {0};
uint8_t str_humidity[10] = {0};
uint8_t str_airQuality[10] = {0};
uint8_t str_airPressure[10] = {0};
uint8_t str_altitude[10] = {0};
uint8_t RX_BUF[50]={0},stata=0;

 void mcu680_irq_Configuration(void)
{
	NVIC_InitTypeDef NVIC_X;

	/*抢占优先级可打断中断级别低的中断*/
	/*响应优先级按等级执行*/
	NVIC_X.NVIC_IRQChannel = USART2_IRQn;		  //中断向量
	NVIC_X.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_X.NVIC_IRQChannelSubPriority = 0;		  //响应优先级
	NVIC_X.NVIC_IRQChannelCmd = ENABLE;			  //使能中断响应
	NVIC_Init(&NVIC_X);
}
void send_Instruction(void)
{
	uint8_t send_data[4] = {0};
	send_data[0] = 0xa5;
	send_data[1] = 0x55;
	send_data[2] = 0x3F;
	send_data[3] = 0x39;
	USART_Send_bytes(send_data, 4); //发送

	delay_ms(1);

	send_data[0] = 0xa5;
	send_data[1] = 0x56;
	send_data[2] = 0x02;
	send_data[3] = 0xfd;
	USART_Send_bytes(send_data, 4); //发送自动输出指令
	delay_ms(1);
}
void USART2_IRQHandler(void)
{
	static uint8_t rebuf[20]={0},i=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rebuf[i++]=USART_ReceiveData(USART2);
		if(rebuf[0]!=0x5a)//判断帧头
			i=0;
	  if((i==2)&&(rebuf[1]!=0x5a))//判断帧头
			i=0;
		if(i>4)//当i计数值=5时，功能字节接受完毕，数据长度字节接收完毕
		{
			if(i==rebuf[3]+5)
			{
	       memcpy(RX_BUF,rebuf,i);
				stata=1;
				i=0;
			}
		}
		USART_ClearFlag(USART2,USART_FLAG_RXNE);//清中断标志
	}	
}
//校验和检查
uint8_t CHeck(uint8_t *data)
{
	uint8_t sum=0,number=0,i=0;
	number=RX_BUF[3]+5;
	if(number>20)//超过上传数据
		return 0;
	for(i=0;i<number-1;i++)
	 sum+=RX_BUF[i];
	if(sum==RX_BUF[number-1])
	{
		memcpy(data,RX_BUF,number);
		return 1;
	}
	else
    return 0;
}

//发送一个字节数据
//input:byte,待发送的数据
void USART_send_byte(uint8_t byte)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//等待发送完成
	USART2->DR=byte;	
}
//发送多字节数据
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		USART_send_byte(Buffer[i++]);
	}
}
//发送多字节数据+校验和
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//累加Length-1前的数据
		USART_send_byte(Buffer[i++]);
	}
}
//发送一帧数据
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[30],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=2*length;//数据长度
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,i);	
}
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[50],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=length;//数据长度
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,i);	
}



