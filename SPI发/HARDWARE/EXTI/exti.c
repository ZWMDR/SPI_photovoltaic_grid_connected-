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
		//LED0=~LED0;
		t=0;
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
		IC1Value = TIM5->CCR1+1;
		if(IC1Value != 0)
		{
			Frequency_REF = IC1Value;
			flag_REF=1;
			if(!flag_F)
			{
				Period_REF=0;
				TIM_SetCounter(TIM3,0);
				TIM_Cmd(TIM3,ENABLE);
			}
			else
			{
				TIM_Cmd(TIM3,DISABLE);
				Period_REF=TIM_GetCounter(TIM3);
				
				if(!capture_flag)
				{
					DMA_SPI_buff_TX[0]=Frequency_REF;
					DMA_SPI_buff_TX[1]=Frequency_F;
					DMA_SPI_buff_TX[2]=Period_REF;
					DMA_SPI_buff_TX[3]=Period_F;
					
					capture_flag=1;
				}
				flag_F=flag_REF=0;
			}
		}
	}
}

void TIM4_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)==SET)
	{
		//LED1=~LED1;
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
		IC1Value = TIM4->CCR1+1;
		if(IC1Value != 0)
		{
			Frequency_F = IC1Value;
			flag_F=1;
			if(!flag_REF)
			{
				Period_F=0;
				TIM_SetCounter(TIM3,0);
				TIM_Cmd(TIM3,ENABLE);
			}
			else
			{
				TIM_Cmd(TIM3,DISABLE);
				Period_F=TIM_GetCounter(TIM3);
				if((!capture_flag) && (!send_flag))
				{
					DMA_SPI_buff_TX[0]=Frequency_REF;
					DMA_SPI_buff_TX[1]=Frequency_F;
					DMA_SPI_buff_TX[2]=Period_REF;
					DMA_SPI_buff_TX[3]=Period_F;
					
					capture_flag=1;
				}
				flag_F=flag_REF=0;
			}
		}
	}
}





