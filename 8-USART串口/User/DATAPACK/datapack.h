#ifndef __DATAPACK_H
#define __DATAPACK_H
#include "MYUSART/myusart.h"
#include "stm32f10x.h"
#include "stdio.h"
/*
KEY---PB1
LED---PB2

key按下发送数据包 发送四个字节
接收数据包控制led亮灭



*/





void Send_Hex_Pack(char *str);
void Rev_Hex_Pack();

#endif