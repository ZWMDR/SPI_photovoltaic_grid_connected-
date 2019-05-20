#include "GUI.h"


void info_init(void)
{
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;
	LCD_ShowString(40,200,200,16,16,"Voltage Vpp  :   .   V");
	LCD_ShowString(40,220,200,16,16,"Frequency_REF:   .   Hz");
	LCD_ShowString(40,240,200,16,16,"Frequency_F:     .   Hz");
	LCD_ShowString(40,260,200,16,16,"Period_REF:      .   T");
	LCD_ShowString(40,280,200,16,16,"Period_F:        .   T");
	
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
	
	LCD_Fill(0,302,240,304,RED);
	
	BACK_COLOR=WHITE;
	POINT_COLOR=MAGENTA;
}

void graph_init(void)
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
	LCD_DrawLine(0,GUID_LINE5,240,GUID_LINE5);
	LCD_DrawLine(0,GUID_LINE6,240,GUID_LINE6);
	
}

void graph_show(void)
{
	
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
		LCD_ShowxNum(152,220,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,220,temp,3,16,0x80);
		
		temp=frequency_F;
		adcx=temp;
		LCD_ShowxNum(152,240,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,240,temp,3,16,0x80);
		
		temp=phase;
		adcx=temp;
		LCD_ShowxNum(152,260,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,260,temp,3,16,0x80);
		
		temp=(double)Period_F/Frequency_F;
		adcx=temp;
		LCD_ShowxNum(152,280,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,280,temp,3,16,0x80);
		
		recv_flag=0;
	}
	
	if(ADC_flag)
	{
		POINT_COLOR=MAGENTA;
		
		VCC=get_vcc(DMA_buff,DMA_buff_len,0)*0.00080566;
		temp=VCC;
		adcx=temp;
		LCD_ShowxNum(152,200,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,200,temp,3,16,0x80);
		ADC_flag=0;
		
		graph_show();
	}
}
