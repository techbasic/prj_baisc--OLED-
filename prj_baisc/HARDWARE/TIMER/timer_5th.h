#ifndef __TIME_H
#define __TIME_H
#include "sys.h"
#include "usart.h"

extern u8 INT_FLAG_10ms;
extern u8 INT_FLAG_20ms;
extern u8 _gINT_CNT;
void TIM5_Init(u16 arr,u16 psc); 
 
#endif





