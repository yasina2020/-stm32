#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

#define HX711_SCK PBout(8) // 
#define HX711_DOUT PBin(9) // 

// 电子称传感器 1g 数据对应 420 AD值
#define scalar    420

// 初始化 HX711 引脚
void Init_HX711pin(void);
// 读取 HX711 数据
u32 HX711_Read(void);

#endif
