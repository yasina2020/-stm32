#include "GP2Y1014.h"

#define GP2Y_High GPIO_SetBits(GPIOA, GPIO_Pin_0);
#define GP2Y_Low  GPIO_ResetBits(GPIOA, GPIO_Pin_0);

static unsigned int adc_value = 0; //保存ADC数值
double PM2_5 = 0;   //保存PM2.5浓度值

void GetGP2Y ( void ) 
{
    
    //采样检测逻辑
    GP2Y_Low;
    delay_ms(1);
    delay_us ( 280 );
    adc_value = Get_Adc(1);  
    delay_us ( 40 );
    GP2Y_High;
    //延时9680微秒 延时目的在于手册中提到需确保LED的周期为10ms
    delay_us ( 1500 );
    delay_us ( 1500 );
    delay_us ( 1500 );
    delay_us ( 1500 );
    delay_us ( 1500 );
    delay_us ( 1500 );
    delay_us ( 680 );
    printf("AD value: %d\n", adc_value);
    //将ADC值转化为PM2.5浓度值
    PM2_5 = ( adc_value * 3.3 * 1000 / 4096 * 5 - 3 ) / 34;
    printf ( "PM2.5: %f mg/m3\r\n", PM2_5);
}