#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "spi.h"
#include "exti.h"
#include "GUI.h"
#include "adc.h"
 

u8 recv_flag;
u8 ADC_flag;
u8 ADC_show_flag;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 DMA_buff_RX[DMA_buff_len_SPI];
u16 DMA_buff_TX[DMA_buff_len_SPI];
u16 DMA_buff[DMA_buff_len];
float frequency_REF;
float frequency_F;
float phase;
float vcc_REF;
float vcc_F;
u16 VCC_REF;
u16 VCC_F;
u8 x_coord_vcc;
u8 x_coord_phs;

PID PID_frequency,PID_phase,PID_VCC;

int main(void)
{
	recv_flag=0;
	x_coord_vcc=x_coord_phs=0;
	ADC_flag=ADC_show_flag=0;
	VCC_REF=VCC_F=0;
	frequency_REF=frequency_F=phase=vcc_REF=vcc_F=0;
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//uart_init(19200);	 	//���ڳ�ʼ��
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LCD_Init();
	KEY_Init();				//������ʼ��
	SPI1_Init();
	ADC1_TIM_Init(_5kHz);
	info_init();
	graph_init(0);
	
	while(1)
	{
		info_show();
	}
}
