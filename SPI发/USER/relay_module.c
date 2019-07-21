#include "relay_module.h"

void relay_module_Init(relay_module_InitTypeDef* module)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(module->RCC_APB2Periph_GPIOx,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=module->Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(module->GPIOx,&GPIO_InitStructure);
	
	if(module->mode==0)
		GPIO_SetBits(module->GPIOx,module->Pin);
	else
		GPIO_ResetBits(module->GPIOx,module->Pin);
	module->status=0;
}

void relay_module_Open(relay_module_InitTypeDef* module)
{
	if(module->mode==0)
		GPIO_ResetBits(module->GPIOx,module->Pin);
	else
		GPIO_SetBits(module->GPIOx,module->Pin);
	
	module->status=1;
}

void relay_module_Close(relay_module_InitTypeDef* module)
{
	if(module->mode==0)
		GPIO_SetBits(module->GPIOx,module->Pin);
	else
		GPIO_ResetBits(module->GPIOx,module->Pin);
	module->status=0;
}
