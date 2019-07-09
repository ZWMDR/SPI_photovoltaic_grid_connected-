#ifndef __MPPT_H
#define __MPPT_H

#include "sys.h"

typedef struct
{
	float next_rate;
	float this_rate;
	float last_rate;
	float step;
	float max_rate;
	float min_rate;
	
	float last_power;
	float this_power;
	
}MPPT_TypeDef;

void MPPT_Init(MPPT_TypeDef *MPPT);
void MPPT_Caculate(MPPT_TypeDef *MPPT);

#endif
