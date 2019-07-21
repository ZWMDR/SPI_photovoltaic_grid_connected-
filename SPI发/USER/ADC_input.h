#ifndef __ADC_INPUT_H
#define __ADC_INPUT_H
#include "sys.h"
#include "stm32f10x_adc.h"
#include "stdlib.h"
#include "led.h"
#include "usart.h"

#define DMA_buff_len_ADC1 40
#define DMA_BUFF_LEN_ADC_2 100

extern u16 DMA_buff[DMA_buff_len_ADC1];
extern u16 DMA_buff_2[DMA_BUFF_LEN_ADC_2];
extern u8 ADC_flag;

typedef struct ADC_Channel
{
	uint8_t num;
	uint8_t ADC_Channel;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint8_t ADC_SampleTime;
	
	
}ADC_Channel;

typedef struct ADC_continuous_sampling
{
	u8 channel_num;
	ADC_Channel *Channels;
	u8 ScanConvMode;
	uint32_t RCC_APB2Periph_GPIOx;
	
	u16 arr;
	u16 psc;
	
}ADC_cs_InitTypeDef;

typedef struct ADC_single_sample
{
	ADC_TypeDef* ADCx;
	u8 channel_num;
	uint8_t ADC_Channel;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint8_t ADC_SampleTime;
	
}ADC_ss_InitTypeDef;

//ADC+DMA连续转换
void ADC1_continuous_sampling_Init(ADC_cs_InitTypeDef *ADC_cs,u16* arr,u16 buff_len);
void ADC1_continuous_sampling_enable(void);
void ADC1_continuous_sampling_disable(void);

void ADC3_continuous_sampling_Init(ADC_cs_InitTypeDef *ADC_cs,u16* arr,u16 buff_len);
void ADC3_continuous_sampling_enable(void);
void ADC3_continuous_sampling_disable(void);

void ADC_sampling_enable(ADC_TypeDef* ADCx);
void ADC_sampling_disable(ADC_TypeDef* ADCx);

//单次转换
void ADC_Single_Sample_Init(ADC_ss_InitTypeDef *ADC_ss,u8 interrupt_mode);
u16 get_ADC_value(ADC_ss_InitTypeDef *ADC_ss);


#endif
