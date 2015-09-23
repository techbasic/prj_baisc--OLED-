#include "pwm.h"

void Motor_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OC1InitStruct;
	TIM_OCInitTypeDef TIM_OC2InitStruct;
	TIM_OCInitTypeDef TIM_OC3InitStruct;
	TIM_OCInitTypeDef TIM_OC4InitStruct;	
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_OC1InitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OC1InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1InitStruct.TIM_Pulse=0;
	TIM_OC1InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OC1InitStruct);
	
	TIM_OC2InitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OC2InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2InitStruct.TIM_Pulse=0;
	TIM_OC2InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM3,&TIM_OC2InitStruct);
	
	TIM_OC3InitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OC3InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC3InitStruct.TIM_Pulse=0;
	TIM_OC3InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC3Init(TIM3,&TIM_OC3InitStruct);
	
	TIM_OC4InitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OC4InitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC4InitStruct.TIM_Pulse=0;
	TIM_OC4InitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC4Init(TIM3,&TIM_OC4InitStruct);
	
	TIM_CtrlPWMOutputs(TIM3,ENABLE);


	TIM_ARRPreloadConfig(TIM3,ENABLE);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	
	TIM_Cmd(TIM3,ENABLE);
}

#ifdef DEBUG_PWM
void PWM_test(void)
{
	u16 MOTO1_PWM,MOTO2_PWM,MOTO3_PWM,MOTO4_PWM;
	MOTO1_PWM=499;
	MOTO2_PWM=499;
	MOTO3_PWM=499;
	MOTO4_PWM=499;
	TIM_SetCompare1(TIM3,MOTO1_PWM);	 
	TIM_SetCompare2(TIM3,MOTO2_PWM);	
	TIM_SetCompare3(TIM3,MOTO3_PWM);	
	TIM_SetCompare4(TIM3,MOTO4_PWM);	
}
#endif


