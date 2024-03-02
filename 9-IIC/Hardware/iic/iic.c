#include "iic.h"
#include "delay.h"
#include "iic_oled.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
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

// ----------------硬件IIC外设-------------------------

/**
  * @brief  初始化I2C1  PB6、PB7  GPIO_Mode_AF_OD
  */
void iic_h_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;//开漏输出模式仍可以输入
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);


    I2C_InitTypeDef I2C_InitStruct;
    // IIC模式
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    // 自动应答使能
    I2C_InitStruct.I2C_Ack = ENABLE;
    // 时钟，最高400KHz，选择高时钟时，
    // 占空比设置的要高一些：即让SCL高电平时间久一点，使其来得及读取和写入数据？
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    // STM做从机时，设置地址和地址位数
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_Init(I2C1,&I2C_InitStruct);
    
    //使能IIC 
    I2C_Cmd(I2C1, ENABLE);
}

/**
  * @brief  I2C事件超时等待
  */
void wait_iic_event(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
    uint32_t   timeout_cnt = 10000;
    while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
    {
        timeout_cnt --;
        if (timeout_cnt == 0) {
            return;
        }
    }
}

/**
  * @brief  向指定设备的指定地址写一个字节
  * @param  device_id：
  * @param  regaddr: 
  * @param  data: 
  * @retval None
  */
void iic_h_w(uint8_t device_id,uint8_t regaddr,uint8_t data)
{
    // 启动---地址+写---写寄存器地址---写数据---停止

    I2C_GenerateSTART(I2C1,ENABLE);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C1, device_id, I2C_Direction_Transmitter);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C1,regaddr);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

    I2C_SendData(I2C1,data);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTOP(I2C1, ENABLE);

}

/**
  * @brief  读指定设备的指定地址一个字节
  * @param  device_id：
  * @param  regaddr:  
  * @retval uint8_t
  */
uint8_t iic_h_r(uint8_t device_id,uint8_t regaddr)
{
    // 开始---地址+写---写寄存器地址---开始---地址+读---读数据---

    I2C_GenerateSTART(I2C1,ENABLE);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C1, device_id, I2C_Direction_Transmitter);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C1,regaddr);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    // 再次开始
    I2C_GenerateSTART(I2C1,ENABLE);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
    
    //因为要指定寄存器地址，所以第一次需要告诉从机：设备id和寄存器地址，故第一次是写的流程
    // 之后就可以进入读的流程，再次开始，写设备id+读，表示要让从机发数据，发刚写的寄存器地址的那个数据 
    // 所以这里是  设备+读  发送地址完成事件（EV6）
    I2C_Send7bitAddress(I2C1, device_id, I2C_Direction_Receiver);
    wait_iic_event(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    
    //此处先将应答改为NACK，那么当收到数据后就会告诉从机不要数据了，因为只接受一个字节，接收后再改就会使从机再次发送数据。
    //这里的STOP同理，且STOP会在接收数据完成后在执行  
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);

    // 在接收数据之前，等待DR为可读的状态
    wait_iic_event(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
    uint8_t data = I2C_ReceiveData(I2C1);

    //为了以后再接收数据，将ACK恢复 
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    return data;
}























