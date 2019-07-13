#ifndef __RELAY_MODULE
#define __RELAY_MODULE
#include "stm32f10x_gpio.h"

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t Pin;
	u8 mode;
	
}relay_module_InitTypeDef;

void relay_module_Init(relay_module_InitTypeDef* module);
void relay_module_Open(relay_module_InitTypeDef* module);
void relay_module_Close(relay_module_InitTypeDef* module);
/*--------------------
mode:
0: 低电平有效
1: 高电平有效
---------------------*/

#endif
