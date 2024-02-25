#ifndef __MQ_SENSOR_H
#define __MQ_SENSOR_H
#include <math.h>
#include <stdio.h>

#include "sys.h"
#include "adc.h"
#include "delay.h"

float GetMQ8Out(void);
float GetMQ135Out(void);
float GetMQ136Out(void);
float GetMQ137Out(void);
float GetMQ138Out(void);

#endif