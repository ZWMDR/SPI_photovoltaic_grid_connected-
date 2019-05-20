#ifndef __GUI_H
#define __GUI_H
#include "lcd.h"
#include "spi.h"
#include "caculate.h"

#define GUID_LINE1 63
#define GUID_LINE2 35
#define GUID_LINE3 8

#define GUID_LINE4 108
#define GUID_LINE5 135
#define GUID_LINE6 163

extern u8 recv_flag;
extern u8 ADC_flag;
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];
extern float frequency_REF;
extern float frequency_F;
extern float phase;
extern float VCC;

void info_init(void);
void graph_init(void);
void info_show(void);
void graph_show(void);

#endif
