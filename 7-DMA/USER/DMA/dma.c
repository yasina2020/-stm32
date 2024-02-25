
#include "DMA/dma.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

void MyDMA_init(uint32_t ADDR_P,uint32_t ADDR_M,uint32_t buffer_size)
{
    //RCC
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //DMA_Init
    DMA_InitTypeDef DMA_InitStruct;
    
    DMA_InitStruct.DMA_PeripheralBaseAddr = ADDR_P;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryBaseAddr = ADDR_M;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//方向选择  P-》M
    DMA_InitStruct.DMA_BufferSize = buffer_size;//计数器
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//自动化重装： 否
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;//软件触发
    
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1,&DMA_InitStruct);

    DMA_Cmd(DMA1_Channel1, ENABLE);

}

void MyDMA_Start()
{
    DMA_Cmd(DMA1_Channel1, ENABLE);
}