#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "SPI_DMA.h"
#include "exti.h"
#include "GUI.h"
#include "remote.h"
#include "menu.h"

u8 IR_flag;
u8 key_flag;
u8 status;
u8 menu_status;
u8 last_menu_status;
u8 IR_instruct;
u8 IR_key;
u8 assign_flag;

u8 recv_flag;
u8 select_status;
u8 status;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 Voltage;
u16 Current;

u16 Set_Voltage;

u16 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u16 DMA_SPI_buff_TX[DMA_SPI_buff_len];

u8 digits[4];

int main(void)
{
	GUI_WW_InitTypeDef GUI_WW;
	GUI_WW_gd_InitTypeDef GUI_gd;
	/*
	GUI_WW_gd_InitTypeDef GUI_WW_gd_v;
	GUI_WW_gd_InitTypeDef GUI_WW_gd_h;
	GUI_MW_InitTypeDef GUI_MW;
	GUI_MW_InitTypeDef GUI_MW;
	GUI_msg_num msg_num[2];
	GUI_msg_str msg_str[1];
	
	GUI_Menu_InitTypeDef Menu;
	GUI_catalog_table catalogs[2];
	*/
	
	//延时函数初始化
	delay_init();
	
	//串口初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(19200);
	
	//LED、LCD初始化
	LED_Init();
 	GUI_Init();
	
	//按键初始化
	KEY_Init();
	
	//SPI初始化
	//delay_ms(1000);
	SPI1_DMA1_Init(2000-1,7200-1,0,0);
	
	//红外初始化
	Remote_Init();
	
	GUI_WW.start_xcoord=0;
	GUI_WW.start_ycoord=0;
	GUI_WW.end_xcoord=240;
	GUI_WW.end_ycoord=170;
	GUI_WW.back_color=LIGHTBLUE;
	GUI_WW.set_axle_wire=0;
	GUI_WW.set_vertical_gd=0;
	GUI_WW.set_hroizontal_gd=1;
	GUI_WW.horizontal_gd=&GUI_gd;
	
	GUI_gd.gd_color=RED;
	GUI_gd.gd_coord=56;
	GUI_gd.gd_interval=56;
	GUI_gd.gd_num=3;
	GUI_gd.wire_type=2;
	GUI_gd.set_gd=1;
	
	
	
	recv_flag=0;
	IR_flag=menu_status=status=IR_instruct=0;
	last_menu_status=0;
	assign_flag=0;
	key_flag=1;
	select_status=status=0;
	
	array_init_u8(digits,4);
	
	
	while(1)
	{
		remote_key();
		LCD_Show_Menu(&key_flag,status,menu_status,&recv_flag,&GUI_WW);
		
		if(assign_flag)
		{
			DMA_SPI_buff_TX[0]=0x0B0B;
			DMA_SPI_buff_TX[1]=menu_status;
			DMA_SPI_buff_TX[2]=Set_Voltage;
			SPI_send();
			assign_flag=0;
		}
	}
}
