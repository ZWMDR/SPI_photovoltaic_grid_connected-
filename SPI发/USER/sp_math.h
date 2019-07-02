#ifndef __SP_MATH_H
#define __SP_MATH_H
#include "math.h"
#include "sys.h"

u16 get_vcc(u16 *arr,u16 len,u16 start,u16 gap,float scale_rate);
void get_vcc_duplex(u16 *arr,u16 len,u16 *VCC1,u16* VCC2);


#endif
