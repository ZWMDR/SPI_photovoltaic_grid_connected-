#include "PWM_input_capture.h"

void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{ 
	u32 temp,temp1;
	temp1=(~NVIC_Group)&0x07;
	temp1<<=8;
	temp=SCB->AIRCR;
	temp&=0X0000F8FF;
	temp|=0X05FA0000;
	temp|=temp1;
	SCB->AIRCR=temp;    	  				   
}

void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//取低四位
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//使能中断位
	NVIC->IP[NVIC_Channel]|=temp<<4;		//设置响应优先级和抢断优先级
}



void PWM_input_capture_Init(PWM_capture_InitTypeDef *init_typedef1,PWM_capture_InitTypeDef *init_typedef2,uint16_t arr,uint16_t psc)
//参数：定时器1，定时器2，预装载值，分频系数，PWM信号输入端口GPIO1，GPIO2，信号输入管脚Pin1，Pin2
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitTypeDef GPIO_InitStructure2;
	
	//初始化端口
	RCC_APB2PeriphClockCmd(init_typedef1->RCC_APB2Periph_GPIOx,ENABLE);
	RCC_APB2PeriphClockCmd(init_typedef2->RCC_APB2Periph_GPIOx,ENABLE);
	
	GPIO_InitStructure1.GPIO_Pin = init_typedef1->Pin;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure2.GPIO_Pin = init_typedef2->Pin;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(init_typedef1->GPIOx,&GPIO_InitStructure1);
	GPIO_Init(init_typedef2->GPIOx,&GPIO_InitStructure2);
	
	//初始化定时器
	RCC_APB1PeriphClockCmd(init_typedef1->RCC_APB1Periph_TIMx,ENABLE);
	RCC_APB1PeriphClockCmd(init_typedef2->RCC_APB1Periph_TIMx,ENABLE);
	
	//PWM输入捕获寄存器初始化
	init_typedef1->TIMx->ARR=arr;
	init_typedef1->TIMx->PSC=psc;
	init_typedef2->TIMx->ARR=arr;
	init_typedef2->TIMx->PSC=psc;
	
	init_typedef1->TIMx->CCMR1|=1<<0;		 //CC1S=01 	选择输入端IC1映射到TI1上
	init_typedef1->TIMx->CCMR1|=1<<9;		 //CC2S=10 	选择输入端IC2映射到TI1上
	init_typedef1->TIMx->CCER|=0<<1;      //CC1P=0   选择IC1为上升沿有效
	init_typedef1->TIMx->CCER|=1<<5;      //CC2P=1   选择IC2为下降沿有效
	init_typedef1->TIMx->SMCR|=0x05<<4;   //TS=101   选择TI1FP1
	init_typedef1->TIMx->SMCR|=0x04<<0;   //SMS=100  配置从模式控制器为复位模式
	init_typedef1->TIMx->CCER|=1<<0; 		 //CC1E=1 	允许TI1捕获计数器的值到捕获寄存器中
	init_typedef1->TIMx->CCER|=1<<4; 		 //CC2E=1 	允许TI2捕获计数器的值到捕获寄存器中
	
	init_typedef2->TIMx->CCMR1|=1<<0;		 //CC1S=01 	选择输入端IC1映射到TI1上
	init_typedef2->TIMx->CCMR1|=1<<9;		 //CC2S=10 	选择输入端IC2映射到TI1上
	init_typedef2->TIMx->CCER|=0<<1;      //CC1P=0   选择IC1为上升沿有效
	init_typedef2->TIMx->CCER|=1<<5;      //CC2P=1   选择IC2为下降沿有效
	init_typedef2->TIMx->SMCR|=0x05<<4;   //TS=101   选择TI1FP1
	init_typedef2->TIMx->SMCR|=0x04<<0;   //SMS=100  配置从模式控制器为复位模式
	init_typedef2->TIMx->CCER|=1<<0; 		 //CC1E=1 	允许TI1捕获计数器的值到捕获寄存器中
	init_typedef2->TIMx->CCER|=1<<4; 		 //CC2E=1 	允许TI2捕获计数器的值到捕获寄存器中

	init_typedef1->TIMx->DIER|=1<<1;     //CC1IE=1
	
	init_typedef2->TIMx->DIER|=1<<1;   	//CC1IE=1¶Ï

	//相位差计数定时器初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	TIM7->ARR=0xFFFF;  	//预装载值
	TIM7->PSC=psc;  	//预分频系数与捕获定时器相同
	//TIM7->DIER|=1<<0;   //允许更新中断
	
	//CNT计数值清零
	TIM7->CNT=0x0000;
	init_typedef1->TIMx->CNT=0x0000;
	init_typedef2->TIMx->CNT=0x0000;
	
	//TIM7->CR1|=0x01;      //使能TIM3
	init_typedef1->TIMx->CR1|=0x01;    	//使能TIMx_1
	init_typedef2->TIMx->CR1|=0x01;    	//使能TIMx_2
}

