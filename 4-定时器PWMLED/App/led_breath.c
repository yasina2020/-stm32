

void Led_breath()
{
	for (uint16_t i = 0; i <= 100; i++)
	{
		PWM_SetCompare3(i);			//���ν���ʱ����CCR�Ĵ�������Ϊ0~100��PWMռ�ձ�������LED�𽥱���
		delay_ms(10);				//��ʱ10ms
	}
	for (uint16_t i = 0; i <= 100; i++)
	{
		PWM_SetCompare3(100 - i);	//���ν���ʱ����CCR�Ĵ�������Ϊ100~0��PWMռ�ձ��𽥼�С��LED�𽥱䰵
		delay_ms(10);				//��ʱ10ms
	}

}
