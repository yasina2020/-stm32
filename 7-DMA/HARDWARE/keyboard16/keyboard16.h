#ifndef __KEYBOARD16_H
#define __KEYBOARD16_H	 
#include "sys.h"

// 读取引脚输入的宏
#define h1 PBout(7) /* pin1 */
#define h2 PBout(6) /* pin2 */
#define h3 PBout(5) /* pin3 */
#define h4 PAout(8) /* pin4 */

#define l1 PBin(15) /* pin5 */
#define l2 PBin(14) /* pin6 */
#define l3 PBin(13) /* pin7 */
#define l4 PBin(12) /* pin8 */

// 键值定义
#define KEY1_PRES	0x01
#define KEY2_PRES	0x02
#define KEY3_PRES	0x03		
#define KEY4_PRES	0x04		
#define KEY5_PRES	0x05	
#define KEY6_PRES	0x06	
#define KEY7_PRES	0x07		
#define KEY8_PRES   0x08
#define KEY9_PRES   0x09
#define KEY10_PRES  0x0a	
#define KEY11_PRES  0x0b	
#define KEY12_PRES  0x0c	
#define KEY13_PRES  0x0d			
#define KEY14_PRES  0x0e			
#define KEY15_PRES  0x0f	
#define KEY16_PRES  0x10

// 函数声明
void    KEYBOARD16_Init(void);
uint8_t KEYBOARD16_Scan(void);  
void    key16_process(void);

#endif