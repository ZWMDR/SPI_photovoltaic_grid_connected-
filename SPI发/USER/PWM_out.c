#include "PWM_out.h"
#include "delay.h"

void PWM_init(u16 arr,u16 ps)
{
	TIM8_PWM_Init(arr,ps);
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare2(TIM8,0);
	TIM6_Int_Init(3600-1,0);//50Hz³õÊ¼ÆµÂÊ
	TIM_Cmd(TIM6,ENABLE);
}
void PWM_Set_duty(float rate)
{
	TIM_SetCompare2(TIM8,1800+Sin[t]*rate);
	/*
	if(Sin[t]>=0)
	{
		TIM_SetCompare2(TIM8,Sin[t]*rate);
		TIM_SetCompare1(TIM8,0);
	}
	else
	{
		TIM_SetCompare1(TIM8,-Sin[t]*rate);
		TIM_SetCompare2(TIM8,0);
	}
	*/
	t=(t+1)%400;
}
