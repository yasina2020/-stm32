#ifndef LCD1602_H
#define LCD1602_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define LCD1602A_CMD1 0X1    //Clear display
#define LCD1602A_CMD2 0X2    //Return home
#define LCD1602A_CMD3 0X4    //Entry mode set
#define LCD1602A_CMD4 0X8    //Display on/off control
#define LCD1602A_CMD5 0X10   //Cursor or display shift
#define LCD1602A_CMD6 0X20   //Function set
#define LCD1602A_CMD7 0X40   //Set CGRAM address
#define LCD1602A_CMD8 0X80   //Set DDRAM address



#define LCD1602A_CMD3_ID_SET   (0x1<<1)       //AC++ 
#define LCD1602A_CMD3_ID_RESET (~(0x1<<1))    //AC--
#define LCD1602A_CMD3_S_SET    (0x1<<0)       //SCREEN SHIFT
#define LCD1602A_CMD3_S_RESET  (~(0x1<<0))    //SCREEN NO SHIFT


#define LCD1602A_CMD4_D_SET   (0x1<<2)       //DISPLAY ON
#define LCD1602A_CMD4_D_RESET (~(0x1<<2))    //DISPLAY OFF
#define LCD1602A_CMD4_C_SET   (0x1<<1)       //CURSOR DISPLAY
#define LCD1602A_CMD4_C_RESET (~(0x1<<1))    //CURSOR NO DISPLAY
#define LCD1602A_CMD4_B_SET   (0x1<<0)       //
#define LCD1602A_CMD4_B_RESET (~(0x1<<0))    //


#define LCD1602A_CMD5_SC_SET   (0x1<<3)       
#define LCD1602A_CMD5_SC_RESET (~(0x1<<3))    
#define LCD1602A_CMD5_RL_SET   (0x1<<2)      
#define LCD1602A_CMD5_RL_RESET (~(0x1<<2))   

#define LCD1602A_CMD6_DL_SET   (0x1<<4)       
#define LCD1602A_CMD6_DL_RESET (~(0x1<<4))    
#define LCD1602A_CMD6_N_SET    (0x1<<3)       
#define LCD1602A_CMD6_N_RESET  (~(0x1<<3))    
#define LCD1602A_CMD6_F_SET    (0x1<<2)       
#define LCD1602A_CMD6_F_RESET  (~(0x1<<2)) 

/*******************************************************************************/
#define BUSY 0x80		//
#define RS GPIO_Pin_5	//
#define RW GPIO_Pin_6	//
#define EN GPIO_Pin_7	//
/*******************************************************************************/

/*******************************************************************************/
void ReadBusy(void);
void LCD_WRITE_CMD( unsigned char CMD );
void LCD_WRITE_StrDATA( char *StrData, unsigned char row, unsigned char col );
void LCD_WRITE_ByteDATA( unsigned char ByteData );
void LCD_INIT(void);
void GPIO_INIT(void);
void WUserImg(unsigned char pos,unsigned char *ImgInfo);
/*******************************************************************************/
#endif
