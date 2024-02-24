#ifndef __AD_H
#define __AD_H

#include "stm32f10x.h"

void AD_Init();
uint16_t AD_GetValue();
float AD_GetVolite();
#endif
