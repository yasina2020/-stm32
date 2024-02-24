#ifndef _MCU680_H_
#define _MCU680_H_

#include "stm32f10x.h"
#include "string.h"
#include "usart.h"
#include "stdio.h"


void mcu680_irq_Configuration(void);
void send_Instruction(void);
void USART_send_byte(uint8_t byte);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);
void send_out(int16_t *data,uint8_t length,uint8_t send);
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send);
uint8_t CHeck(uint8_t *data_buf);

#endif
