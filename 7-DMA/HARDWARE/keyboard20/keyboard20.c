#include "keyboard20.h"
#include <stdio.h>

uint8_t KEY20 = 0;

// 初始化矩阵键盘的引脚
void KEYBOARD20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //行，推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //行，推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 列，下拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//矩阵键盘扫描函数
uint8_t KEYBOARD20_Scan(void)
{
    static uint8_t flag1 = 1, flag2 = 1, flag3 = 1, flag4 = 1, flag5 = 1;

    H1 = 1;
    H2 = 0;
    H3 = 0;
    H4 = 0;
    H5 = 0;
    if (flag1 && (L1 == 1 || L2 == 1 || L3 == 1 || L4 == 1))
    {
        flag1 = 0;
        if (L1 == 1)
            KEY20 = KEY1_PRES;
        else if (L2 == 1)
            KEY20 = KEY2_PRES;
        else if (L3 == 1)
            KEY20 = KEY3_PRES;
        else if (L4 == 1)
            KEY20 = KEY4_PRES;
    }
    else if (L1 == 0 && L2 == 0 && L3 == 0 && L4 == 0)
        flag1 = 1;
    // else
    //     return 0;

    H1 = 0;
    H2 = 1;
    H3 = 0;
    H4 = 0;
    H5 = 0;
    if (flag2 && (L1 == 1 || L2 == 1 || L3 == 1 || L4 == 1))
    {
        flag2 = 0;
        if (L1 == 1)
            KEY20 = KEY5_PRES;
        else if (L2 == 1)
            KEY20 = KEY6_PRES;
        else if (L3 == 1)
            KEY20 = KEY7_PRES;
        else if (L4 == 1)
            KEY20 = KEY8_PRES;
    }
    else if (L1 == 0 && L2 == 0 && L3 == 0 && L4 == 0)
        flag2 = 1;
    // else
    //     return 0;

    H1 = 0;
    H2 = 0;
    H3 = 1;
    H4 = 0;
    H5 = 0;
    if (flag3 && (L1 == 1 || L2 == 1 || L3 == 1 || L4 == 1))
    {
        flag3 = 0;
        if (L1 == 1)
            KEY20 = KEY9_PRES;
        else if (L2 == 1)
            KEY20 = KEY10_PRES;
        else if (L3 == 1)
            KEY20 = KEY11_PRES;
        else if (L4 == 1)
            KEY20 = KEY12_PRES;
    }
    else if (L1 == 0 && L2 == 0 && L3 == 0 && L4 == 0)
        flag3 = 1;
    // else
    //     return 0;

    H1 = 0;
    H2 = 0;
    H3 = 0;
    H4 = 1;
    H5 = 0;
    if (flag4 && (L1 == 1 || L2 == 1 || L3 == 1 || L4 == 1))
    {
        flag4 = 0;
        if (L1 == 1)
            KEY20 = KEY13_PRES;
        else if (L2 == 1)
            KEY20 = KEY14_PRES;
        else if (L3 == 1)
            KEY20 = KEY15_PRES;
        else if (L4 == 1)
            KEY20 = KEY16_PRES;
    }
    else if (L1 == 0 && L2 == 0 && L3 == 0 && L4 == 0)
        flag4 = 1;
    // else
    //     return 0;

    H5 = 1;
    H1 = 0;
    H2 = 0;
    H3 = 0;
    H4 = 0;
    if (flag5 && (L1 == 1 || L2 == 1 || L3 == 1 || L4 == 1))
    {
        flag5 = 0;
        if (L1 == 1)
            KEY20 = KEY17_PRES;
        else if (L2 == 1)
            KEY20 = KEY18_PRES;
        else if (L3 == 1)
            KEY20 = KEY19_PRES;
        else if (L4 == 1)
            KEY20 = KEY20_PRES;
    }

    else if (L1 == 0 && L2 == 0 && L3 == 0 && L4 == 0)
        flag5 = 1;
    // else
    //     return 0;
    return KEY20;
}

//键值处理函数
void key20_process(void)
{
    //串口打印信息
    switch (KEY20)
    {
    case 0x01:
        printf("input 'F1'\n");
        break;
    case 0x02:
        printf("input 'F2'\n");
        break;
    case 0x03:
        printf("input '#'\n");
        break;
    case 0x04:
        printf("input '*'\n");
        break;
    case 0x05:
        printf("input '1'\n");
        break;
    case 0x06:
        printf("input '2'\n");
        break;
    case 0x07:
        printf("input '3'\n");
        break;
    case 0x08:
        printf("input 'UP'\n");
        break;
    case 0x09:
        printf("input '4'\n");
        break;
    case 0x0a:
        printf("input '5'\n");
        break;
    case 0x0b:
        printf("input '6'\n");
        break;
    case 0x0c:
        printf("input 'DOWN'\n");
        break;
    case 0x0d:
        printf("input '7'\n");
        break;
    case 0x0e:
        printf("input '8'\n");
        break;
    case 0x0f:
        printf("input '9'\n");
        break;
    case 0x10:
        printf("input 'Esc'\n");
        break;
    case 0x11:
        printf("input 'LEFT' \n");
        break;
    case 0x12:
        printf("input '0'\n");
        break;
    case 0x13:
        printf("input 'RIGHT'\n");
        break;
    case 0x14:
        printf("input 'Ent'\n");
        break;
    default:
        break;
    }
    KEY20 = 0;
}