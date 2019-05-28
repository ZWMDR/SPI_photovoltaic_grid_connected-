#ifndef _CACULATE_H
#define _CACULATE_H
#include "adc.h"

typedef struct PID
{
    s16 SetPoint; //设定目标 Desired Value
    float Proportion; //比例常数 Proportional Const
    float Integral; //积分常数 Integral Const
    float Derivative; //微分常数 Derivative Const
    s16 LastError; //Error[-1]
    s16 PrevError; //Error[-2]
}PID;

void get_VCC(u16 *arr,u16 len,u16 *VCC1,u16* VCC2)
{
	u16 max1=0x0000;
	u16 min1=0xFFFF;
	u16 max2=0x0000;
	u16 min2=0xFFFF;
	u16 i;
	for(i=0;i<len;i+=2)
	{
		if(max1<arr[i])
			max1=arr[i];
		else if(min1>arr[i])
			min1=arr[i];
		
		if(max2<arr[i+1])
			max2=arr[i+1];
		else if(min2>arr[i+1])
			min2=arr[i+1];
	}
	*VCC1=max1-min1;
	*VCC2=max2-min2;
}

void PID_init(PID* pid,float P,float I,float D)
{
	pid->LastError=0;
	pid->PrevError=0;
	pid->Proportion=P;
	pid->Integral=I;
	pid->Derivative=D;
	pid->SetPoint=0x0FFF;
}
s16 get_iIncpid(PID* pid,u16 set_point,u16 this_point)
{
	s16 iError, iIncpid; 
	pid->SetPoint=set_point;
	iError=pid->SetPoint-this_point;
	iIncpid=pid->Proportion*iError
	-pid->Integral*pid->LastError
	+pid->Derivative*pid->PrevError;
	pid->PrevError=pid->LastError;
	pid->LastError=iError;
	
	return iIncpid;
}

#endif
