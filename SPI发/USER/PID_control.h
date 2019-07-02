#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H

#include "sys.h"

typedef struct PID
{
    s16 SetPoint;       //Desired Value
    float Proportion;   //Proportional Const
    float Integral;     //Integral Const
    float Derivative;   //Derivative Const
    s16 LastError;      //Error[-1]
    s16 PrevError;      //Error[-2]
}PID;

void PID_Init(PID* pid,float P,float I,float D,u16 set_point);
s16 get_iIncpid(PID* pid,u16 set_point,u16 this_point);
void PID_error_clear(PID* pid);
void PID_reset_point(PID* pid,u16 set_point);

#endif
