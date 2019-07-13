#ifndef __EXTI_H

#define __EXIT_H	 
#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "usart.h"
#include "NRF24L01_module.h"

extern u8 DMA_SPI_buff_RX[DMA_SPI_buff_len];
extern u8 DMA_SPI_buff_TX[DMA_SPI_buff_len];
extern u16 SPI_send_buff[SPI_send_buff_len];
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u8 flag_REF;
extern u8 flag_F;
extern u8 send_flag;
extern u8 capture_flag;
extern u8 ADC_flag;
extern u16 t;
extern float rate;


void EXTIX_Init(void);//IO��ʼ��
void DMA_EXTI_Init(void);

#endif
