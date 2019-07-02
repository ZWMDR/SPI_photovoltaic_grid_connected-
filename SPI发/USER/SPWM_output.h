#ifndef __SPWM_OUTPUT_H
#define __SPWM_OUTPUT_H

#include "sys.h"
#include "stm32f10x_adc.h"

#define SINTABLE_LEN 400

extern const s16 Sin[SINTABLE_LEN];
extern float voltage_scale_rate;
extern u16 t;

typedef struct SPWM_output_struct
{
	TIM_TypeDef* TIMx;
	uint32_t RCC_APB2Periph_TIMx;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint32_t RCC_APB2Periph_GPIOx;
	
}SPWM_InitTypeDef;

void SPWM_output_Init(SPWM_InitTypeDef *spwm_init_typedef,uint16_t arr,uint16_t psc);
void PWM_Set_duty(float rate,u16 *t,u8 mode);
//mode=0,单极性SPWM输出; mode=1，单极性SPWM输出（磨平衡） ；mode=2，双极性输出

#endif
