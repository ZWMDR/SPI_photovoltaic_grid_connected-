#ifndef _PWM_OUT_H
#define _PWM_OUT_H
#include "pwm.h"
#include "timer.h"
#include "led.h"
#include <math.h>

#define _PI 3.14159265358979323846
#define _2_PI 6.283185307179586476925286766559

extern u16 Sin[400];
extern u16 t;

void PWM_init(u16 arr,u16 ps);
void PWM_Set_duty(void);

#endif
