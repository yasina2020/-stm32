#include "bkp.h"
#include "usart.h"


void TAMPER_ITConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
	NVIC_Init(&NVIC_InitStructure);//初始化侵入中断的优先级
	BKP_ITConfig(ENABLE);//使能侵入中断
}


void TAMPER_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能电源管理单元的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问
	
	BKP_TamperPinCmd(DISABLE);	 //先关闭侵入检测引脚
	BKP_ITConfig(DISABLE);      //关闭侵入中断
	BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);	//设置检测引脚低电平有效
	BKP_ClearFlag();	//清除侵入检测事件
	TAMPER_ITConfig();//配置中断优先级并打开侵入中断，不需要进入中断可以注释掉这句，并不影响侵入事件的发生
	BKP_TamperPinCmd(ENABLE);//开启侵入检测引脚

}



void TAMPER_IRQHandler(void)
{
    if(BKP_GetITStatus()!=RESET){
        printf("触发侵入中断\r\n");
	BKP_ClearITPendingBit();//清除侵入检测中断
	BKP_ClearFlag();//清除侵入检测事件
    }
}


// -----------------------------------------
void bkp_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE);
}

uint16_t bkp_read(uint16_t dr)
{
    return BKP_ReadBackupRegister(dr);
}

void bkp_write(uint16_t dr,uint16_t data)
{
    BKP_WriteBackupRegister(dr, data);
}
