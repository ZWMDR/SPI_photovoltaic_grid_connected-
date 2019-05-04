#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "spi.h"
#include "exti.h"
#include "GUI.h"
 
 
u8 recv_flag;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 DMA_buff_RX[DMA_buff_len];
u16 DMA_buff_TX[DMA_buff_len];
float frequency;

int main(void)
{
	recv_flag=0;
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(19200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();
	KEY_Init();				//按键初始化
	SPI1_Init();
	info_init();
	
	while(1)
	{
		info_show();
	}
}
