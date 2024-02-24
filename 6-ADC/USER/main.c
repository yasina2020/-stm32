/**
  ******************************************************************************
  * @ 名称  STM32 学习模板
  * @ 版本  STD 库 V3.5.0
  * @ 描述  适用于嵌入式虚拟仿真平台
  *         
  * @ 注意  本程序只供学习使用
  ******************************************************************************
  */


#include "iic_oled.h"
#include "sys.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "AD/ad.h"
#include <stdint.h>

//电位器接PA0----ADC123CH0----stm32f103c8只有ADC12
//ADC模拟器PA1-PA2-PA3

// 单通道
// int main()
// {
//   OLED_Init();
//   AD_Init();
//   uint16_t adc_value;
//   float volage;
//   OLED_ShowString(0, 0, "ADC:", 12);
//   OLED_ShowString(0, 1, "VOLAGE:0.00v", 12);
//     while(1)
//     {
//       adc_value = AD_GetValue();
//       volage = 0 + (float)adc_value / 4095 * (3.3 - 0);
//       OLED_ShowNum(4*6, 0, adc_value, 4, 12);   
//       OLED_ShowNum(9*6, 1, volage,1, 12);  
//       OLED_ShowNum(11*6, 1, (int)(volage * 100) % 100,2, 12);
//     }
// }

// 模拟多通道
int main()
{
  OLED_Init();
  AD_Init_CHS();
  uint16_t adc_value;
  OLED_ShowString(0, 1, "CH1:", 12);
  OLED_ShowString(0, 2, "CH2:", 12);
  OLED_ShowString(0, 3, "CH3:", 12);
  OLED_ShowString(0, 4, "CH4:", 12);

    while(1)
    {
      adc_value = AD_GetValue_CHS(0);
      OLED_ShowNum(4*6, 1, adc_value, 4, 12);   
      adc_value = AD_GetValue_CHS(1);
      OLED_ShowNum(4*6, 2, adc_value, 4, 12); 
      adc_value = AD_GetValue_CHS(2);
      OLED_ShowNum(4*6, 3, adc_value, 4, 12); 
      adc_value = AD_GetValue_CHS(3);
      OLED_ShowNum(4*6, 4, adc_value, 4, 12); 
    }
}




