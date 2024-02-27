#ifndef _DS1302_H
#define _DS1302_H

#include "stm32f10x.h"

#define SEC             0x40
#define MIN             0x41
#define HR              0x42
#define DATE            0x43
#define MONTH           0x44
#define DAY             0x45
#define YEAR            0x46
#define CONTROL         0x47
#define TRACKLE_CHARGER 0x48
#define CLOCK_BURST     0x5F
#define RAM0            0x60
#define RAM1            0x61
#define RAM30           0x7E
#define RAM_BURST       0x7F

#define RST_HIGH      GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define RST_LOW       GPIO_ResetBits(GPIOB, GPIO_Pin_6)     

#define CLK_HIGH      GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define CLK_LOW       GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define DAT_HIGH      GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define DAT_LOW       GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define DAT           GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)


void DS1302_Init(void);
void DS1302_ReadReg(u8 addr, u8 *value);
void DS1302_GetYear(u8 *year);
void DS1302_GetMonth(u8 *month);
void DS1302_GetDate(u8 *date);
void DS1302_GetHour(u8 *hour);
void DS1302_GetMinite(u8 *minute);
void DS1302_GetSecond(u8 *second);
void DS1302_SetTime(u8 yr, u8 mon, u8 date, u8 hr, u8 min, u8 sec);

#endif
