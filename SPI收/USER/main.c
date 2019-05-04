#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "spi.h"
#include "exti.h"
 
 
u8 recv_flag;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 DMA_buff_RX[DMA_buff_len];
u16 DMA_buff_TX[DMA_buff_len];

int main(void)
{
	double temp;
	int adcx;
	recv_flag=0;
	delay_init();	    	 //��ʱ������ʼ��
	uart_init(19200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LCD_Init();
	KEY_Init();				//������ʼ��
	SPI1_Init();
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_16);
 	POINT_COLOR=RED;//��������Ϊ��ɫ
	LCD_ShowString(60,50,200,16,16,"Frequency_REF:   .   Hz");
	LCD_ShowString(60,70,200,16,16,"Frequency_F:     .   Hz");
	LCD_ShowString(60,90,200,16,16,"Period_REF:      .   T");
	LCD_ShowString(60,110,200,16,16,"Period_F:       .   T");
	POINT_COLOR=BLUE;		//��������Ϊ��ɫ
	while(1)
	{
		if(recv_flag)
		{
			temp=1000000.0/Frequency_REF;
			adcx=temp;
			LCD_ShowxNum(172,50,adcx,3,16,0);
			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(204,50,temp,3,16,0x80);
			
			temp=1000000.0/Frequency_F;
			adcx=temp;
			LCD_ShowxNum(172,70,adcx,3,16,0);
			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(204,70,temp,3,16,0x80);
			
			temp=(double)Period_REF/Frequency_REF;
			adcx=temp;
			LCD_ShowxNum(172,90,adcx,3,16,0);
			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(204,90,temp,3,16,0x80);
			
			temp=(double)Period_F/Frequency_F;
			adcx=temp;
			LCD_ShowxNum(172,110,adcx,3,16,0);
			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(204,110,temp,3,16,0x80);
			
			recv_flag=0;
		}
		//LED1=~LED1;
	}
}
