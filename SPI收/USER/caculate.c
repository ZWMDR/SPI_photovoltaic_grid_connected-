#include "caculate.h"

void PID_init(void)
{
	//频率调节PID参数
	PID_frequency.LastError=0;
	PID_frequency.PrevError=0;
	PID_frequency.Proportion=0.18;
	PID_frequency.Integral=0.09;
	PID_frequency.Derivative=0.015;
	PID_frequency.SetPoint=3200;
	
	//相位调节PID参数
	PID_phase.LastError=0;
	PID_phase.PrevError=0;
	PID_phase.Proportion=0.18;
	PID_phase.Integral=0.09;
	PID_phase.Derivative=0.015;
	PID_phase.SetPoint=0;
	
	//电压调节PID参数
	PID_VCC.LastError=0;
	PID_VCC.PrevError=0;
	PID_VCC.Proportion=0.25;
	PID_VCC.Integral=0.09;
	PID_VCC.Derivative=0.015;
	PID_VCC.SetPoint=4096;
}

u16 get_frequency(float *frequency_REF,float *frequency_F)
{
	int iError, iIncpid;                                   //当前误差
	
	*frequency_REF=1000000.0/Frequency_REF;
	*frequency_F=1000000.0/Frequency_F;
	
	PID_frequency.SetPoint=(*frequency_REF)*64;            //设置调整目标
	iError = PID_frequency.SetPoint-(*frequency_F)*64;     //增量计算
	iIncpid = PID_frequency.Proportion * iError            //E[k]项
	- PID_frequency.Integral * PID_frequency.LastError     //E[k－1]项
	+ PID_frequency.Derivative * PID_frequency.PrevError;  //E[k－2]项
	PID_frequency.PrevError = PID_frequency.LastError;     //存储误差，用于下次计算
	PID_frequency.LastError = iError;
	
	return (u16)(180000/(*frequency_F+(iIncpid>>6)));
}

u16 get_phase(float *phase)
{
	int iError, iIncpid;                                    //当前误差
	
	*phase=(float)(Period_F-Period_REF)/Frequency_REF;
	
	iError = PID_phase.SetPoint - (*phase)*100;             //增量计算
	iIncpid = PID_phase.Proportion * iError                 //E[k]项
	- PID_phase.Integral * PID_phase.LastError              //E[k－1]项
	+ PID_phase.Derivative * PID_phase.PrevError;           //E[k－2]项
	PID_phase.PrevError = PID_phase.LastError;              //存储误差，用于下次计算
	PID_phase.LastError = iError;
	
	return (s16)(iIncpid<<2);
}

u16 get_vcc(u16 *arr,u16 count,u8 status,u16 *VCC)
{
	u16 max=0,min=0xFFFF;
	u16 i;
	for(i=status;i<count;i+=2)
	{
		if(arr[i]<min)
			min=arr[i];
		else if(arr[i]>max)
			max=arr[i];
	}
	*VCC=max-min;
	return *VCC;
}

u16 get_VCC_coef(void)
{
	int iError, iIncpid;                          //当前误差
	if(ADC_flag)
	{
		PID_VCC.SetPoint=VCC_REF;                   //设置调整目标
		iError = PID_VCC.SetPoint-VCC_F;            //增量计算
		iIncpid = PID_VCC.Proportion * iError       //E[k]项
		- PID_VCC.Integral * PID_VCC.LastError      //E[k－1]项
		+ PID_VCC.Derivative * PID_VCC.PrevError;   //E[k－2]项
		PID_VCC.PrevError = PID_VCC.LastError;      //存储误差，用于下次计算
		PID_VCC.LastError = iError;
		
		DMA_Cmd(DMA1_Channel1,ENABLE);
		TIM_Cmd(TIM2,ENABLE);
		ADC_flag=0;
		
		return (s16)((float)iIncpid/VCC_F*1000);
	}
	else
		return 0;
}
