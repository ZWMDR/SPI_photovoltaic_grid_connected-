#ifndef __EXTI_H

#define __EXIT_H	 
#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "SPI_DMA.h"
#include "usart.h"

extern u16 DMA_SPI_buff_RX[DMA_SPI_buff_len];
extern u16 DMA_SPI_buff_TX[DMA_SPI_buff_len];
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u8 flag_REF;
extern u8 flag_F;
extern u8 send_flag;
extern u8 ADC_flag;
extern u16 t;
extern float rate;


void EXTIX_Init(void);//IO≥ı ºªØ
void DMA_EXTI_Init(void);

#endif
