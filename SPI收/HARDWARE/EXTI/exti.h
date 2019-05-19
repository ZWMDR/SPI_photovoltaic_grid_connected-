#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "spi.h"
#include "usart.h"
#include "adc.h"
#include "caculate.h"

extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u8 recv_flag;
extern u8 ADC_flag;
extern float frequency_REF;
extern float frequency_F;
extern float phase;
extern float VCC;

void EXTIX_Init(void);//IO≥ı ºªØ
void DMA_EXTI_Init(void);

#endif

