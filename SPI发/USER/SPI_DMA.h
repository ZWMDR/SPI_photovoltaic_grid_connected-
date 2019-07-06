#ifndef __SPI_DMA_H
#define __SPI_DMA_H

#include "stm32f10x_dma.h"
#include "sys.h"
#include "timer.h"
#include "led.h"
#include "sp_math.h"
#include "usart.h"

#define DMA_SPI_buff_len 7

extern u16 DMA_SPI_buff_TX[DMA_SPI_buff_len];
extern u16 DMA_SPI_buff_RX[DMA_SPI_buff_len];
extern u16 SPI_send_buff[DMA_SPI_buff_len];

extern u8 send_flag;
extern u8 recv_flag;
extern u8 working_mode;
extern u16 target;

void SPI1_DMA1_Init(u16 arr,u16 psc,u8 master_slaver,u8 open_scheduled_transmition);
/* -----------------------------------------------------------------------------
主机：master_slaver=1     从机：master_slaver=0
定时器定时发送：open_scheduled_transmition=1，否则open_scheduled_transmition=0
-------------------------------------------------------------------------------*/

void SPI_send(const u16 send_buff[DMA_SPI_buff_len]);
/*------------------------------------------------------------------------------
SPI单次数据发送，接收采用DMA中断方式
发送数据保存在数组send_buff中，数组长度固定为 DMA_SPI_buff_len
--------------------------------------------------------------------------------*/


#endif
