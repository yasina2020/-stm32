#include "stm32f10x.h"                  // Device header




//IWDG
void iwdg_foo()
{
	//�жϸ�λ��Դ
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)
	{
		//TODO IWDG RESET	
	}else
	{
		//������λ
	}
	
		/*IWDG��ʼ��*/
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//�������Ź�дʹ�ܣ��������
	//LSI = 40K��
	/*
	
	��ʱʱ�乫ʽ��Ti = Tlsi X PRԤ��Ƶϵ�� X (RL + 1)
	�����ó�ʱʱ��1000ms������PR��Ҫ����Ϊ16��Ƶ     IWDG_SetPrescaler(IWDG_Prescaler_16);
	Tlsi = 1/40 ��λ��ms
	�ʵó�PL = 2499     IWDG_SetReload(2500 - 1)
	
	*/
	IWDG_SetPrescaler(IWDG_Prescaler_16);			//����Ԥ��ƵΪ16
	IWDG_SetReload(2500 - 1);							//������װֵΪ2499���������Ź��ĳ�ʱʱ��Ϊ1000ms
	IWDG_ReloadCounter();							//��װ��������ι��
	IWDG_Enable();									//�������Ź�ʹ��
	
	while(1)
	{
		//ι��
		IWDG_ReloadCounter();						//��װ��������ι��
	}
}

//WWDG
void wwdg_foo()
{
	//�жϸ�λ��Դ
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)
	{
	//TODO WWDG RESET	
	}else
	{
	//������λ
	}
	
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	//����WWDG��ʱ��
	
	/*WWDG��ʼ��*/
	//APB1 36MHz  
	/*
	�����ó�ʱʱ��50ms������ó�WDGTB=3    WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	��ʱʱ�乫ʽ��Tw = Tplck1 X 4096 X 2^WDGTB X (T[5:0] + 1)
	Tw:50ms
	Tplck1:1/36k(�����Ժ���Ϊ��λ)
	WDGTB:3
	�ó�T[5:0] = 54     WWDG_Enable(0x40 | 54);
	
	���ڳ�ʱ��ʽ��Tw = Tplck1 X 4096 X 2^WDGTB X (T[5:0] -W[5:0])
	�����ô���ʱ��30ms
	�ó�W[5:0] = 21     WWDG_SetWindowValue(0x40 | 21);
	*/
	WWDG_SetPrescaler(WWDG_Prescaler_8);			//����Ԥ��ƵΪ8
	WWDG_SetWindowValue(0x40 | 21);					//���ô���ֵ������ʱ��Ϊ30ms
	WWDG_Enable(0x40 | 54);							//ʹ�ܲ���һ��ι������ʱʱ��Ϊ50ms
	
	while(1)
	{
		//ι��
		WWDG_SetCounter(0x40 | 54);					//��װ��������ι��
	}
}

int main()
{
	void wwdg_foo();
	//void iwdg_foo();

}
