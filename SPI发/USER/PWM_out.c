#include "PWM_out.h"
#include "delay.h"

void PWM_init(u16 arr,u16 ps)
{
	TIM8_PWM_Init(arr,ps);
	delay_ms(1);
	TIM1_PWM_Init(arr,ps);
	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare1(TIM1,0);
	TIM6_Int_Init(3600-1,0);//50Hz³õÊ¼ÆµÂÊ
	TIM_Cmd(TIM6,ENABLE);
}
void PWM_Set_duty(void)
{
	TIM_SetCompare1(TIM8,Sin[t]);
	TIM_SetCompare1(TIM1,Sin[t]);
	t++;
	if(t==400)
		t=0;
}
