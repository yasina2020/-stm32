#include "ds18b20.h"
#include "delay.h"

//复位DS18B20
void DS18B20_Rst(void)
{
  DS18B20_IO_OUT();   //SET PA0 OUTPUT
  DS18B20_DQ_OUT = 0; //拉低DQ
  delay_us(750);      //拉低750us
  DS18B20_DQ_OUT = 1; //DQ=1
  delay_us(15);       //15US
  //DS18B20_DQ_OUT = 0; //DQ=1
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void)
{
  u8 retry = 0;
  DS18B20_IO_IN(); //SET PA0 INPUT
  while (DS18B20_DQ_IN && retry < 200)
  {
    retry++;
    delay_us(1);
  };
  if (retry >= 200)
    return 1;
  else
    retry = 0;
  while (!DS18B20_DQ_IN && retry < 240)
  {
    retry++;
    delay_us(1);
  };
  if (retry >= 240)
    return 1;
  return 0;
}
//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) // read one bit
{
  u8 data;
  DS18B20_IO_OUT(); //SET PA0 OUTPUT
  DS18B20_DQ_OUT = 0;
  delay_us(2);
  DS18B20_DQ_OUT = 1;
  DS18B20_IO_IN(); //SET PA0 INPUT

  delay_us(10);
  if (DS18B20_DQ_IN)
    data = 1;
  else
    data = 0;
  delay_us(50);
  return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(void) // read one byte
{
  u8 i, j, dat;
  dat = 0;
  for (i = 1; i <= 8; i++)
  {
    j = DS18B20_Read_Bit();
    dat = (j << 7) | (dat >> 1);
  }
  return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)
{
  u8 j;
  u8 testb;
  DS18B20_IO_OUT(); //SET PA0 OUTPUT;
  for (j = 1; j <= 8; j++)
  {
    testb = dat & 0x01;
    dat = dat >> 1;
    if (testb)
    {
      DS18B20_DQ_OUT = 0; // Write 1
      delay_us(2);
      DS18B20_DQ_OUT = 1;
      delay_us(61);
    }
    else
    {
      DS18B20_DQ_OUT = 0; // Write 0
      delay_us(61);
      DS18B20_DQ_OUT = 1;
      delay_us(2);
    }
  }
}
//开始温度转换
void DS18B20_Start(void) // ds1820 start convert
{
  DS18B20_Rst();
  DS18B20_Check();
  DS18B20_Write_Byte(0xcc); // skip rom
  DS18B20_Write_Byte(0x44); // convert
}
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
u8 DS18B20_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PORTA口时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;        // PA0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Deselect the PA0 Select high */
  GPIO_SetBits(GPIOA, GPIO_Pin_0);
  DS18B20_Rst();
//	return DS18B20_Check();
  if( DS18B20_Check())
	{	//设备不存在
			return 1;
	}
	else
	{
		DS18B20_Write_Byte(0xCC);
		DS18B20_Write_Byte(0x4E);	//开启写入DS18B20模块的功能
		DS18B20_Write_Byte(100);	//写入报警阈值上限	100℃
		DS18B20_Write_Byte(0x00);	//写入报警阈值下限	0℃
		DS18B20_Write_Byte(0x7F); //设置分辨率	0x1F 0.5℃		0x3F 0.25℃  	0x5F 0.125℃		0x7F 0.0625℃
		DS18B20_Rst();
		DS18B20_Check();
		DS18B20_Write_Byte(0xCC);
		DS18B20_Write_Byte(0x48);
		DS18B20_DQ_OUT = 1;
		return 0;
	}
}
//从ds18b20得到温度值
//返回值：温度值*10000 （-550~1250）
int DS18B20_Get_Temp(void)
{
  u8 temp;
  u8 TL, TH;
  int tem;
  DS18B20_Start(); // ds1820 start convert
  DS18B20_Rst();
  DS18B20_Check();
  DS18B20_Write_Byte(0xcc); // 发送skip ROM命令
  DS18B20_Write_Byte(0xbe); // convert
  TL = DS18B20_Read_Byte(); // LSB
  TH = DS18B20_Read_Byte(); // MSB

  if (TH > 7)
  {
    TH = ~TH;
    TL = ~TL;
    temp = 0; //温度为负
  }
  else
    temp = 1; //温度为正
  tem = TH;   //获得高八位
  tem <<= 8;
  tem += TL;                //获得底八位
  tem = tem * 625; //转换
  if (temp)
    return tem; //返回温度值
  else
    return -tem;
}
