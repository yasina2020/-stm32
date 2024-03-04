#include "stm32f10x.h"                  // Device header
#include "exti14.h"



int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	EXTI_Init_14();
	while(1)
	{
	
	}
}
