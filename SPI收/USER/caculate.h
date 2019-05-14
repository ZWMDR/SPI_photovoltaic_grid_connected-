#ifndef __CACULATE_H
#define __CACULATE_H
#include "spi.h"

extern u8 recv_flag;
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];
extern float frequency;

void resend(void);

#endif
