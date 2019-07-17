#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "GUI.h"
#include "remote.h"
#include "menu.h"
#include "24l01.h"

u8 IR_flag;
u8 key_flag;
u8 status;
u8 menu_status;
u8 last_menu_status;
u8 IR_instruct;
u8 IR_key;
u8 assign_flag;
u8 NRF_mode;

u8 recv_flag;
u8 select_status;
u8 status;

u8 exception_flag;
u8 buzzer_count;

u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 Voltage;
u16 Current;

u16 Set_Voltage;

u8 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u8 DMA_SPI_buff_TX[DMA_SPI_buff_len];
u16 SPI_send_buff[SPI_send_buff_len];
u16 SPI_recv_buff[SPI_send_buff_len];

u8 digits[4];
u8 digits_MPPT[4];

int main(void)
{
	u8 lst_exception_flag;
	GUI_WW_InitTypeDef GUI_WW;
	//GUI_WW_InitTypeDef GUI_WW_MPPT;
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
	//SPI1_DMA1_Init(2000-1,7200-1,0,0);
	NRF24L01_Init();
	NRF24L01_RX_Mode();
	NRF_mode=0;
	
	//红外初始化
	Remote_Init();
	
	GUI_WW.start_xcoord=0;
	GUI_WW.start_ycoord=0;
	GUI_WW.end_xcoord=240;
	GUI_WW.end_ycoord=180;
	GUI_WW.back_color=LIGHTBLUE;
	GUI_WW.set_axle_wire=0;
	GUI_WW.set_vertical_gd=0;
	GUI_WW.set_hroizontal_gd=1;
	GUI_WW.vertical_gd=&GUI_gd;
	
	GUI_gd.gd_color=RED;
	GUI_gd.gd_coord=45;
	GUI_gd.gd_interval=45;
	GUI_gd.gd_num=3;
	GUI_gd.wire_type=2;
	GUI_gd.set_gd=1;
	
	//看门狗初始化
	//IWDG_Init(4,0xAFF);
	
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
		
		if(NRF_mode==0)
		{
			recv_flag=NRF_recv();
		}
		if(recv_flag)
		{
			printf("%x %d %d, %d %d\r\n",SPI_recv_buff[0],SPI_recv_buff[1],SPI_recv_buff[2],SPI_recv_buff[3],SPI_recv_buff[4]);
		}
		
		if(assign_flag)
		{
			SPI_send_buff[0]=0x0B0B;
			SPI_send_buff[1]=menu_status;
			SPI_send_buff[2]=Set_Voltage;
			NRF_send(SPI_send_buff,SPI_send_buff_len);
			assign_flag=0;
		}
		
		if(menu_status>=10 && menu_status<20)
		{
			if(recv_flag)
			{
				if(exception_flag)
				{
					lst_exception_flag=1;
					LCD_Show_Exception(buzzer_count);
				}
				else
				{
					if(lst_exception_flag)
					{
						LCD_Show_Wave_MPPT_Init(&GUI_WW);
						LCD_Show_InputBox(digits,status-20,2,2,"Voltage:","V");
						lst_exception_flag=0;
					}
					LCD_Show_Wave(Voltage,Current,MAGENTA,BROWN,&GUI_WW);
					LCD_Show_Msg(Frequency_REF,Current);
				}
				recv_flag=0;
			}
		}
		else if(menu_status>=20 && menu_status<30)
		{
			if(recv_flag)
			{
				if(exception_flag)//异常状态显示
				{
					lst_exception_flag=1;
					LCD_Show_Exception(buzzer_count);
				}
				else
				{
					if(lst_exception_flag)//显示恢复
					{
						LCD_Show_Clear();
						LCD_Show_Wave_MPPT_Init(&GUI_WW);
						LCD_Show_InputBox(digits,status-20,2,2,"Voltage:","V");
						lst_exception_flag=0;
					}
					LCD_Show_Wave(Voltage,Set_Voltage,MAGENTA,BROWN,&GUI_WW);
					LCD_Show_Msg(Frequency_F,Current);
				}
				recv_flag=0;
			}
		}
	}
}
