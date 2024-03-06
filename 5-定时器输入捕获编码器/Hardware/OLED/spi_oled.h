/*****************************************************************************
 * 			7接口硬件 SPI OLED 例程
 *			接线说明:
 *              ----------------------------------------------------------------
 *              GND    电源地
 *              VCC  接5V或3.3v电源
 *              D0   接PA5（SCL）
 *              D1   接PA7（SDA）
 *              RES  接PB2
 *              CS   接PB1
 *              DC   接PB0
 *              ----------------------------------------------------------------
 *			汉字取模格式为：
 *  				阴码、列行式、逆向
 * 库版本  ：V3.5.0
******************************************************************************/
#ifndef __SPI_OLED_H
#define __SPI_OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------测试LED端口定义---------------- 
#define LED_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DC
#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)

//-----------------OLED端口定义----------------  					   


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_2)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void spi_config(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_WriteByte(u8 TxData);
void SPI_OLED_WR_Byte(u8 dat,u8 cmd);
void SPI_OLED_Set_Pos(unsigned char x, unsigned char y);
void SPI_OLED_Display_On(void);
void SPI_OLED_Display_Off(void);
void SPI_OLED_Clear(void);

void SPI_OLED_ShowChar(u8 x,u8 y,u8 chr);
void SPI_OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void SPI_OLED_ShowString(u8 x,u8 y, char *p);	 
void SPI_OLED_Set_Pos(unsigned char x, unsigned char y);
void SPI_OLED_ShowCHinese(u8 x,u8 y,u8 no);
void SPI_OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void spi_oled_init(void);
#endif  
	 



