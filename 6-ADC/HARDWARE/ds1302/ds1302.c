#include "ds1302.h"




static void delay(int i) {
	while (i) {
		i--;
	}
}

void DS1302_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11);
}

void DS1302_Out() {
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void DS1302_In() {
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void DS1302_WriteByte(u8 value) {
	u8 i;
	DS1302_Out();
	for (i=0;i<8;i++){
		CLK_LOW;
		if (value&0x01) {
			DAT_HIGH;
		} else {
			DAT_LOW;
		}
		delay(1);
		CLK_HIGH;
		delay(1);		
		value >>= 1;
	}
}

void DS1302_ReadByte(u8 *value) {
	u8 i;
	DS1302_In();
	for (i=0;i<8;i++) {
		*value >>= 1;
		CLK_HIGH;
		delay(1);
		CLK_LOW;
		delay(1);
		if (DAT == 1) {
			*value |= 0x80;
		} else {
			*value &= 0x7F;
		}
	}
}

void DS1302_ReadReg(u8 addr, u8 *value) {
	CLK_LOW;
	RST_HIGH;
	DS1302_WriteByte((addr<<1)|0x01);
	DS1302_ReadByte(value);
	RST_LOW;
}

void DS1302_WriteReg(u8 addr, u8 value) {
	CLK_LOW;
	RST_HIGH;
	DS1302_WriteByte((addr<<1)&0xfe);
	DS1302_WriteByte(value);
	CLK_LOW;
	RST_LOW;
}

void DS1302_GetYear(u8 *year) {
	u8 value;
	DS1302_ReadReg(YEAR, &value);
	*year = ((value&0xf0)>>4)*10 + (value&0x0f);
}

void DS1302_GetMonth(u8 *month) {
	u8 value;
	DS1302_ReadReg(MONTH, &value);
	*month = ((value&0x10)>>4)*10 + (value&0x0f);
}

void DS1302_GetDate(u8 *date) {
	u8 value;
	DS1302_ReadReg(DATE, &value);
	*date = ((value&0x30)>>4)*10 + (value&0x0f);
}

void DS1302_GetHour(u8 *hour) {
	u8 value;
	DS1302_ReadReg(HR, &value);
	*hour = ((value&0x10)>>4)*10 + (value&0x0f);
}

void DS1302_GetMinite(u8 *minute) {
	u8 value;
	DS1302_ReadReg(MIN, &value);
	*minute = ((value&0x70)>>4)*10 + (value&0x0f);
}

void DS1302_GetSecond(u8 *second) {
	u8 value;
	DS1302_ReadReg(SEC, &value);
	*second = ((value&0x70)>>4)*10 + (value&0x0f);
}

void DS1302_SetTime(u8 yr, u8 mon, u8 date, u8 hr, u8 min, u8 sec) {
	DS1302_WriteReg(CONTROL, 0x00);
	DS1302_WriteReg(SEC, 0x80);
	DS1302_WriteReg(YEAR, ((yr/10)<<4)|(yr%10));
	DS1302_WriteReg(MONTH, ((mon/10)<<4)|(mon%10));
	DS1302_WriteReg(DATE, ((date/10)<<4)|(date%10));
	DS1302_WriteReg(HR, ((hr/10)<<4)|(hr%10));
	DS1302_WriteReg(MIN, ((min/10)<<4)|(min%10));
	DS1302_WriteReg(SEC, ((sec/10)<<4)|(sec%10));
	DS1302_WriteReg(CONTROL, 0x80);
}
