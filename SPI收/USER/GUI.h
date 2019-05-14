#ifndef __GUI_H
#define __GUI_H
#include "lcd.h"
#include "spi.h"
#include "caculate.h"

extern u8 recv_flag;
extern u8 ADC_flag;
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];

void info_init(void);
void info_show(void);




#endif
