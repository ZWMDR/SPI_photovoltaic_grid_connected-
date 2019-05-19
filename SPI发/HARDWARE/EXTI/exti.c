#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//�ⲿ�жϳ�ʼ������

void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{ 
	u32 temp,temp1;
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}

void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	NVIC->IP[NVIC_Channel]|=temp<<4;		//������Ӧ���ȼ����������ȼ�
}

void TIM5_IRQHandler(void)
{
	u16 IC1Value;
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1)==SET)
	{
		LED0=~LED0;
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
		IC1Value = TIM5->CCR1;
		//if((IC1Value != 0) && (!(flag_REF && flag_F)))
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
				
				if(!send_flag)
				{
					DMA_buff_TX[0]=Frequency_REF;
					DMA_buff_TX[1]=Frequency_F;
					DMA_buff_TX[2]=Period_REF;
					DMA_buff_TX[3]=Period_F;
					
					send_flag=1;
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
		LED1=~LED1;
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
		IC1Value = TIM4->CCR1;
		//if((IC1Value != 0) && (!(flag_REF && flag_F)))
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
				if(!send_flag)
				{
					DMA_buff_TX[0]=Frequency_REF;
					DMA_buff_TX[1]=Frequency_F;
					DMA_buff_TX[2]=Period_REF;
					DMA_buff_TX[3]=Period_F;
					
					send_flag=1;
				}
				flag_F=flag_REF=0;
			}
		}
	}
}

void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		PWM_Set_duty();
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}

void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}

void DMA1_Channel3_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		send_flag=0;
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC2)!=RESET)
	{
		//LED1=~LED1;

		if(DMA_buff_RX[0]<5000 && DMA_buff_RX[0]>2000)
		{
			TIM6->ARR=DMA_buff_RX[0];
			//TIM_Cmd(TIM6,ENABLE);
		}
		
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA1_Channel2->CNDTR=4;
		DMA_Cmd(DMA1_Channel2,ENABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		DMA1_Channel3->CNDTR=4;
		DMA_Cmd(DMA1_Channel3, ENABLE);
		//printf("F_REF=%d, F_F=%d\r\n",DMA_buff_TX[0],DMA_buff_TX[1]);
		//printf("P_REF=%d, P_F=%d\r\n\r\n",DMA_buff_TX[2],DMA_buff_TX[3]);
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}
