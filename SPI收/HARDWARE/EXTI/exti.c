#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//外部中断初始化函数

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
		LED0=~LED0;
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	if ((DMA_GetITStatus(DMA1_IT_TC2)!=RESET) && (!recv_flag))
	{
		Frequency_REF=DMA_buff_RX[0];
		Frequency_F=DMA_buff_RX[1];
		Period_REF=DMA_buff_RX[2];
		Period_REF=DMA_buff_RX[3];
		LED1=~LED1;
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA1_Channel2->CNDTR=4;
		DMA_Cmd(DMA1_Channel2,ENABLE);
		
		recv_flag++;
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

