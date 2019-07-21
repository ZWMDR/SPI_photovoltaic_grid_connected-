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
		t=3;
		IC1Value = TIM5->CCR1-2;
		if(IC1Value != 0)
		{
			Frequency_REF = IC1Value;
			flag_REF=1;

			TIM_Cmd(TIM7,DISABLE);
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
		IC1Value = TIM4->CCR1-2;
		if(IC1Value != 0)
		{
			Frequency_F = IC1Value;
			flag_F=1;

			TIM_Cmd(TIM7,DISABLE);
			Period_F=TIM_GetCounter(TIM7);
			capture_flag=1;
			SPI_send_buff[5]=Period_F;
		}
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	}
}

void TIM1_UP_IRQHandler(void)
{
	u8 i;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		NRF_mode=1;
		SPI_send_buff[0]=0x0A0A;
		//SPI_send_buff[1]=Frequency_REF;
		SPI_send_buff[2]=Frequency_F;
		//printf("before: %x %d %d\r\n",SPI_send_buff[0],Frequency_REF,Frequency_F);
		NRF24L01_TX_Mode();
		//LED0=~LED0;
		for(i=0;i<SPI_send_buff_len;i++)//高位在前，低位在后
		{
			DMA_SPI_buff_TX[2*i]=(SPI_send_buff[i]>>8)&0x00FF;
			DMA_SPI_buff_TX[2*i+1]=SPI_send_buff[i]&0x00FF;
		}
		NRF24L01_TxPacket(DMA_SPI_buff_TX);
		NRF24L01_RX_Mode();
		NRF_mode=0;
		
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}



