#include "delay.h" 			 
#include "usart.h" 			 
#include "usart2.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"


//��ʼ��ATK-HC05ģ��
//����ֵ:0,�ɹ�;1,ʧ��.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��	 	
	RCC->APB2ENR|=1<<4;    	//ʹ��PORTCʱ��	 	
 	GPIOA->CRL&=0XFFF0FFFF;	//PA4,����
 	GPIOA->CRL|=0X00080000; 
	GPIOA->ODR|=1<<4; 		//PA4���� 
	GPIOC->CRL&=0XFFF0FFFF;	//PC4,�������
	GPIOC->CRL|=0X00030000; 
	GPIOC->ODR|=1<<4; 		//PC4���1	
	USART2_Init(36,115200);	//��ʼ������2Ϊ:9600,������.
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u2_printf("AT\r\n");		//����AT����ָ��
		
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<10;t++) 			//��ȴ�50ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART2_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART2_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			USART2_RX_STA=0;			 
			if(temp==4&&USART2_RX_BUF[0]=='O'&&USART2_RX_BUF[1]=='K')
			{
				temp=0;//���յ�OK��Ӧ
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//���ʧ��
	return temp;	 
}	 
//��ȡATK-HC05ģ��Ľ�ɫ
//����ֵ:0,�ӻ�;1,����;0XFF,��ȡʧ��.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u2_printf("AT+ROLE?\r\n");	//��ѯ��ɫ
		for(t=0;t<20;t++) 			//��ȴ�200ms,������HC05ģ��Ļ�Ӧ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		if(USART2_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			USART2_RX_STA=0;			 
			if(temp==13&&USART2_RX_BUF[0]=='+')//���յ���ȷ��Ӧ����
			{
				temp=USART2_RX_BUF[6]-'0';//�õ�����ģʽֵ
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//��ѯʧ��.
	return temp;
} 							   
//ATK-HC05��������
//�˺�����������ATK-HC05,�����ڽ�����OKӦ���ATָ��
//atstr:ATָ�.����:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"���ַ���
//����ֵ:0,���óɹ�;����,����ʧ��.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u2_printf("%s\r\n",atstr);	//����AT�ַ���
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<20;t++) 			//��ȴ�100ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART2_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART2_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			USART2_RX_STA=0;			 
			if(temp==4&&USART2_RX_BUF[0]=='O')//���յ���ȷ��Ӧ����
			{			
				temp=0;
				break;			 
			}
		}		
	}
	if(retry==0)temp=0XFF;//����ʧ��.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//ͨ���ú���,��������USMART,���Խ��ڴ���2�ϵ�ATK-HC05ģ��
//str:���.(����ע�ⲻ����Ҫ������س���)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEY�ø�,����ATģʽ
	delay_ms(10);
	u2_printf("%s\r\n",(char*)str); //����ָ��
	for(t=0;t<50;t++) 				//��ȴ�500ms,������HC05ģ��Ļ�Ӧ
	{
		if(USART2_RX_STA&0X8000)break;
		delay_ms(10);
	}									    
	HC05_KEY=0;						//KEY����,�˳�ATģʽ
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{
		temp=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
		USART2_RX_STA=0;
		USART2_RX_BUF[temp]=0;		//�ӽ�����		 
		printf("\r\n%s",USART2_RX_BUF);//���ͻ�Ӧ���ݵ�����1
	} 				 
}














