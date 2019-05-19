#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//�ⲿ�жϳ�ʼ������

void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}

void DMA1_Channel3_IRQHandler(void)//SPI��������ж�
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		LED0=~LED0;
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)//SPI�����ж�
{
	if ((DMA_GetITStatus(DMA1_IT_TC2)!=RESET) && (!recv_flag))
	{
		DMA_buff_TX[0]=get_frequency(&frequency_REF,&frequency_F);
		DMA_buff_TX[1]=get_phase(&phase);
		DMA_buff_TX[2]=get_VCC_coef(&VCC);
		DMA_buff_TX[3]=0;
		
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA1_Channel2->CNDTR=4;
		DMA_Cmd(DMA1_Channel2,ENABLE);
		
		DMA1_Channel3->CNDTR=4;
		DMA_Cmd(DMA1_Channel3,ENABLE);
		
		recv_flag=1;
		
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void DMA1_Channel1_IRQHandler(void)//ADC1�ж�
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		LED0=1;
		ADC_flag=1;
		DMA_Cmd(DMA1_Channel1,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}
