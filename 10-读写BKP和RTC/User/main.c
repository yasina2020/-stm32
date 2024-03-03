/**
  ******************************************************************************
  * @ 名称  STM32 学习模板
  * @ 版本  STD 库 V3.5.0
  * @ 描述  适用于嵌入式虚拟仿真平台
  *         
  * @ 注意  本程序只供学习使用
  ******************************************************************************
  */


#include "MyRTC.h"
#include "sys.h"
#include "stm32f10x_conf.h"
#include "delay.h"
#include "iic_oled.h"
#include "bkp.h"
#include <time.h>

void test_bkp()
{
  bkp_init();
  bkp_write(BKP_DR1, 1234);
  uint16_t data = bkp_read(BKP_DR1);
  OLED_ShowNum(0, 0, data, 4, 12);
}

void test_rtc()
{
  my_rtc_init();
  OLED_ShowNum(0, 0, rtc_read(), 10, 12);
}
// 阿拉斯加搭理你收到啦没打码阿妈
int main()
{
  OLED_Init();

  // test_bkp();
  my_rtc_init();
  
  struct tm time_data;
  time_data.tm_year = 2024;
  time_data.tm_mon = 3;
  time_data.tm_mday = 1;
  time_data.tm_hour = 13;
  time_data.tm_min = 56;
  time_data.tm_sec = 34;
  rtc_settime(&time_data);

  while(1)
  {
    OLED_ShowNum(0, 0, rtc_read(), 10, 12);

    struct tm now_time;
    now_time = rtc_readtime();
    OLED_ShowNum(0, 1, now_time.tm_year, 4, 12);
    OLED_ShowNum(24, 1, now_time.tm_mon, 2, 12);
    OLED_ShowNum(37, 1, now_time.tm_mday, 2, 12);
    OLED_ShowNum(52, 1, now_time.tm_hour, 2, 12);
    OLED_ShowNum(67, 1, now_time.tm_min, 2, 12);
    OLED_ShowNum(81, 1, now_time.tm_sec, 2, 12);
    
  }
}
