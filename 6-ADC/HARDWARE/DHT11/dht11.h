#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

//IO方向设置
//为了精确起见，这里采用寄存器写法
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
////IO操作函数											   
#define	DHT11_DQ_OUT PAout(0) //数据端口	PA0
#define	DHT11_DQ_IN  PAin(0)  //数据端口	PA0 

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(int *temp,int *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif















