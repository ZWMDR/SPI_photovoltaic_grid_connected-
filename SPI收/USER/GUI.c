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

void info_show(void)
{
	float temp;
	int adcx;
	
	if(recv_flag)
	{
		frequency=temp=1000000.0/Frequency_REF;
		adcx=temp;
		LCD_ShowxNum(152,220,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,220,temp,3,16,0x80);
		
		temp=1000000.0/Frequency_F;
		adcx=temp;
		LCD_ShowxNum(152,240,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(184,240,temp,3,16,0x80);
		
		temp=(double)Period_REF/Frequency_REF;
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
		
		resend();
		recv_flag=0;
	}
	
	if(ADC_flag)
	{
		
		ADC_flag=0;
	}
}
