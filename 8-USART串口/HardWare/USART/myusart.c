#include "myusart.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

void USART1_Init(u32 baudRate)
{
    //pa9-usart1_tx
    //pa10-usart1_rx
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // GPIO设置  TX---复用推挽输出   RX------上拉输入/浮空输入
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART设置
    USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baudRate;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
    USART_Init(USART1, &USART_InitStructure);
    //开USART1接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC设置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);

    // 使能
    USART_Cmd(USART1, ENABLE);

}


uint16_t USART1_RevData;

u16 USART1_GetRev()
{
    return  USART1_RevData;
}

// // usart1中断服务函数
void USART1_IRQHandler()
{
    //判断是否接受，如果接受，就读读完后清除标志位
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
	{
        // read dr
        USART1_RevData = USART_ReceiveData(USART1);
	    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


void USART1_SendData(uint16_t Data)
{
    USART_SendData(USART1,Data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    /*
    往TDR中写数据，TDR把数据给发送位移寄存器，发送位移寄存器发送数据，当发送位移寄存器为空时，USART_FLAG_TXE=SET,此时可以往TDR写数据
    往TDR中写数据会自动清除USART_FLAG_TXE状态
    */
}

void USART1_SendArray(u16 *arr,u16 size)
{
    for (int i=0; i<size; i++) {
        USART1_SendData(arr[i]);
    }
}

void USART1_SendSrting(char *str)
{
    int i=0;
   while (str[i]!='\0') {
         
         USART1_SendData(str[i]);
         i ++;
   }
}
