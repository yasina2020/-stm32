#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"


// 对SCL写1或者写0，也就是拉高或者拉低SCL
void my_IIC_W_SCL(uint8_t BitValue);
// 对SDA写1或者写0，也就是拉高或者拉低SDA
void my_IIC_W_SDA(uint8_t BitValue);

// 对SDA读
uint8_t my_IIC_R_SDA();

// GPIO初始化
void My_IIC_GPIO_Init();


// 起始条件：SCL高期间，SDA下降沿
void My_IIC_Start();

// 终止条件：SCL高期间，SDA上升沿
void My_IIC_Stop();

//写一个字节
void My_IIC_WriteByte(uint8_t Byte);
// 读一个字节
uint8_t My_IIC_ReadByte();

// 发送应答
void My_IIC_SendACK(uint8_t ACK);

// 接收应答
uint8_t My_IIC_RevACK();








// ----------------------------------------------------------------------
uint8_t iic_h_r(uint8_t device_id,uint8_t regaddr);
void iic_h_w(uint8_t device_id,uint8_t regaddr,uint8_t data);
void wait_iic_event(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void iic_h_init();






#endif