#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"

#define DMA_buff_len 200

#define _5kHz 5000
#define _2k5Hz 2500

#define REF_GAIN 1
#define F_GAIN 1

extern u16 DMA_buff[DMA_buff_len];


void ADC1_TIM_Init(u16 frequency);
void ADC_get(u16 count);

#endif 
