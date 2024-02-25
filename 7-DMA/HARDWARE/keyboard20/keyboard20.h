#ifndef __KEYBOARD20_H
#define __KEYBOARD20_H	 
#include "sys.h"

// 读取引脚输入的宏
#define L1 PAin(1) /* pin1 */
#define L2 PAin(2) /* pin2 */
#define L3 PAin(3) /* pin3 */
#define L4 PAin(4) /* pin4 */

#define H5 PAout(5) /* pin5 */
#define H4 PAout(6) /* pin6 */
#define H3 PAout(7) /* pin7 */
#define H2 PBout(0) /* pin8 */
#define H1 PBout(1) /* pin9 */


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
#define KEY17_PRES  0x11
#define KEY18_PRES  0x12	
#define KEY19_PRES  0x13
#define KEY20_PRES  0x14	

extern uint8_t key_flag;

// 函数声明
void    KEYBOARD20_Init(void);
uint8_t KEYBOARD20_Scan(void);  
void    key20_process(void);

#endif