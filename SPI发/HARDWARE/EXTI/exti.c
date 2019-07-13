#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "SPWM_output.h"

//外部中断初始化函数


void TIM5_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1)==SET)
	{
		t=4;
		IC1Value = TIM5->CCR1+1;
		if(IC1Value != 0)
		{
			Frequency_REF = IC1Value;
			SPI_send_buff[1]=Frequency_REF;
			flag_REF=1;
			LED1=~LED1;

			TIM_SetCounter(TIM7,0);
			TIM_Cmd(TIM7,ENABLE);
		}
		
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
	}
}

void TIM4_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)==SET)
	{
		IC1Value = TIM4->CCR1+1;
		if(IC1Value != 0)
		{
			Frequency_F = IC1Value;
			SPI_send_buff[2]=Frequency_F;
			flag_F=1;

			TIM_Cmd(TIM7,DISABLE);
			Period_F=TIM_GetCounter(TIM7);
			capture_flag=1;
			SPI_send_buff[5]=Period_F;
		}
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	}
}





