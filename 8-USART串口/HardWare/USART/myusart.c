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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // GPIO����  TX---�����������   RX------��������/��������
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART����
    USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baudRate;										//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);
    //��USART1�����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC����
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);

    // ʹ��
    USART_Cmd(USART1, ENABLE);

}


uint16_t USART1_RevData;

u16 USART1_GetRev()
{
    return  USART1_RevData;
}

// // usart1�жϷ�����
void USART1_IRQHandler()
{
    //�ж��Ƿ���ܣ�������ܣ��Ͷ�����������־λ
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
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
    ��TDR��д���ݣ�TDR�����ݸ�����λ�ƼĴ���������λ�ƼĴ����������ݣ�������λ�ƼĴ���Ϊ��ʱ��USART_FLAG_TXE=SET,��ʱ������TDRд����
    ��TDR��д���ݻ��Զ����USART_FLAG_TXE״̬
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
