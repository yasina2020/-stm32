#include "chinese.h"
#include "lcd.h"	   	  
			    
//在LCD上的（x，y）处画点
//color：点的颜色
void LCD_Draw_Point(u16 x,u16 y,u16 color)
{
	u16 temp;
	temp=POINT_COLOR;
	POINT_COLOR=color;
    LCD_DrawPoint(x,y);
	POINT_COLOR=temp;
}

void LCD_ShowChinese16(u16 x, u16 y, const u8 text[][16], u16 count, u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;			
	int index;
	for(index=0;index<count;index++)
	{
		for(t=0;t<32;t++)//每个16*16的汉字点阵 有32个字节
		{   
			if(t<16)
				temp=text[index*2][t];      //前16个字节
			else 
				temp=text[index*2+1][t-16];    //后16个字节	                          
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					LCD_Draw_Point(x,y,color);//画实心点
				else 
					LCD_Draw_Point(x,y,BACK_COLOR);   //画空白点（使用背景色）
			
				temp<<=1;
				y++;
				if((y-y0)==16)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
		}      
	}	
}

void LCD_ShowChinese24(u16 x, u16 y, const u8 text[][24], u16 count, u16 color, u16 back_color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;			
	int index;
	for(index=0;index<count;index++)
	{
    for(t=0;t<72;t++)//每个24*24的汉字点阵 有72个字节
    {   
		if(t<24)temp=text[index*3][t];           //前24个字节
		else if(t<48)temp=text[index*3+1][t-24]; //中24个字节	                          
        else temp=text[index*3+2][t-48];         //后24个字节
	    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Draw_Point(x,y,color);//画实心点
			else LCD_Draw_Point(x,y,back_color);   //画空白点（使用背景色）
			temp<<=1;
			y++;
			if((y-y0)==24)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }                     
	}	
}
