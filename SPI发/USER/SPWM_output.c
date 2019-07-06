#include "SPWM_output.h"

void TIM6_Init(u16 arr,u16 psc);

void SPWM_output_Init(SPWM_InitTypeDef *spwm_init_typedef,uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(spwm_init_typedef->RCC_APB2Periph_TIMx,ENABLE);
 	RCC_APB2PeriphClockCmd(spwm_init_typedef->RCC_APB2Periph_GPIOx,ENABLE);  //使能GPIO外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = spwm_init_typedef->Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(spwm_init_typedef->GPIOx, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(spwm_init_typedef->TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_DeadTime = 20;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;               
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(spwm_init_typedef->TIMx, &TIM_BDTRInitStructure);
	
	TIM_OC1Init(spwm_init_typedef->TIMx, &TIM_OCInitStructure);
	
	TIM_OC2Init(spwm_init_typedef->TIMx, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(spwm_init_typedef->TIMx,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(spwm_init_typedef->TIMx, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(spwm_init_typedef->TIMx, TIM_OCPreload_Enable);  //CH1预装载使能	 	
	
	TIM_ARRPreloadConfig(spwm_init_typedef->TIMx, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM6_Init(3600-1,0);//50Hz初始频率
	TIM_Cmd(TIM6,ENABLE);
	
	TIM_Cmd(spwm_init_typedef->TIMx, ENABLE);  //使能TIMx
	
	TIM_SetCompare1(spwm_init_typedef->TIMx,0);
	TIM_SetCompare2(spwm_init_typedef->TIMx,0);
}

void TIM6_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period =arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ARRPreloadConfig(TIM6,ENABLE);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		PWM_Set_duty(voltage_scale_rate,&t,2);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

void PWM_Set_duty(float rate,u16* t,u8 mode)
{
	static u8 status=0;
	if(mode==0)//单极性输出
	{
		TIM_SetCompare2(TIM8,1799+Sin[*t]*rate);
		if(Sin[*t]>=0)
			TIM_SetCompare1(TIM8,0);
		else
			TIM_SetCompare1(TIM8,3600);
	}
	else if(mode==1)//单极性，磨平衡
	{
		if(status==0)
		{
			TIM_SetCompare2(TIM8,1799+Sin[*t]*rate);
			if(Sin[*t]>=0)
				TIM_SetCompare1(TIM8,0);
			else
				TIM_SetCompare1(TIM8,3600);
			if(*t==SINTABLE_LEN-1)
				status=1;
		}
		else
		{
			TIM_SetCompare1(TIM8,1799+Sin[*t]*rate);
			if(Sin[*t]>=0)
				TIM_SetCompare2(TIM8,0);
			else
				TIM_SetCompare2(TIM8,3600);
			if(*t==SINTABLE_LEN-1)
				status=0;
		}
	}
	else//双极性输出
	{
		TIM_SetCompare1(TIM8,0);
		TIM_SetCompare2(TIM8,1799+Sin[*t]*rate);
	}
	*t=((*t)+1)%SINTABLE_LEN;
}

