#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "lcd.h"
#include "usart.h"

#define DMA_buff_len 4

extern u8 recv_flag;
extern u16 DMA_buff_RX[DMA_buff_len];
extern u16 DMA_buff_TX[DMA_buff_len];
 

void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

