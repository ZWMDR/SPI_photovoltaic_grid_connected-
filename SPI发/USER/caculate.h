#ifndef _CACULATE_H
#define _CACULATE_H
#include "adc.h"


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

#endif
