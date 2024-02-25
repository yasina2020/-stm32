#ifndef __AD_H
#define __AD_H

#include "stm32f10x.h"

void AD_Init();
uint16_t AD_GetValue();

void AD_Init_CHS();
uint16_t AD_GetValue_CHS(uint8_t ADC_Channel);

void ADC_DMA_Init();
void ADC_DMA_GetValue();

#endif
