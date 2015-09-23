#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "delay.h"
#define LED PDout(2)	// PD2	

void LED_Init(void);//≥ı ºªØ

#ifdef DEBUG_LED
void LED_test(void);
#endif
		 				    
#endif
