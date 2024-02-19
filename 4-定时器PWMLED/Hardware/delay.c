#include "delay.h"

// ʹ�� SysTick ʵ����ʱ���Ƚ�׼ȷ
void delay_us(uint32_t nus)
{
    // ���� SysTick �ļ�������
    SysTick_Config(SystemCoreClock/1000000); // ÿ���� 1us
    // �ر� SysTick ���жϣ�����ʡ��
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    // ��ʼ����
    for(uint32_t i=0; i<nus; i++)
    {   // ѭ��һ�ξ��� 1us
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // �ر� SysTick ��ʱ��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// ʹ�� SysTick ʵ����ʱ���Ƚ�׼ȷ
void delay_ms(uint32_t nms)
{
    // ���� SysTick �ļ�������
    SysTick_Config(SystemCoreClock/1000); // ÿ���� 1ms
    // �ر� SysTick ���жϣ�����ʡ��
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    // ��ʼ����
    for(uint32_t i=0; i<nms; i++)
    {   // ѭ��һ�ξ��� 1ms
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // �ر� SysTick ��ʱ��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// ʹ����ѭ��ʵ����ʱ
// ע�⺯��ǰ���ֹ�����Ż��ı�ʶ����ʡ��
__attribute__((optimize("-O0")))							   
void delay(uint32_t nus)
{
	while (nus--);
}

// ʹ����ѭ��ʵ����ʱ
// ע�⺯��ǰ���ֹ�����Ż��ı�ʶ����ʡ��
// __attribute__((optimize("-O0")))							   
// void delay_us(uint32_t nus)
// {
// 	while (nus--)
// 	{
// 		for(uint32_t i = 0; i<10; i++);
// 	}
// }

// ʹ����ѭ��ʵ����ʱ
// ע�⺯��ǰ���ֹ�����Ż��ı�ʶ����ʡ��
// void delay_ms(uint32_t nms)
// {	 		  	  
//     while (nms--)
// 	{
// 		delay_us(1000);
// 	}
// } 


