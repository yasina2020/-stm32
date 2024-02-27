#include "keyboard16.h"
#include <stdio.h>

uint8_t KEY16 = 0;

// 初始化矩阵键盘的引脚
void KEYBOARD16_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;                           //行，推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;                           //行，推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;                              // 列，下拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//矩阵键盘扫描函数
uint8_t KEYBOARD16_Scan(void)
{	
    static uint8_t flag1=1,flag2=1,flag3=1,flag4=1;
    uint8_t KEY16 = 0;
    h1 = 1;
    h2 = 0;
    h3 = 0;
    h4 = 0;
    if(flag1&&(l1==1||l2==1||l3==1||l4==1))
    {	
        flag1=0;
        if(l1==1)
        {
            KEY16=KEY1_PRES;
        }
        else if(l2==1)
            KEY16=KEY2_PRES;
        else if(l3==1)
            KEY16=KEY3_PRES;		
        else if(l4==1)
            KEY16=KEY4_PRES;
    }
    else if(l1==0&&l2==0&&l3==0&&l4==0)
        flag1=1;
    // else 
    //     return 0;
		
    h1 = 0;
    h2 = 1;
    h3 = 0;
    h4 = 0;
    if(flag2&&(l1==1||l2==1||l3==1||l4==1))
    {	
        flag2=0;
        if(l1==1)
            KEY16=KEY5_PRES;
        else if(l2==1)
            KEY16=KEY6_PRES;
        else if(l3==1)
            KEY16=KEY7_PRES;		
        else if(l4==1)
            KEY16=KEY8_PRES;
    }
    else if(l1==0&&l2==0&&l3==0&&l4==0)
        flag2=1;
    // else 
    //     return 0;
		  
    h1 = 0;
    h2 = 0;
    h3 = 1;
    h4 = 0;
    if(flag3&&(l1==1||l2==1||l3==1||l4==1))
    {	
        flag3=0;
        if(l1==1)
            KEY16=KEY9_PRES;
        else if(l2==1)
            KEY16=KEY10_PRES;
        else if(l3==1)
            KEY16=KEY11_PRES;		
        else if(l4==1)
            KEY16=KEY12_PRES;
    }
    else if(l1==0&&l2==0&&l3==0&&l4==0)
        flag3=1;
    // else 
    //     return 0;
		
	h4 = 1;
    h1 = 0;
    h2 = 0;
    h3 = 0;
    if(flag4&&(l1==1||l2==1||l3==1||l4==1))
    {	
        flag4=0;
        if(l1==1)
            KEY16=KEY13_PRES;
        else if(l2==1)
            KEY16=KEY14_PRES;
        else if(l3==1)
            KEY16=KEY15_PRES;		
        else if(l4==1)
            KEY16=KEY16_PRES;
    }
    else if(l1==0&&l2==0&&l3==0&&l4==0)
        flag4=1;
    // else 
    //     return 0;
			
    return KEY16;			
}
		
//键值处理函数
void key16_process(void)
{
    //串口打印信息
    switch (KEY16)
    {
    case 0x01:
        printf("input 'K1'\n");
        break;
    case 0x02:
        printf("input 'K2'\n");
        break;
    case 0x03:
        printf("input 'K3'\n");
        break;
    case 0x04:
        printf("input 'K4'\n");
        break;
    case 0x05:
        printf("input 'K5'\n");
        break;
    case 0x06:
        printf("input 'K6'\n");
        break;
    case 0x07:
        printf("input 'K7'\n");
        break;
    case 0x08:
        printf("input 'K8'\n");
        break;
    case 0x09:
        printf("input 'K9'\n");
        break;
    case 0x0a:
        printf("input 'K10'\n");
        break;
    case 0x0b:
        printf("input 'K11'\n");
        break;
    case 0x0c:
        printf("input 'K12'\n");
        break;
    case 0x0d:
        printf("input 'K13'\n");
        break;
    case 0x0e:
        printf("input 'K14'\n");
        break;
    case 0x0f:
        printf("input 'K15'\n");
        break;
    case 0x10:
        printf("input 'K16'\n");
        break;
    default:
        break;
    }
    KEY16 = 0;
}
