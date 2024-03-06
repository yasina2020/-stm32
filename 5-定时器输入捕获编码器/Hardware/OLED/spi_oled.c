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

#include "spi_oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"

void spi_config(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7); // PA5/6/7上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	// Enable SPIx
	SPI_Cmd(SPI1, ENABLE);
}

// SPI 速度设置函数
// SpeedSet:
// SPI_BaudRatePrescaler_2   2分频
// SPI_BaudRatePrescaler_8   8分频
// SPI_BaudRatePrescaler_16  16分频
// SPI_BaudRatePrescaler_256 256分频
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler; //设置SPI1速度
	SPI_Cmd(SPI1, ENABLE);
}

// SPIx 写一个字节
// TxData:要写入的字节
//返回值:无
u8 SPI1_WriteByte(u8 TxData)
{
	u8 retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if (retry > 200)
			return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if (retry > 200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
}

//向SSD1106写入一个字节。
// dat:要写入的数据/命令
// cmd:数据/命令标志 0,表示命令;1,表示数据;
void SPI_OLED_WR_Byte(u8 dat, u8 cmd)
{
	//	u8 i;
	if (cmd)
		OLED_DC_Set();
	else
		OLED_DC_Clr();
	OLED_CS_Clr();
	//	for(i=0;i<8;i++)
	//	{
	//		OLED_SCLK_Clr();
	//		if(dat&0x80)
	//			 OLED_SDIN_Set();
	//		else
	//			 OLED_SDIN_Clr();
	//		OLED_SCLK_Set();
	//		dat<<=1;
	//	}
	SPI1_WriteByte(dat);

	OLED_CS_Set();
	OLED_DC_Set();
}
void SPI_OLED_Set_Pos(unsigned char x, unsigned char y)
{
	SPI_OLED_WR_Byte(0xb0 + y, OLED_CMD);
	SPI_OLED_WR_Byte((((x)&0xf0) >> 4) | 0x10, OLED_CMD);
	SPI_OLED_WR_Byte(((x)&0x0f), OLED_CMD);
}
//开启OLED显示
void SPI_OLED_Display_On(void)
{
	SPI_OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	SPI_OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	SPI_OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
//关闭OLED显示
void SPI_OLED_Display_Off(void)
{
	SPI_OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	SPI_OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	SPI_OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void SPI_OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		SPI_OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
		SPI_OLED_WR_Byte(0x00, OLED_CMD);	  //设置显示位置—列低地址
		SPI_OLED_WR_Byte(0x10, OLED_CMD);	  //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			SPI_OLED_WR_Byte(0, OLED_DATA);
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// mode:0,反白显示;1,正常显示
// size:选择字体 16/12
void SPI_OLED_ShowChar(u8 x, u8 y, u8 chr)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (SIZE == 16)
	{
		SPI_OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			SPI_OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		SPI_OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			SPI_OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		SPI_OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 6; i++)
			SPI_OLED_WR_Byte(F6x8[c][i], OLED_DATA);
	}
}
// m^n函数
static u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}
//显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式	0,填充模式;1,叠加模式
// num:数值(0~4294967295);
void SPI_OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				SPI_OLED_ShowChar(x + (size / 2) * t, y, ' ');
				continue;
			}
			else
				enshow = 1;
		}
		SPI_OLED_ShowChar(x + (size / 2) * t, y, temp + '0');
	}
}
//显示一个字符号串
void SPI_OLED_ShowString(u8 x, u8 y, char *chr)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		SPI_OLED_ShowChar(x, y, chr[j]);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}
//显示汉字
void SPI_OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
	u8 t, adder = 0;
	SPI_OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		SPI_OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	SPI_OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		SPI_OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void SPI_OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		SPI_OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			SPI_OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

void spi_oled_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// PB0作为数据/指令信号引脚,PB1作为片选信号,PB2作为复位信号引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2); // PB0,PB1,PB2输出高

	spi_config();

	OLED_RST_Set();
	delay_ms(10);
	OLED_RST_Clr();
	delay_ms(20);
	OLED_RST_Set();

	SPI_OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
	SPI_OLED_WR_Byte(0x02, OLED_CMD); //---set low column address
	SPI_OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	SPI_OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	SPI_OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
	SPI_OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	SPI_OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	SPI_OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	SPI_OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
	SPI_OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	SPI_OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
	SPI_OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	SPI_OLED_WR_Byte(0x00, OLED_CMD); //-not offset
	SPI_OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	SPI_OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	SPI_OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
	SPI_OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SPI_OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	SPI_OLED_WR_Byte(0x12, OLED_CMD);
	SPI_OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
	SPI_OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
	SPI_OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	SPI_OLED_WR_Byte(0x02, OLED_CMD); //
	SPI_OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	SPI_OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
	SPI_OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	SPI_OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	SPI_OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

	SPI_OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
	SPI_OLED_Clear();
	SPI_OLED_Set_Pos(0, 0);
}
