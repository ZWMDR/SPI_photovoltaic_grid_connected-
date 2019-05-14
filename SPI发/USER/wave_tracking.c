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
/*
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM5, &TIM_ICInitStructure);
	TIM_SelectInputTrigger(TIM5, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
	TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
 	
 	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
	
	TIM3_Int_Init(0xFFFF,72-1); // ��λ1us�ֱ���
	
	TIM_SetCounter(TIM3,0);
	TIM_SetCounter(TIM4,0);
	TIM_SetCounter(TIM5,0);
 	//TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);
 	//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);
	
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
	*/
	
	flag_REF=0;
	flag_F=0;
	Period_REF=0;
	Period_F=0;
	Frequency_REF=0;
	Frequency_F=0;
}
