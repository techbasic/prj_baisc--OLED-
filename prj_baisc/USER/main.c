#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "exti.h" 
#include "adc.h"
#include "inv_mpu.h"					
#include "inv_mpu_dmp_motion_driver.h" 
#include "pwm.h"
#include "timer_3th.h"
#include "timer_5th.h"
#include "CommPc.h"
#include "usart2.h"
#include "hc05.h"
#include "control.h"
#include "oled.h"
//SCL:PC12   SDA: PC11   AD0:GND   VCC:5V  

int main(void)
{ 		
	Stm32_Clock_Init(9);
//		u16 temp;
	NVIC_Configuration();
	UART1_init(72,115200); 
	delay_init();  
	//��ʼ������1��
	
	pid_Init();

	LED_Init();
	EXTIX_Init();
	Adc_Init();		


	Motor_Init(999,0);	
	
	HC05_Init();
	while(mpu_dmp_init());	 

// 	TIM5_Init(999,72);
//	TIM2_Int_Init(999,72);//��ʱ��3���ã�1ms�ж�һ��

		OLED_Init();                    //OLED��ʼ��

	while(1)
	{
	OLED_test();
//		CtrlAttiAng();
//	PWM_test();
//	COMMPC_TEST();	
//	Get_angle();
//	u2_printf("hell\n");
//	test_SendPC();//������PC����Э�����ݣ����ڵ���
	} 	
}

