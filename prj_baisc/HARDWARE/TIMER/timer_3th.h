#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//定时器没有连接引脚，只有一个初始化函数
//声明初始化函数
void TIM2_Int_Init(u16 arr,u16 psc);
#endif
