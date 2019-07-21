#include "menu.h"

void LCD_Show_Clear(void)//清屏
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
				LCD_Show_Wave_Init(GUI_WW);
			}
			else if(menu_status==10)
			{
				digits_MPPT[0]=2;
				digits_MPPT[1]=5;
				digits_MPPT[2]=0;
				digits_MPPT[3]=0;
				LCD_Show_Wave_MPPT_Init(GUI_WW);
			}
			last_menu_status=menu_status;
		}
		
		if(menu_status==0)
		{
			LCD_Show_Menu_1(status);
		}
		else if(menu_status>9 && menu_status<20)//跟踪模式
		{
			LCD_Show_InputBox(digits_MPPT,status-10,2,2,"MAX_Vpp:","V");
		}
		else if(menu_status>19 && menu_status<30)//稳压模式
		{
			LCD_Show_InputBox(digits,status-20,2,2,"Voltage:","V");
			
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
			LCD_ShowString(45,100,180,24,24,"MPPT    Mode");
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			break;
		}
		case 1:
		{
			POINT_COLOR=RED;
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,100,180,24,24,"MPPT    Mode");
			BACK_COLOR=WHITE;
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			break;
		}
		case 2:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(45,100,180,24,24,"MPPT    Mode");
			LCD_ShowString(45,180,180,24,24,"Current Mode");
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			break;
		}
		case 3:
		{
			POINT_COLOR=RED;
			BACK_COLOR=WHITE;
			LCD_ShowString(45,100,180,24,24,"MPPT    Mode");
			LCD_ShowString(45,140,180,24,24,"Voltage Mode");
			BACK_COLOR=YELLOW;
			LCD_ShowString(45,180,180,24,24,"Current Mode");
		}
	}
}

void LCD_Show_InputBox(u8* digits,u8 status,u16 former,u16 latter,char* header,char* ending)
{
	u8 i;
	u16 num=former+latter;
	u16 xcoord=130;
	
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	LCD_ShowString(25,220,180,24,24,(u8*)header);
	
	if(assign_flag)
	{
		BACK_COLOR=BLUE;
		POINT_COLOR=WHITE;
		
		for(i=0;i<num;i++)
		{
			LCD_ShowNum(xcoord,220,digits[i],1,24);
			
			if(i==former-1)
			{
				xcoord+=12;
				LCD_ShowChar(xcoord,220,'.',24,0);
			}
			xcoord+=12;
		}
		LCD_ShowString(xcoord,220,180,24,24,(u8*)ending);
		return;
	}
	POINT_COLOR=BLACK;
	for(i=0;i<num;i++)
	{
		if(i+1==status)
		{
			BACK_COLOR=YELLOW;
		}
		LCD_ShowNum(xcoord,220,digits[i],1,24);
		if(i+1==status)
		{
			BACK_COLOR=WHITE;
		}
		
		if(i==former-1)
		{
			xcoord+=12;
			LCD_ShowChar(xcoord,220,'.',24,0);
		}
		xcoord+=12;
	}
	LCD_ShowString(xcoord,220,180,24,24,(u8*)ending);
}


void LCD_Show_Wave_Init(GUI_WW_InitTypeDef* GUI_WW)
{
	GUI_WaveWindow_Init(GUI_WW);
	POINT_COLOR=RED;
	BACK_COLOR=LIGHTBLUE;
	LCD_DrawLine(0,181,240,181);
	LCD_DrawLine(0,182,240,182);
	LCD_ShowString(220,47,180,12,12,"45V");
	LCD_ShowString(220,92,180,12,12,"30V");
	LCD_ShowString(220,137,180,12,12,"15V");
				
	LCD_Fill(10,194,25,196,BROWN);
	POINT_COLOR=BROWN;
	BACK_COLOR=WHITE;
	LCD_ShowString(30,187,180,16,16,"VCC_Target");
	
	LCD_Fill(130,194,145,196,MAGENTA);
	POINT_COLOR=MAGENTA;
	LCD_ShowString(150,187,180,16,16,"VCC_OutPut");
	
	LCD_Fill(0,210,240,212,RED);
}

void LCD_Show_Wave_MPPT_Init(GUI_WW_InitTypeDef* GUI_WW)
{
	GUI_WaveWindow_Init(GUI_WW);
	POINT_COLOR=RED;
	BACK_COLOR=LIGHTBLUE;
	LCD_DrawLine(0,181,240,181);
	LCD_DrawLine(0,182,240,182);
	LCD_ShowString(220,47,180,12,12,"45V");
	LCD_ShowString(220,92,180,12,12,"30V");
	LCD_ShowString(220,137,180,12,12,"15V");

	LCD_Fill(10,194,25,196,BROWN);
	POINT_COLOR=BROWN;
	BACK_COLOR=WHITE;
	LCD_ShowString(30,187,180,16,16,"InPut Vd");
	
	LCD_Fill(130,194,145,196,MAGENTA);
	POINT_COLOR=MAGENTA;
	LCD_ShowString(150,187,180,16,16,"OutPut Vo");
	
	LCD_Fill(0,210,240,212,RED);
}

void LCD_Show_Wave(u16 voltage_F,u16 target,u16 voltage_color,u16 target_color,GUI_WW_InitTypeDef* GUI_WW)
{
	static u16 last_ycoord_target=180;
	static u16 last_ycoord_voltage=180;
	static u8 xcoord=0;
	
	u16 ycoord_target;
	u16 ycoord_voltage;
	
	if(voltage_F>=1450)
		ycoord_voltage=0;
	else
		ycoord_voltage=180*(1-((float)voltage_F*0.0413+0.0932)*0.01666667);
	
	if(target>=6000)
		ycoord_target=0;
	else
		ycoord_target=180*(1-(float)target*0.000166666);
	
	//printf("voltage_F=%d, ycoord=%d\r\n",voltage_F,ycoord_voltage);
	
	POINT_COLOR=voltage_color;
	LCD_DrawLine(xcoord,last_ycoord_voltage,xcoord+1,ycoord_voltage);
	LCD_DrawLine(xcoord,last_ycoord_voltage+1,xcoord+1,ycoord_voltage+1);
	
	POINT_COLOR=target_color;
	LCD_DrawLine(xcoord,last_ycoord_target,xcoord+1,ycoord_target);
	LCD_DrawLine(xcoord,last_ycoord_target+1,xcoord+1,ycoord_target+1);
	
	if(xcoord<238)
		xcoord=xcoord+2;
	else
	{
		xcoord=0;
		GUI_WaveWindow_Init(GUI_WW);
		POINT_COLOR=RED;
		BACK_COLOR=LIGHTBLUE;
		LCD_DrawLine(0,181,240,181);
		LCD_DrawLine(0,182,240,182);
		LCD_ShowString(220,47,180,12,12,"45V");
		LCD_ShowString(220,92,180,12,12,"30V");
		LCD_ShowString(220,137,180,12,12,"15V");
	}
	last_ycoord_voltage=ycoord_voltage;
	last_ycoord_target=ycoord_target;
}

void LCD_Show_Wave_MPPT(u16 voltage_F,u16 target,u16 voltage_color,u16 target_color,GUI_WW_InitTypeDef* GUI_WW)
{
	static u16 last_ycoord_target=180;
	static u16 last_ycoord_voltage=180;
	static u8 xcoord=0;
	
	u16 ycoord_target;
	u16 ycoord_voltage;
	
	if(voltage_F>=1450)
		ycoord_voltage=0;
	else
		ycoord_voltage=180*(1-((float)voltage_F*0.0413+0.0932)*0.01666667);
	
	if(target>=6000)
		ycoord_target=0;
	else
		ycoord_target=180*(1-(float)target*0.0001666);
	
	//printf("voltage_F=%d, ycoord=%d\r\n",voltage_F,ycoord_voltage);
	
	POINT_COLOR=voltage_color;
	LCD_DrawLine(xcoord,last_ycoord_voltage,xcoord+1,ycoord_voltage);
	LCD_DrawLine(xcoord,last_ycoord_voltage+1,xcoord+1,ycoord_voltage+1);
	
	POINT_COLOR=target_color;
	LCD_DrawLine(xcoord,last_ycoord_target,xcoord+1,ycoord_target);
	LCD_DrawLine(xcoord,last_ycoord_target+1,xcoord+1,ycoord_target+1);
	
	if(xcoord<238)
		xcoord=xcoord+2;
	else
	{
		xcoord=0;
		GUI_WaveWindow_Init(GUI_WW);
		POINT_COLOR=RED;
		BACK_COLOR=LIGHTBLUE;
		LCD_DrawLine(0,181,240,181);
		LCD_DrawLine(0,182,240,182);
		LCD_ShowString(220,47,180,12,12,"45V");
		LCD_ShowString(220,92,180,12,12,"30V");
		LCD_ShowString(220,137,180,12,12,"15V");
	}
	last_ycoord_voltage=ycoord_voltage;
	last_ycoord_target=ycoord_target;
}

void LCD_Show_Msg(u16 Frequency,u16 Current)
{
	float frequency=2000000.0/Frequency;
	float current=Current*0.0007461+0.0047563;
	float temp;
	short adcx;
	
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE;
	LCD_ShowString(40,260,180,12,12,"Frequency:   .  Hz");
	adcx=frequency;
	LCD_ShowxNum(100,260,adcx,3,12,0);
	temp=frequency-adcx;
	temp*=100;
	LCD_ShowxNum(124,260,temp,2,12,0x80);
	
	LCD_ShowString(40,280,180,12,12,"Current:  .   A");
	adcx=current;
	LCD_ShowxNum(88,280,adcx,2,12,0);
	temp=current-adcx;
	temp*=1000;
	LCD_ShowxNum(104,280,temp,3,12,0x80);
}



void LCD_Show_Exception(u8 count, char*msg)
{
	if(count==5)
	{
		LCD_Fill(0,100,240,240,RED);
		BACK_COLOR=RED;
		POINT_COLOR=YELLOW;
		LCD_ShowString(30,120,180,24,24,"EXCEPTION:");
		LCD_ShowString(30,150,240,24,24,(u8*)msg);
		LCD_ShowString(30,190,200,16,16,"Retry   Seconds Later...");
	}
	BACK_COLOR=RED;
	POINT_COLOR=YELLOW;
	
	LCD_ShowNum(78,190,count,1,16);
}



