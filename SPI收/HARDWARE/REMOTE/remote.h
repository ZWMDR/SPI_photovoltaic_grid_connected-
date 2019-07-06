#ifndef __RED_H
#define __RED_H 
#include "sys.h"  
#include "led.h"
#include "sp_math.h"
  
#define RDATA 	PAin(1)	 	//红外数据输入脚

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;			//按键按下的次数
extern u8 IR_flag;
extern u8 key_flag;
extern u8 status;
extern u8 menu_status;
extern u8 IR_instruct;
extern u8 IR_key;
extern u8 assign_flag;
extern u8 digits[4];
extern u16 Set_Voltage;


void Remote_Init(void);    	//红外传感器接收头引脚初始化
u8 Remote_Scan(void);	 
void remote_key(void);
#endif

