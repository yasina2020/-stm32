#ifndef __MYUSART_H
#define __MYUSART_H
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <stdint.h>



void USART1_Init(u32 baudRate);

u16 USART1_GetRev();

void USART1_SendData(uint16_t Data);
void USART1_SendArray(u16 *arr,u16 size);
void USART1_SendSrting(char *str);

#endif