#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"

#define DMA_buff_len 120

#define _5kHz 5000
#define _2k5Hz 2500

extern u16 DMA_buff[DMA_buff_len];


void ADC1_TIM_Init(int frequency);
void ADC_get(int count);

#endif 
