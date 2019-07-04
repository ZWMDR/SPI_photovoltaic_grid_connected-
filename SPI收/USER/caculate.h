#ifndef __CACULATE_H
#define __CACULATE_H

#include "sys.h"
#include "SPI_DMA.h"

extern u8 recv_flag;
extern u16 Frequency_REF;
extern u16 Frequency_F;
extern u16 Period_REF;
extern u16 Period_F;
extern u16 DMA_buff_RX[DMA_SPI_buff_len];
extern u16 DMA_buff_TX[DMA_SPI_buff_len];

typedef struct PID
{
    s16 SetPoint; //�趨Ŀ�� Desired Value
    float Proportion; //�������� Proportional Const
    float Integral; //���ֳ��� Integral Const
    float Derivative; //΢�ֳ��� Derivative Const
    s16 LastError; //Error[-1]
    s16 PrevError; //Error[-2]
}PID;

extern PID PID_frequency,PID_phase,PID_VCC;


void PID_init(void);
u16 get_frequency(float *frequency_REF,float *frequency_F);
u16 get_phase(float *phase);
u16 get_vcc(u16 *arr,u16 count,u8 status,u16 *VCC);
u16 get_VCC_coef(void);

#endif
