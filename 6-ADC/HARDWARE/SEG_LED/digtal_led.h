#ifndef _DIGTAL_LED_H_
#define _DIGTAL_LED_H_

#include "sys.h"
#include "stm32f10x_conf.h"

void digtal_LED_Init(void);
void show_digtal_one_segment(uint32_t num);
void show_digtal_four_segment(uint32_t num);


#endif
