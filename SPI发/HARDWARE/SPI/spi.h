#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "timer.h"

#define DMA_buff_len 4

extern u16 DMA_buff_TX[DMA_buff_len];
extern u16 DMA_buff_RX[DMA_buff_len];
extern u16 t;

void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void Send_Frequency(u16 F_REF,u16 F_F);
void Send_Phase(u16 P_REF,u16 P_F);
		 
#endif

