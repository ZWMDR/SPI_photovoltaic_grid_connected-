#include "menu.h"

void LCD_Show_Clear(void)//ÇåÆÁ
{
	LCD_Fill(0,0,240,305,WHITE);
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
}

void LCD_Show_Menu(u8* key_flag,u8 status,u8 menu_status)
{
	if(*key_flag)
	{
		if(last_menu_status!=menu_status)
		{
			LCD_Show_Clear();
			last_menu_status=menu_status;
		}
		if(menu_status==0)
		{
			LCD_Show_Menu_1(status);
		}
		else if(menu_status>9 && menu_status<20)
		{
			
		}
		else if(menu_status>19 && menu_status<30)
		{
			
		}
		
		*key_flag=0;
	}
}

void LCD_Show_Menu_1(u8 status)
{
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	//GUI_show_frame(11,BROWN,40,80,210,220);
	switch(status)
	{
		case 0:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(50,120,180,24,24,"Tracing Mode");
			LCD_ShowString(50,160,180,24,24,"Voltage Mode");
			break;
		}
		case 1:
		{
			POINT_COLOR=RED;
			BACK_COLOR=YELLOW;
			LCD_ShowString(50,120,180,24,24,"Tracing Mode");
			BACK_COLOR=WHITE;
			LCD_ShowString(50,160,180,24,24,"Voltage Mode");
			break;
		}
		case 2:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(50,120,180,24,24,"Tracing Mode");
			BACK_COLOR=YELLOW;
			LCD_ShowString(50,160,180,24,24,"Voltage Mode");
			break;
		}
	}
}
