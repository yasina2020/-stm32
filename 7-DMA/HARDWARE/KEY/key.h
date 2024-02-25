#ifndef __KEY_H
#define __KEY_H	 
 

#define KEY PAin(1)
	 
// 将 PA1 配置为 GPIO 下拉输入功能
void KEY_Init(void);

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);	

#define KEY_ON  1
#define KEY_OFF 0

#endif
