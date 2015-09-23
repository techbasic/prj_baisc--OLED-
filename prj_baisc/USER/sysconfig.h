#ifndef _SYSCONFIG_H
#define _SYSCONFIG_H

#include "stm32f10x.h"
/*SYS CONFIG */
#define PID_MODE single_PID
#define single_PID 0	//angle
#define double_PID 1	//angle and rate


/*commpc*/
enum {SRC_PC=0,SRC_APP};
extern uint8_t btSrc;
#define BT_SRC_PC			//both ok


/*DEBUG CONFIG TABLE*/
#define DEBUG_PID	
//#define DEBUG_MPU
#define DEBUG_COMMPC
#define DEBUG_PWM
//#define DEBUG_LED
//#define DEBUG_ADC
#define DEBUG_OLED


#endif





