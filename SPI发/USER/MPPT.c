#include "MPPT.h"

void MPPT_Init(MPPT_TypeDef *MPPT)
{
	MPPT->last_power=0;
	MPPT->this_power=0;
	MPPT->last_rate=0;
	MPPT->this_rate=0;
	MPPT->step=0.0005;
	MPPT->min_rate=0;
	MPPT->max_rate=0.99999;
}


void MPPT_Caculate(MPPT_TypeDef *MPPT)
{
	if(MPPT->this_power>MPPT->last_power)
	{
		if(MPPT->this_rate>MPPT->last_rate)
			MPPT->next_rate=MPPT->this_rate+MPPT->step;
		else
			MPPT->next_rate=MPPT->this_rate-MPPT->step;
	}
	else if(MPPT->this_power<MPPT->last_power)
	{
		if(MPPT->this_rate>MPPT->last_rate)
			MPPT->next_rate=MPPT->this_rate-MPPT->step;
		else
			MPPT->next_rate=MPPT->this_rate+MPPT->step;
	}
	
	if(MPPT->next_rate>MPPT->max_rate)
		MPPT->next_rate=MPPT->max_rate;
	else if(MPPT->next_rate<MPPT->min_rate)
		MPPT->next_rate=MPPT->min_rate;
	
	MPPT->last_power=MPPT->this_power;
	MPPT->last_rate=MPPT->this_rate;
	MPPT->this_rate=MPPT->next_rate;
}
