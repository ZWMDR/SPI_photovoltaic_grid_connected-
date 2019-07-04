#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "SPI_DMA.h"
#include "exti.h"
#include "GUI.h"
#include "remote.h"

u8 IR_flag;
u8 key_flag;
u8 status;
u8 menu_status;
u8 last_menu_status;
u8 IR_instruct;
u8 IR_key;

u8 recv_flag;
u8 select_status;
u8 status;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 DMA_SPI_buff_RX[DMA_buff_len_SPI];
u16 DMA_SPI_buff_TX[DMA_buff_len_SPI];

int main(void)
{
	/*
	GUI_WW_InitTypeDef GUI_WW;
	GUI_WW_gd_InitTypeDef GUI_WW_gd_v;
	GUI_WW_gd_InitTypeDef GUI_WW_gd_h;
	GUI_MW_InitTypeDef GUI_MW;
	GUI_MW_InitTypeDef GUI_MW;
	GUI_msg_num msg_num[2];
	GUI_msg_str msg_str[1];
	*/
	GUI_Menu_InitTypeDef Menu;
	GUI_catalog_table catalogs[2];
	
	
	//延时函数初始化
	delay_init();
	
	//串口初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//uart_init(19200);
	
	//LED、LCD初始化
	LED_Init();
 	GUI_Init();
	
	//按键初始化
	KEY_Init();
	
	//SPI初始化
	SPI1_DMA1_Init(2000-1,7200-1,0,0);
	
	//红外初始化
	
	
	//一级菜单
	Menu.start_xcoord=40;
	Menu.start_ycoord=80;
	Menu.end_xcoord=210;
	Menu.end_ycoord=220;
	
	Menu.show_frame=1;
	Menu.frame_color=BROWN;
	Menu.frame_mode=3;
	
	Menu.point_color=BLACK;
	Menu.back_color=WHITE;
	Menu.select_back_color=BROWN;
	Menu.select_point_color=WHITE;
	
	Menu.table_num=2;
	Menu.catalog_tables=catalogs;
	Menu.catalog_tables[0].content="Tracing Mode";
	Menu.catalog_tables[0].length=12;
	Menu.catalog_tables[0].size=24;
	Menu.catalog_tables[0].xcoord=50;
	Menu.catalog_tables[0].ycoord=120;
	
	Menu.catalog_tables[1].content="Voltage Mode";
	Menu.catalog_tables[1].length=12;
	Menu.catalog_tables[1].size=24;
	Menu.catalog_tables[1].xcoord=50;
	Menu.catalog_tables[1].ycoord=160;
	
	GUI_Menu_Init(&Menu);
	
	recv_flag=0;
	IR_flag=key_flag=menu_status=status=IR_instruct=0;
	last_menu_status=0;
	select_status=status=0;
	
	while(1)
	{
		
	}
}
