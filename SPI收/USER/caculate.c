#include "caculate.h"

void resend(void)
{
	DMA_buff_TX[0]=(u16)(180000/frequency);
	DMA_buff_TX[1]=0;
	DMA_buff_TX[2]=0;
	DMA_buff_TX[3]=0;
	
	DMA1_Channel3->CNDTR=4;
	DMA_Cmd(DMA1_Channel3,ENABLE);
}
