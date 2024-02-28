/**
  ******************************************************************************
  * @ 名称  STM32 学习模板
  * @ 版本  STD 库 V3.5.0
  * @ 描述  适用于嵌入式虚拟仿真平台
  *         
  * @ 注意  本程序只供学习使用
  ******************************************************************************
  */


#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "MYUSART/myusart.h"
#include "iic_oled.h"
#include <stdio.h>
#include "DATAPACK/datapack.h"

void Test_Usart1()
{
// USART发送测试
    USART1_SendData('A');
    USART1_SendSrting("ABCDEFG");
    u8 arr[] = {'1','2','3','4','5','6'};
    USART1_SendArray(arr, sizeof(arr)/sizeof(u8));
    USART1_SendNum(12345);

// 利用sPrintf格式化输出
    char str[1000];
    sprintf(str, "Num:%d\n",666);
    USART1_SendSrting(str);
}

extern u8 Rev_Pack[4];
extern char Rev_Str_Data[100];

void Test_Hex_Pack()
{
// USART发送hexPack
    char Send_HexPack[] = {0x31,0x32,0x33,0x34};
    Send_Hex_Pack(Send_HexPack);
    
}


int main()
{
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC分组
    USART1_Init(115200);
    OLED_Init();
    LED_Init();

    Test_Usart1();
    Test_Hex_Pack();
    while(1)
    {
        // OLED_ShowNum(0, 0, USART1_GetRev(), 2, 12);
        //---------------HEX模式收，显示在OLED上---------------------------
        // if(Get_Data_Rev_state() == 1)
        // {
        //     OLED_ShowChar(0, 0, Rev_Pack[0], 12);
        //     OLED_ShowChar(6, 0, Rev_Pack[1], 12);
        //     OLED_ShowChar(12, 0, Rev_Pack[2], 12);
        //     OLED_ShowChar(18, 0, Rev_Pack[3], 12);

        // }
        // -----------SRT模式收，控制LED亮灭------------------------------------
        if (Get_Data_Rev_state() == 1) {
            Ctr_LED_Str();
            OLED_ShowString(0, 0, Rev_Str_Data, 12);
        }
        OLED_ShowString(0, 2, Rev_Str_Data, 12);  
    }
}
