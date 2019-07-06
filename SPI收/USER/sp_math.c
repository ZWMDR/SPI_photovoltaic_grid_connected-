#include "sp_math.h"

float abs_f(float num)
{
	return num>0?num:(-num);
}

u16 get_vcc(const u16 *arr,u16 len,u16 start,u16 gap,float scale_rate)
{
	u16 max=0;
	u16 min=0xFFFF;
	u16 i;
	for(i=start;i<len;i+=gap)
	{
		if(arr[i]>max)
			max=arr[i];
		else if(arr[i]<min)
			min=arr[i];
	}
	
	return (max-min)*scale_rate;
}

void get_vcc_duplex(const u16 *arr,u16 len,u16 *VCC1,u16* VCC2)
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

float find_zero_cur(const u16 *arr,u16 average,u16 len)
{
	u16 i;
	float delta;
	for(i=1;i<len;i++)
	{
		if(arr[i]>=average && arr[i-1]<average)
		{
			delta=1/(abs_f((arr[i]-average)/(average-arr[i-1]))+1);
			return delta+i-1;
		}
	}
	return 0;
}

double active_power(const u16 *voltage_arr,const u16 *current_arr,u16 len,u16 voltage_ave,u16 current_ave,float voltage_rate,float current_rate)
{
	u16 i;
	double sum=0;
	double voltage,current;
	
	for(i=0;i<len;i++)
	{
		voltage=(double)(voltage_arr[i]-voltage_ave)*voltage_rate;
		current=(double)(current_arr[i]-current_ave)*current_rate;
		sum+=voltage*current;
	}
	return sum;
}

double RMS_sinusoid(const u16 *arr,u16 len,u16 average,float rate)
{
	double sum;
	u16 i;
	
	for(i=0;i<len;i++)
	{
		sum+=pow((arr[i]-average)*rate,2);
	}
	sum/=len;
	return sum;
}

double apparent_power(const u16 *voltage_arr,const u16 *current_arr,u16 len,u16 voltage_ave,u16 current_ave,float voltage_rate,float current_rate)
{
	return RMS_sinusoid(voltage_arr,len,voltage_ave,voltage_rate)*RMS_sinusoid(voltage_arr,len,voltage_ave,voltage_rate);
}

float InputBox_assign_float(u8* arr,u16 num_former,u16 num_latter)
{
	float decimals=0;
	float decimal_rate=0.1;
	u16 integer_rate=1;
	u16 integer=0;
	s16 i;
	
	for(i=0;i<num_latter;i++)
	{
		decimals+=arr[num_former+i]*decimal_rate;
		decimal_rate*=0.1;
	}
	for(i=num_former-1;i>=0;i--)
	{
		integer+=arr[i]*integer_rate;
		integer_rate*=10;
	}
	return integer+decimals;
}

u16 InputBox_assign_u16(u8* arr,u16 len)
{
	u16 integer=0;
	u16 i;
	u16 rate=1;
	for(i=0;i<len;i++)
	{
		integer+=arr[len-1-i]*rate;
		rate*=10;
	}
	return integer;
}


void array_init_u8(u8* arr,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
		arr[i]=0;
}
void array_init_u16(u16* arr,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
		arr[i]=0;
}
