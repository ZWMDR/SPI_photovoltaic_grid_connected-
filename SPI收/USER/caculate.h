#ifndef __CACULATE_H
#define __CACULATE_H
#include "spi.h"
#include "adc.h"

extern u8 recv_flag;
extern u8 ADC_flag;
extern u16 DMA_buff_RX[DMA_buff_len_SPI];
extern u16 DMA_buff_TX[DMA_buff_len_SPI];
extern u16 DMA_buff[DMA_buff_len];

typedef struct PID
{
    int SetPoint; //�趨Ŀ�� Desired Value
    float Proportion; //�������� Proportional Const
    float Integral; //���ֳ��� Integral Const
    float Derivative; //΢�ֳ��� Derivative Const
    int LastError; //Error[-1]
    int PrevError; //Error[-2]
}PID;

extern PID PID_frequency,PID_phase,PID_VCC;


void PID_init(void);
u16 get_frequency(float *frequency_REF,float *frequency_F);
u16 get_phase(float *phase);
u16 get_VCC_coef(float *VCC);

#endif
