#ifndef __SP_MATH_H
#define __SP_MATH_H
#include "math.h"
#include "sys.h"

float abs_f(float num);//取绝对值
u16 get_vcc(const u16 *arr,u16 len,u16 start,u16 gap,float scale_rate);
//获取一组数据中的峰峰值
void get_vcc_duplex(const u16 *arr,u16 len,u16 *VCC1,u16* VCC2);//获取两路峰峰值
float find_zero_cur(const u16 *arr,u16 average,u16 len);//找到正弦波零点(斜率大于0)，手动输入均值

double RMS_sinusoid(const u16 *arr,u16 len,u16 average,float rate);
double active_power(const u16 *voltage_arr,const u16 *current_arr,u16 len,u16 voltage_ave,u16 current_ave,float voltage_rate,float current_rate);//有功功率计算
double apparent_power(const u16 *voltage_arr,const u16 *current_arr,u16 len,u16 voltage_ave,
u16 current_ave,float voltage_rate,float current_rate);//计算视在功率

float InputBox_assign_float(u8* arr,u16 num_former,u16 num_latter);
u16 InputBox_assign_u16(u8* arr,u16 len);

//数组初始化

void array_init_u8(u8* arr,u16 len);
void array_init_u16(u16* arr,u16 len);

#endif
