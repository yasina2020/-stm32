#include "iic.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>


// ----------------软件实现IIC-------------------------
/*


*/ 

// 对SCL写1或者写0，也就是拉高或者拉低SCL
void my_IIC_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction) BitValue);
    delay_us(10);
}

// 对SDA写1或者写0，也就是拉高或者拉低SDA
void my_IIC_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction) BitValue);
    delay_us(10);
}

// 对SDA读
uint8_t my_IIC_R_SDA()
{
    uint8_t bitValue = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
    delay_us(10);
    return bitValue;
}

// GPIO初始化
void My_IIC_GPIO_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出模式仍可以输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //两者处于高电平，ICC空闲
    my_IIC_W_SCL(1);
    my_IIC_W_SDA(1);
}


// 起始条件：SCL高期间，SDA下降沿
void My_IIC_Start()
{
    //两者处于高电平，ICC空闲
    my_IIC_W_SDA(1);
    my_IIC_W_SCL(1);
    //SCL高期间，SDA下降沿
    my_IIC_W_SDA(0);
    // 释放SCL
    my_IIC_W_SCL(0);
    
}

// 终止条件：SCL高期间，SDA上升沿
void My_IIC_Stop()
{
    my_IIC_W_SDA(0);
    my_IIC_W_SCL(1);

    my_IIC_W_SDA(1);
    
}

//写一个字节
void My_IIC_WriteByte(uint8_t Byte)
{
    uint8_t i=8;
    while(i--)
    {
        // 依次读Byte最高位是几  位与  然后再决定SDA拉高还是释放 
        if((Byte & 0x80) == 0x80)
        {
            my_IIC_W_SDA(1);
        }else 
        {
            my_IIC_W_SDA(0);
        }
        my_IIC_W_SCL(1);//SDA放好数据后，SCL拉高，这时候从机去读SDA
        my_IIC_W_SCL(0);//拉低SCL，表示开始传输下一位

        Byte >>= 1;//Byte右移1位,保证最高位是新的
    }
}
// 读一个字节
uint8_t My_IIC_ReadByte()
{
    uint8_t Byte = 0x00;

    my_IIC_W_SDA(1);//释放SDA？，方便从机控制写数据

    for(uint8_t i =0; i < 8; i ++) 
    {
        my_IIC_W_SCL(1);
        if (my_IIC_R_SDA() == 1) 
        {
            // 高位先行，读出来后不断右移读次低位
            Byte |= (0x80>>i);
        }
        my_IIC_W_SCL(0);
    }
    return  Byte;
}

// 发送应答
void My_IIC_SendACK(uint8_t ACK)
{

        my_IIC_W_SDA(ACK);
        my_IIC_W_SCL(1);

        my_IIC_W_SCL(0);
}

// 接收应答
uint8_t My_IIC_RevACK()
{
    my_IIC_W_SDA(1);

    my_IIC_W_SCL(1);
    uint8_t ACK = my_IIC_R_SDA();

    my_IIC_W_SCL(0);
    return ACK;
}





