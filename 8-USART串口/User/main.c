#include "stm32f10x.h"                  // Device header
#include "USART/myusart.h"
#include "OLED/OLED.h"


int main()
{
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC·Ö×é
    USART1_Init(9600);
    OLED_Init();
    USART1_SendData('A');
    USART1_SendSrting("ABCDEFG");
    // u16 arr[] = {0,1,2,3,4,5};
    // USART1_SendArray(arr, sizeof(arr)/sizeof(u16));
   
    u16 RecDate;
    while(1)
    {
		OLED_ShowNum(0, 0, USART1_GetRev(), 2);
      
    }
}