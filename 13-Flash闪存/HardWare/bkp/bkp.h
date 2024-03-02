#ifndef __BKP_H
#define __BKP_H

#include "sys.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_bkp.h"

void TAMPER_Init(void);
#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>
void bkp_init();
uint16_t bkp_read(uint16_t dr);

void bkp_write(uint16_t dr,uint16_t data);	

#endif

