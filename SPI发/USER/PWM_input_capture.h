#ifndef __PWM_INPUT_CAPTURE_H
#define __PWM_INPUT_CAPTURE_H
#include "stm32f10x.h"
#include "pwm.h"
#include "timer.h"
#include "sys.h"

typedef struct PWM_capture_struct
{
	TIM_TypeDef* TIMx;
	uint32_t RCC_APB1Periph_TIMx;
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint32_t RCC_APB2Periph_GPIOx;
}PWM_capture_InitTypeDef;



void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
void PWM_input_capture_Init(PWM_capture_InitTypeDef *init_typedef1,PWM_capture_InitTypeDef *init_typedef2,uint16_t arr,uint16_t psc);
//参数：定时器1，定时器2，预装载值，分频系数，PWM信号输入端口GPIO1，GPIO2，信号输入管脚Pin1，Pin2			



#endif
