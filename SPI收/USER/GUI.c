#include "GUI.h"


void info_init(void)
{
	POINT_COLOR=RED;//设置字体为红色
	LCD_ShowString(60,50,200,16,16,"Frequency_REF:   .   Hz");
	LCD_ShowString(60,70,200,16,16,"Frequency_F:     .   Hz");
	LCD_ShowString(60,90,200,16,16,"Period_REF:      .   T");
	LCD_ShowString(60,110,200,16,16,"Period_F:        .   T");
}

void info_show(void)
{
	float temp;
	int adcx;
	
	
	if(recv_flag)
	{
		frequency=temp=1000000.0/Frequency_REF;
		adcx=temp;
		LCD_ShowxNum(172,50,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(204,50,temp,3,16,0x80);
		resend();
		
		temp=1000000.0/Frequency_F;
		adcx=temp;
		LCD_ShowxNum(172,70,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(204,70,temp,3,16,0x80);
		
		temp=(double)Period_REF/Frequency_REF;
		adcx=temp;
		LCD_ShowxNum(172,90,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(204,90,temp,3,16,0x80);
		
		temp=(double)Period_F/Frequency_F;
		adcx=temp;
		LCD_ShowxNum(172,110,adcx,3,16,0);
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(204,110,temp,3,16,0x80);
		
		recv_flag=0;
	}
}
