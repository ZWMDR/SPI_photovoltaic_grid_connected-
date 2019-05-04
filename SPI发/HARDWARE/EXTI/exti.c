#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//外部中断初始化函数



void TIM5_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1)==SET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
		IC1Value = TIM_GetCapture1(TIM5)+1;
		//if((IC1Value != 0) && (!(flag_REF && flag_F)))
		if(IC1Value != 0)
		{
			Frequency_REF = IC1Value;
			flag_REF=1;
			if(!flag_F)
			{
				Period_REF=0;
				TIM_Cmd(TIM3,ENABLE);
			}
			else
			{
				TIM_Cmd(TIM3,DISABLE);
				Period_REF=TIM_GetCounter(TIM3);
				TIM_SetCounter(TIM3,0);
				
				DMA1_Channel3->CNDTR=4;
				DMA_buff_TX[0]=Frequency_REF;
				DMA_buff_TX[1]=Frequency_F;
				DMA_buff_TX[2]=Period_REF;
				DMA_buff_TX[3]=Period_F;
				DMA_Cmd(DMA1_Channel3, ENABLE);
				//printf("F_REF=%d, F_F=%d\r\n",Frequency_REF,Frequency_F);
				//printf("P_REF=%d, P_F=%d\r\n",Period_REF,Period_F);
			}
		}
	}
}

void TIM4_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
		IC1Value = TIM_GetCapture1(TIM4)+1;
		//if((IC1Value != 0) && (!(flag_REF && flag_F)))
		if(IC1Value != 0)
		{
			Frequency_F = IC1Value;
			flag_F=1;
			if(!flag_REF)
			{
				Period_F=0;
				TIM_Cmd(TIM3,ENABLE);
			}
			else
			{
				TIM_Cmd(TIM3,DISABLE);
				Period_F=TIM_GetCounter(TIM3);
				TIM_SetCounter(TIM3,0);
				
				DMA1_Channel3->CNDTR=4;
				DMA_buff_TX[0]=Frequency_REF;
				DMA_buff_TX[1]=Frequency_F;
				DMA_buff_TX[2]=Period_REF;
				DMA_buff_TX[3]=Period_F;
				DMA_Cmd(DMA1_Channel3, ENABLE);
				//printf("F_REF=%d, F_F=%d\r\n",Frequency_REF,Frequency_F);
				//printf("P_REF=%d, P_F=%d\r\n",Period_REF,Period_F);
			}
		}
	}
}

void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		PWM_Set_duty();
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

void DMA1_Channel3_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		flag_F=flag_REF=0;
		LED1=~LED1;
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC2)!=RESET)
	{
		
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}
