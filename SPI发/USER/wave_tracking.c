#include "wave_tracking.h"

void PWM_input_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	//TIM_ICInitTypeDef TIM_ICInitStructure;
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//��ʼ��PWM���벶��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PWM���벶��Ĵ�����ʼ��
	
 	TIM4->ARR=0xFFFF-1;  		//�趨�������Զ���װֵ   
	TIM4->PSC=72-1;  		//Ԥ��Ƶ��
	
	TIM5->ARR=0xFFFF-1;  		//�趨�������Զ���װֵ   
	TIM5->PSC=72-1;  		//Ԥ��Ƶ��

	TIM4->CCMR1|=1<<0;		 //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4->CCMR1|=1<<9;		 //CC2S=10 	ѡ������� IC2ӳ�䵽TI1��
	TIM4->CCER|=0<<1;      //CC1P=0   ѡ������IC1Ϊ��������Ч
	TIM4->CCER|=1<<5;      //CC2P=1   ѡ������IC2Ϊ�½�����Ч
	TIM4->SMCR|=0x05<<4;   //TS=101   ѡ��TI1FP1
	TIM4->SMCR|=0x04<<0;   //SMS=100  ���ô�ģʽ������Ϊ��λģʽ
	TIM4->CCER|=1<<0; 		 //CC1E=1 	�������������ֵ������Ĵ�����
	TIM4->CCER|=1<<4; 		 //CC2E=1 	�������������ֵ������Ĵ�����
	
	TIM5->CCMR1|=1<<0;		 //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5->CCMR1|=1<<9;		 //CC2S=10 	ѡ������� IC2ӳ�䵽TI1��
	TIM5->CCER|=0<<1;      //CC1P=0   ѡ������IC1Ϊ��������Ч
	TIM5->CCER|=1<<5;      //CC2P=1   ѡ������IC2Ϊ�½�����Ч
	TIM5->SMCR|=0x05<<4;   //TS=101   ѡ��TI1FP1
	TIM5->SMCR|=0x04<<0;   //SMS=100  ���ô�ģʽ������Ϊ��λģʽ
	TIM5->CCER|=1<<0; 		 //CC1E=1 	�������������ֵ������Ĵ�����
	TIM5->CCER|=1<<4; 		 //CC2E=1 	�������������ֵ������Ĵ�����


	TIM4->DIER|=1<<1;     //CC1IE=1   ������/�Ƚ�1�ж�
	MY_NVIC_Init(1,0,TIM4_IRQn,2);//��ռ1�������ȼ�0����2
	
	TIM5->DIER|=1<<1;   	//CC1IE=1   ������/�Ƚ�1�ж�
	MY_NVIC_Init(1,0,TIM5_IRQn,2);//��ռ1�������ȼ�0����2
	
	//��ʱ������
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��
	TIM3->ARR=0xFFFF;  	//�趨�������Զ���װֵ 
	TIM3->PSC=72-1;  	//Ԥ��Ƶ������
	//TIM3->DIER|=1<<0;   //��������ж�
	
	TIM3->CNT=0x0000;
	TIM4->CNT=0x0000;
	TIM5->CNT=0x0000;
	
	//TIM3->CR1|=0x01;      //ʹ�ܶ�ʱ��3
	TIM4->CR1|=0x01;    	//ʹ�ܶ�ʱ��4
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��5
	
	flag_REF=0;
	flag_F=0;
	Period_REF=0;
	Period_F=0;
	Frequency_REF=0;
	Frequency_F=0;
}
