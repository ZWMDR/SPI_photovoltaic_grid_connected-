#ifndef _WAVE_TRACKING
#define _WAVE_TRACKING
#include "pwm.h"
#include "timer.h"
#include "sys.h"
#include "led.h"
#include "stm32f10x.h"
#include <math.h>

extern u16 Period_REF;
extern u16 Period_F;
extern u8 flag_REF;
extern u8 flag_F;
extern u16 Frequency_REF;
extern u16 Frequency_F;

void PWM_input_Init(void);

#endif
