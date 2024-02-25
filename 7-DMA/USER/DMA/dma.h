#ifndef __DMA_H
#define	__DMA_H	   


#include "sys.h"
#include "stm32f10x_dma.h"

void MyDMA_init(uint32_t ADDR_P,uint32_t ADDR_M,uint32_t buffer_size);
void MyDMA_Start();



#endif