#include "ADC_input.h"


void ADC_continuous_sampling_Init(ADC_cs_InitTypeDef *ADC_cs,u16 frequency)
{
	int i;
	ADC_InitTypeDef          ADC_InitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;
	DMA_InitTypeDef          DMA_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	TIM_TimeBaseStructure.TIM_Period = (frequency==_5kHz)?199:399;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //72M/72=1 MHz, 1MHz/200=5kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (frequency==_5kHz)?59:159;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_InternalClockConfig(TIM2);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_UpdateDisableConfig(TIM2, DISABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)DMA_buff;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize=DMA_buff_len_ADC;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=(ADC_cs->ScanConvMode)?ENABLE:DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=ADC_cs->channel_num;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	for(i=0;i<ADC_cs->channel_num;i++)
	{
		if(i==0)
			GPIO_InitStructure.GPIO_Pin=ADC_cs->Channels[i].Pin;
		else
			GPIO_InitStructure.GPIO_Pin|=ADC_cs->Channels[i].Pin;
		
		ADC_RegularChannelConfig(ADC1,ADC_cs->Channels[i].ADC_Channel,ADC_cs->Channels[i].num,ADC_cs->Channels[i].ADC_SampleTime);
	}
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC校准
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)== SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
	TIM_Cmd(TIM2,DISABLE);
}

void ADC_Single_Sample_Init(ADC_ss_InitTypeDef *ADC_ss,u8 interrupt_mode)
{
	ADC_InitTypeDef    ADC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=ADC_ss->Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(ADC_ss->GPIOx, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC_ss->ADCx,&ADC_InitStructure);
	
	ADC_Cmd(ADC_ss->ADCx, ENABLE);
	ADC_ResetCalibration(ADC_ss->ADCx);
	while(ADC_GetResetCalibrationStatus(ADC_ss->ADCx));
	ADC_StartCalibration(ADC_ss->ADCx);
	while(ADC_GetCalibrationStatus(ADC_ss->ADCx));
}

u16 get_ADC_value(ADC_ss_InitTypeDef *ADC_ss)
{
	ADC_RegularChannelConfig(ADC_ss->ADCx,ADC_ss->ADC_Channel,1,ADC_ss->ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC_ss->ADCx,ENABLE);
	while(!ADC_GetFlagStatus(ADC_ss->ADCx,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC_ss->ADCx);
}

void DMA1_Channel1_IRQHandler(void)//ADC1中断
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		LED0=~LED0;
		ADC_flag=1;
		
		DMA_Cmd(DMA1_Channel1,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}

void ADC_continuous_sampling_enable(void)
{
	ADC_Cmd(ADC1,ENABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
void ADC_continuous_sampling_disable(void)
{
	ADC_Cmd(ADC1,DISABLE);
	DMA_Cmd(DMA1_Channel1,DISABLE);
	TIM_Cmd(TIM2,DISABLE);
}

void ADC_sampling_enable(ADC_TypeDef* ADCx)
{
	ADC_Cmd(ADCx,ENABLE);
}
void ADC_sampling_disable(ADC_TypeDef* ADCx)
{
	ADC_Cmd(ADCx,DISABLE);
}

