

#include "datapack.h"
#include "stm32f10x.h"
#include "myusart.h"

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
static u8 state_flag = 0;
//利用状态机转移来解析接收到的数据  因为会多次进出该函数，所以设为static
// 状态0，等待接收包头
// 状态1，等待接收数据（4个
// 状态2，等待接收包尾（如果第五个状态1后面紧跟的数据不是包尾，则为错误包）
static u8 rev_data_end = 0;
// 接收数据包完成标志位  因为会多次进出该函数，所以设为static
static int Rev_index=0;
// 数据包中的数据部分中的第几个数据，规定为4个
extern uint8_t USART1_RevData;

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