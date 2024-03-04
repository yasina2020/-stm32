#ifndef __HMOMTER_H
#define __HMOMTER_H

#include "sys.h"
#include <stdint.h>


extern uint8_t Speed_Dir;

void momter_init();

void momter_set_speed(uint16_t speed);

void momter_turn_dir();

#endif