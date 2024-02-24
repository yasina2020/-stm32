#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc_Init(void);
void Adc_NVIC_Config(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
void ADC1_DMA_Config(void);

#endif 
