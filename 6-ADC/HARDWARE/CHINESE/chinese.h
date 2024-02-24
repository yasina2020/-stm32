#ifndef __CHINESE_H
#define __CHINESE_H	 
#include "sys.h"   

void LCD_ShowChinese16(u16 x, u16 y, const u8 text[][16], u16 count, u16 color);
void LCD_ShowChinese24(u16 x, u16 y, const u8 text[][24], u16 count, u16 color, u16 back_color);
#endif
