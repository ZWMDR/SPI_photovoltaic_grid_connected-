#include "wave_tracking.h"

void PWM_input_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	//TIM_ICInitTypeDef TIM_ICInitStructure;
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//初始化PWM输入捕获
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

	//PWM输入捕获寄存器初始化
	
 	TIM4->ARR=0xFFFF-1;  		//设定计数器自动重装值   
	TIM4->PSC=72-1;  		//预分频器
	
	TIM5->ARR=0xFFFF-1;  		//设定计数器自动重装值   
	TIM5->PSC=72-1;  		//预分频器

	TIM4->CCMR1|=1<<0;		 //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM4->CCMR1|=1<<9;		 //CC2S=10 	选择输入端 IC2映射到TI1上
	TIM4->CCER|=0<<1;      //CC1P=0   选择输入IC1为上升沿有效
	TIM4->CCER|=1<<5;      //CC2P=1   选择输入IC2为下降沿有效
	TIM4->SMCR|=0x05<<4;   //TS=101   选择TI1FP1
	TIM4->SMCR|=0x04<<0;   //SMS=100  配置从模式控制器为复位模式
	TIM4->CCER|=1<<0; 		 //CC1E=1 	允许捕获计数器的值到捕获寄存器中
	TIM4->CCER|=1<<4; 		 //CC2E=1 	允许捕获计数器的值到捕获寄存器中
	
	TIM5->CCMR1|=1<<0;		 //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5->CCMR1|=1<<9;		 //CC2S=10 	选择输入端 IC2映射到TI1上
	TIM5->CCER|=0<<1;      //CC1P=0   选择输入IC1为上升沿有效
	TIM5->CCER|=1<<5;      //CC2P=1   选择输入IC2为下降沿有效
	TIM5->SMCR|=0x05<<4;   //TS=101   选择TI1FP1
	TIM5->SMCR|=0x04<<0;   //SMS=100  配置从模式控制器为复位模式
	TIM5->CCER|=1<<0; 		 //CC1E=1 	允许捕获计数器的值到捕获寄存器中
	TIM5->CCER|=1<<4; 		 //CC2E=1 	允许捕获计数器的值到捕获寄存器中


	TIM4->DIER|=1<<1;     //CC1IE=1   允许捕获/比较1中断
	MY_NVIC_Init(1,0,TIM4_IRQn,2);//抢占1，子优先级0，组2
	
	TIM5->DIER|=1<<1;   	//CC1IE=1   允许捕获/比较1中断
	MY_NVIC_Init(1,0,TIM5_IRQn,2);//抢占1，子优先级0，组2
	
	//定时器配置
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能
	TIM3->ARR=0xFFFF;  	//设定计数器自动重装值 
	TIM3->PSC=72-1;  	//预分频器设置
	//TIM3->DIER|=1<<0;   //允许更新中断
	
	TIM3->CNT=0x0000;
	TIM4->CNT=0x0000;
	TIM5->CNT=0x0000;
	
	//TIM3->CR1|=0x01;      //使能定时器3
	TIM4->CR1|=0x01;    	//使能定时器4
	TIM5->CR1|=0x01;    	//使能定时器5
	
	flag_REF=0;
	flag_F=0;
	Period_REF=0;
	Period_F=0;
	Frequency_REF=0;
	Frequency_F=0;
}
