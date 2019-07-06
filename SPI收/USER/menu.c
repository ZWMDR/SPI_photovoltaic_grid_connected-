#include "menu.h"

void LCD_Show_Clear(void)//ÇåÆÁ
{
	LCD_Fill(0,0,240,305,WHITE);
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
}

void LCD_Show_Menu(u8* key_flag,u8 status,u8 menu_status,u8* recv_flag,GUI_WW_InitTypeDef* GUI_WW)
{
	if(*key_flag)
	{
		if(last_menu_status!=menu_status)
		{
			LCD_Show_Clear();
			if(menu_status==20)
			{
				
			}
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
			LCD_Show_InputBox(status-20,2,2,"Voltage","V");
			if(*recv_flag)
			{
				
				*recv_flag=0;
			}
		}
		
		*key_flag=0;
	}
}

void LCD_Show_Menu_1(u8 status)
{
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	GUI_show_frame(13,BROWN,30,60,210,235);
	switch(status)
	{
		case 0:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(45,100,180,24,24,"Tracing Mode");
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			break;
		}
		case 1:
		{
			POINT_COLOR=RED;
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,100,180,24,24,"Tracing Mode");
			BACK_COLOR=WHITE;
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			break;
		}
		case 2:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(45,100,180,24,24,"Tracing Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			break;
		}
		case 3:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(45,100,180,24,24,"Tracing Mode");
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,180,180,24,24,"Current Mode");
		}
	}
}

void LCD_Show_InputBox(u8 status,u16 former,u16 latter,char* header,char* ending)
{
	u8 i;
	u16 num=former+latter;
	u16 xcoord=80;
	
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	LCD_ShowString(45,200,180,24,24,(u8*)header);
	
	if(assign_flag)
	{
		BACK_COLOR=BLUE;
		POINT_COLOR=WHITE;
		
		for(i=0;i<num;i++)
		{
			LCD_ShowNum(xcoord,240,digits[i],1,24);
			
			if(i==former-1)
			{
				xcoord+=12;
				LCD_ShowChar(xcoord,240,'.',24,0);
			}
			xcoord+=12;
		}
		LCD_ShowString(xcoord,240,180,24,24,(u8*)ending);
		return;
	}
	POINT_COLOR=BLACK;
	for(i=0;i<num;i++)
	{
		if(i+1==status)
		{
			BACK_COLOR=YELLOW;
		}
		LCD_ShowNum(xcoord,240,digits[i],1,24);
		if(i+1==status)
		{
			BACK_COLOR=WHITE;
		}
		
		if(i==former-1)
		{
			xcoord+=12;
			LCD_ShowChar(xcoord,240,'.',24,0);
		}
		xcoord+=12;
	}
	LCD_ShowString(xcoord,240,180,24,24,(u8*)ending);
}

