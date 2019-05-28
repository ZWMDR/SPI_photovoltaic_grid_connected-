#include "GUI.h"

float vcc_REF_lst;
float vcc_F_lst;
float phase_lst;

void info_init(void)
{
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	LCD_ShowString(40,200,200,16,16,"Voltage_REF:     .   V");
	LCD_ShowString(40,220,200,16,16,"Voltage_F:       .   V");
	LCD_ShowString(40,240,200,16,16,"Frequency_REF:   .   Hz");
	LCD_ShowString(40,260,200,16,16,"Frequency_F:     .   Hz");
	LCD_ShowString(40,280,200,16,16,"Phase:           .   T");
	
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
	
	LCD_Fill(0,302,240,304,RED);
	
	BACK_COLOR=WHITE;
	POINT_COLOR=MAGENTA;
}

void graph_init(u8 mode)
{
	switch(mode)
	{
		case 0:
		{
			LCD_Fill(0,190,240,192,RED);
			LCD_Fill(0,98,240,99,RED);
			LCD_Fill(0,90,240,91,RED);
		
			LCD_Fill(0,0,240,89,LIGHTBLUE);
			LCD_Fill(0,100,240,189,LIGHTBLUE);
			LCD_Fill(0,92,240,97,BLUE);
		
			POINT_COLOR=YELLOW;
			LCD_DrawLine(0,GUID_LINE1,240,GUID_LINE1);
			LCD_DrawLine(0,GUID_LINE2,240,GUID_LINE2);
			LCD_DrawLine(0,GUID_LINE3,240,GUID_LINE3);
			
			LCD_DrawLine(0,GUID_LINE4,240,GUID_LINE4);
			break;
		}
		
		case 1:
		{
			LCD_Fill(0,0,240,89,LIGHTBLUE);
			POINT_COLOR=YELLOW;
			LCD_DrawLine(0,GUID_LINE1,240,GUID_LINE1);
			LCD_DrawLine(0,GUID_LINE2,240,GUID_LINE2);
			LCD_DrawLine(0,GUID_LINE3,240,GUID_LINE3);
			break;
		}
		case 2:
		{
			LCD_Fill(0,100,240,189,LIGHTBLUE);
			POINT_COLOR=YELLOW;
			LCD_DrawLine(0,GUID_LINE4,240,GUID_LINE4);
			break;
		}
	}
}

void vcc_graph_show(float vcc_REF,float vcc_F)
{
	if(x_coord_vcc>0)
	{
		POINT_COLOR=BLUE;
		LCD_DrawLine(x_coord_vcc-1,89*(1-vcc_REF_lst*0.3030303),x_coord_vcc,89*(1-vcc_REF*0.3030303));
		POINT_COLOR=RED;
		LCD_DrawLine(x_coord_vcc-1,89*(1-vcc_F_lst*0.3030303),x_coord_vcc,89*(1-vcc_F*0.3030303));
	}
	
	vcc_REF_lst=vcc_REF;
	vcc_F_lst=vcc_F;
	
	if((x_coord_vcc+=2)>=240)
	{
		x_coord_vcc=0;
		graph_init(1);
	}
}

void phase_graph_show(float phase)
{
	if(x_coord_phs>0)
	{
		POINT_COLOR=BLUE;
		LCD_DrawLine(x_coord_phs-1,GUID_LINE4-90*phase_lst,x_coord_phs,GUID_LINE4-90*phase);
	}
	
	phase_lst=phase;
	
	if((x_coord_phs+=2)>=240)
	{
		x_coord_phs=0;
		graph_init(2);
	}
}

void info_show(void)
{
	float temp;
	int adcx;
	
	if(recv_flag)
	{
		POINT_COLOR=BLUE;
		
		temp=frequency_REF;
		adcx=temp;
		LCD_ShowxNum(152,240,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,240,temp,3,16,0x80);
		
		temp=frequency_F;
		adcx=temp;
		LCD_ShowxNum(152,260,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,260,temp,3,16,0x80);
		
		temp=phase;
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(144,280,8,16,16,"-");
		}
		else
			LCD_ShowString(144,280,8,16,16," ");
		adcx=temp;
		LCD_ShowxNum(152,280,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,280,temp,3,16,0x80);
		
		phase_graph_show(phase);
		recv_flag=0;
	}
	
	if(ADC_show_flag)
	{
		POINT_COLOR=MAGENTA;

		temp=vcc_F;
		adcx=temp;
		LCD_ShowxNum(152,200,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,200,temp,3,16,0x80);
		ADC_flag=0;
		
		temp=vcc_F;
		adcx=temp;
		LCD_ShowxNum(152,220,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,220,temp,3,16,0x80);
		ADC_flag=0;
		
		vcc_graph_show(vcc_REF,vcc_F);
		ADC_show_flag=0;
	}
}
