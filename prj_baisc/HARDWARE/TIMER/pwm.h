#ifndef _PWM_H
#define _PWM_H

#include "sys.h"
#include "delay.h"
#include "usart.h"




void Motor_Init(u16 arr,u16 psc);
#ifdef DEBUG_PWM
void PWM_test(void);
#endif 

#endif 



