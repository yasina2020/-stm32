
#include "MyRTC.h"
#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include <stdint.h>
#include <time.h>

void my_rtc_init()
{
    // 1.  时钟配置和RTC功能使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    // 2.  启动LSE（默认关闭）`RCC-LSEConfig`
    RCC_LSEConfig(RCC_LSE_ON);
    while ( RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待LSE启动完成
    // TODO 超时等待策略
    // 3.  设置RTCCLK选择LSE `RCC_RTCCLKConfig、RCC_RTCCLKCmd`
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    // 4.  等待APB总线和RTC时钟同步、等待上一次RTC操作完成
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    // 5.  设置与分频器使其输出1Hz
    // RTC_SetPrescaler(32768 - 1);//LSE是32.768KHz
     RTC_SetPrescaler(327);
    RTC_WaitForLastTask();
        //每次写RTC时，需要进入和退出配置模式，不过库函数已经封装好了 
    // 6. 写入一个初始时间，非必须
    // RTC_SetCounter(0);
    // RTC_WaitForLastTask();

    // 7.  读CNT计数器
    // 8.  可选：设置闹钟
    // 9.  可选：设置中断
}

uint32_t rtc_read()
{
    return RTC_GetCounter();
}


void rtc_settime(const struct tm *time_data)
{
    time_t time_cnt;
    struct tm time_d;
    time_d.tm_year = time_data->tm_year - 1900;
    time_d.tm_mon = time_data->tm_mon - 1;
    time_d.tm_mday = time_data->tm_mday;
    time_d.tm_hour = time_data->tm_hour;
    time_d.tm_min = time_data->tm_min;
    time_d.tm_sec = time_data->tm_sec;

    // time_data->tm_year = time_data->tm_year - 1900;
    // time_data->tm_mon = time_data->tm_mon - 1;

    time_cnt = mktime(&time_d);
    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}

struct tm rtc_readtime()
{
    struct tm time_data;
    time_t time_cnt = RTC_GetCounter();
    RTC_WaitForLastTask();

    time_data = *localtime(&time_cnt);
    time_data.tm_year+=1900;
    time_data.tm_mon+=1;
    return  time_data;
}

