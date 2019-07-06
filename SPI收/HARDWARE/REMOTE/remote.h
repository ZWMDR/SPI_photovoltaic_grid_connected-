#ifndef __RED_H
#define __RED_H 
#include "sys.h"  
#include "led.h"
#include "sp_math.h"
  
#define RDATA 	PAin(1)	 	//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;			//�������µĴ���
extern u8 IR_flag;
extern u8 key_flag;
extern u8 status;
extern u8 menu_status;
extern u8 IR_instruct;
extern u8 IR_key;
extern u8 assign_flag;
extern u8 digits[4];
extern u16 Set_Voltage;


void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	 
void remote_key(void);
#endif

