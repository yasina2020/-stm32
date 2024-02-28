

#include "datapack.h"
#include "iic_oled.h"
#include "myled.h"
#include "stm32f10x.h"
#include "myusart.h"
#include "stm32f10x_gpio.h"
#include "string.h"

// --------------------HEX收发---------------------------------------------

// 收发数据4个一包，开始结束0xFF和0xFE不存。  
// 接收标志位Rev_flag 1接收到  0未接收到
char Send_Pack[4];
char Rev_Pack[4];

void Send_Hex_Pack(char *str)
{
    char send_str[10];
    // 封装包头尾发送出去
    sprintf(send_str,"0xFF%s0xFE",str);
    USART1_SendSrting(send_str);
}



// 数据包格式 0xFF 0x01 0x02 0x03 0x04 0xFE
static int state_flag = 0;
//利用状态机转移来解析接收到的数据  因为会多次进出该函数，所以设为static
// 状态0，等待接收包头
// 状态1，等待接收数据（4个
// 状态2，等待接收包尾（如果第五个状态1后面紧跟的数据不是包尾，则为错误包）
static int rev_data_end = 0;
// 接收数据包完成标志位  因为会多次进出该函数，所以设为static
static int Rev_index=0;
// 数据包中的数据部分中的第几个数据，规定为4个
extern int USART1_RevData;

// 进中断表示接收到了一个数据，此时开启这个函数进行组装，组装好之后标志位置1表示完成组装，然后读取。
void Rev_Hex_Pack()
{
    switch (state_flag) {
        case 0:
            if(USART1_RevData == 0xFF){
                state_flag = 1;
            }
            break;
        case 1:
            Rev_Pack[Rev_index++] = USART1_RevData;
            if (Rev_index==4) {
                state_flag = 2;
                Rev_index = 0;
            }
            break;
        case 2:
            if(USART1_RevData == 0xFE){
                state_flag = 0;
                rev_data_end = 1;
            }
            break;   
    }
}

// 表示接收完成，获取该标志位同时置0
int Get_Data_Rev_state()
{
    if(rev_data_end == 1)
    {
        rev_data_end = 0;
        return  1;
    }else {
        return  0;
    }
}

// -------------字符串命令控制LED亮灭----（文本模式）----------------------------------

// 因为都在串口中断，因此该函数的运行要屏蔽掉HEX的中断函数

// @开头  数据不定长  \r\n结束位   数据位最后加\0
// static int state_flag = 0;
// //利用状态机转移来解析接收到的数据  因为会多次进出该函数，所以设为static
// // 状态0，等待接收包头
// // 状态1，等待接收数据（不定长：\r结束
// // 状态2，等待接收包尾\n
// static int rev_data_end = 0;
// // 接收数据包完成标志位  因为会多次进出该函数，所以设为static
// static int Rev_index=0;
// // 数据包中的数据部分中的第几个数据
// extern int USART1_RevData;

char Rev_Str_Data[100];

void Rev_Str_Pack()
{
    switch (state_flag) {
    case 0:
        if(USART1_RevData == '@' && rev_data_end == 0){
            state_flag = 1;
            Rev_index = 0;
        }
        break;
    case 1:
        if (USART1_RevData=='!') {
            state_flag = 2;
        }else {
            Rev_Str_Data[Rev_index++] = USART1_RevData;
        }
        break;
    case 2:
        if(USART1_RevData == '!'){
            Rev_Str_Data[Rev_index] = '\0';
            state_flag = 0;
            rev_data_end = 1;
        }
        break;   
    }
}


void Ctr_LED_Str()
{
    if (strcmp(Rev_Str_Data,"LED_ON")==0) {
        LED_ON();
    }
    if (strcmp(Rev_Str_Data,"LED_OFF")==0) {
        LED_OFF();
    }
}