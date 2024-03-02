#ifndef __MYRTC_H
#define __MYRTC_H

#include "sys.h"
#include "time.h"

void my_rtc_init();
uint32_t rtc_read();
void rtc_settime(const struct tm *time_data);
struct tm rtc_readtime();

#endif