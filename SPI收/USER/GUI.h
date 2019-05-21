#ifndef __GUI_H
#define __GUI_H
#include "lcd.h"
#include "spi.h"
#include "caculate.h"

#define GUID_LINE1 63
#define GUID_LINE2 35
#define GUID_LINE3 8

#define GUID_LINE4 145

extern u8 recv_flag;
extern u8 ADC_flag;
extern u8 ADC_show_flag;
extern u8 x_coord_vcc;
extern u8 x_coord_phs;
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];
extern float frequency_REF;
extern float frequency_F;
extern float phase;
extern float vcc_REF;
extern float vcc_F;

void info_init(void);
void graph_init(u8 mode);
void info_show(void);

#endif
