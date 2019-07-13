#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
#include "timer.h"

void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度
u16 SPI1_ReadWriteByte(u16 TxData);//SPI总线读写一个字节
void Send_Frequency(u16 F_REF,u16 F_F);
void Send_Phase(u16 P_REF,u16 P_F);

#endif

