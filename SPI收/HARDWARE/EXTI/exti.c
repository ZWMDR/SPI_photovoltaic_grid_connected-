#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//外部中断初始化函数


void DMA1_Channel3_IRQHandler(void)//SPI发送完成中断
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		LED0=~LED0;
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)//SPI接收中断
{
	if ((DMA_GetITStatus(DMA1_IT_TC2)!=RESET) && (!recv_flag))
	{
		Frequency_REF=DMA_buff_RX[0];
		Frequency_F=DMA_buff_RX[1];
		Period_REF=DMA_buff_RX[2];
		Period_F=DMA_buff_RX[3];
		
		printf("REF:%d,F:%d\r\n",Frequency_REF,Frequency_F);
		
		if(Frequency_REF==0xFFFF && Frequency_F==0xFFFF)
		{
			DMA_buff_TX[0]=DMA_buff_TX[1]=DMA_buff_TX[2]=DMA_buff_TX[3]=0;
			frequency_REF=frequency_F=0;
		}
		else
		{
			//DMA_buff_TX[0]=get_frequency(&frequency_REF,&frequency_F);
			DMA_buff_TX[1]=get_phase(&phase);
			DMA_buff_TX[2]=get_VCC_coef();
			DMA_buff_TX[3]=0;
		}
		
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA1_Channel2->CNDTR=4;
		DMA_Cmd(DMA1_Channel2,ENABLE);
		
		DMA1_Channel3->CNDTR=4;
		DMA_Cmd(DMA1_Channel3,ENABLE);
		
		DMA_Cmd(DMA1_Channel1,ENABLE);
		TIM_Cmd(TIM2,ENABLE);
		
		recv_flag=1;
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void DMA1_Channel1_IRQHandler(void)//ADC1中断
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		LED0=~LED0;
		ADC_flag=1;
		ADC_show_flag=1;
		
		vcc_REF=get_vcc(DMA_buff,DMA_buff_len,0,&VCC_REF)*0.00080566*REF_GAIN;//REF的VCC具体数值
		vcc_F=get_vcc(DMA_buff,DMA_buff_len,1,&VCC_F)*0.00080566*F_GAIN;//F的VCC具体数值
		
		DMA_Cmd(DMA1_Channel1,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}
