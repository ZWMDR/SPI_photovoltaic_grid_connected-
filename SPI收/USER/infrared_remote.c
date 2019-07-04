#include "infrared_remote.h"

u16 zoom[3]={0,10,100};
u16 zooms[5]={1,10,100,1000,10000};

void IR_Init(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef        TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //??PORTB?? 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	//TIM5 ???? 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1 ?? 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//???? 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_1);	//???GPIOA1
	
 	TIM_TimeBaseStructure.TIM_Period = 10000; //?????????? ??10ms??  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//????,1M?????,1us?1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //??????????TIMx

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // ????? IC2???TI5?
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//?????
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //??????,??? 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ??????? 8??????????
  TIM_ICInit(TIM5, &TIM_ICInitStructure);//????????????

  TIM_Cmd(TIM5,ENABLE ); 	//?????5
 
 						
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //??NVIC_InitStruct???????????NVIC???	
 
 	TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//?????? ,??CC2IE????
}

u8 	RmtSta=0;	  	  
u16 Dval;		//?????????
u32 RmtRec=0;	//????????
u8  RmtCnt=0;

void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)//??????????
		{	
			RmtSta&=~0X10;						//????????????
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//?????????????????
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//??????
				RmtSta&=0XF0;	//?????	
			}						 	   	
		}							    
	}
 	if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA)//?????
		{

			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ????????				
	    	TIM_SetCounter(TIM5,0);	   	//??????
			RmtSta|=0X10;					//??????????
		}else //?????
		{			
  			 Dval=TIM_GetCapture2(TIM5);//??CCR1????CC1IF???
			 TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC4P=0	????????
 			
			if(RmtSta&0X10)					//????????? 
			{
 				if(RmtSta&0X80)//???????
				{
					
					if(Dval>300&&Dval<800)			//560????,560us
					{
						RmtRec<<=1;	//????.
						RmtRec|=0;	//???0	   
					}else if(Dval>1400&&Dval<1800)	//1680????,1680us
					{
						RmtRec<<=1;	//????.
						RmtRec|=1;	//???1
					}else if(Dval>2200&&Dval<2600)	//??????????? 2500????2.5ms
					{
						RmtCnt++; 		//??????1?
						RmtSta&=0XF0;	//?????		
					}
 				}else if(Dval>4200&&Dval<4700)		//4500????4.5ms
				{
					RmtSta|=1<<7;	//???????????
					RmtCnt=0;		//?????????
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
 TIM_ClearFlag(TIM5,TIM_IT_Update|TIM_IT_CC2);
}

u8 Remote_Scan(void)
{
	u8 sta=0;
    u8 t1,t2;  
	if(RmtSta&(1<<6))//????????????
	{ 
	    t1=RmtRec>>24;			//?????
	    t2=(RmtRec>>16)&0xff;	//?????? 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//???????(ID)??? 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)sta=t1;//????
		}
		if((sta==0)||((RmtSta&0X80)==0))//??????/?????????
		{
		 	RmtSta&=~(1<<6);//???????????
			RmtCnt=0;		//?????????
		}
		RmtSta=0,RmtRec=0,RmtCnt=0;
	}
    return sta;
}

u8 IR_read(u8* IR_instruct)
{
	u8 IR_key=Remote_Scan();
	switch(IR_key)
	{
		case 162://POWER
		{
			*IR_instruct=10;
			return 1;
		}
		case 98://UP
		{
			*IR_instruct=11;
			return 1;
		}
		case 2://PLAY
		{
			*IR_instruct=15;
			return 1;
		}
		case 226://ALIENTEK
		{
			*IR_instruct=23;
			return 1;
		}
		case 194://RIGHT
		{
			*IR_instruct=14;
			return 1;
		}
		case 34://LEFT
		{
			*IR_instruct=13;
			return 1;
		}
		case 224://VOL-
		{
			*IR_instruct=22;
			return 1;
		}
		case 168://DOWN
		{
			*IR_instruct=12;
			return 1;
		}
		case 144://VOL+
		{
			*IR_instruct=21;
			return 1;
		}
		case 104://1
		{
			*IR_instruct=1;
			return 1;
		}
		case 152://2
		{
			*IR_instruct=2;
			return 1;
		}
		case 176://3
		{
			*IR_instruct=3;
			return 1;
		}
		case 48://4
		{
			*IR_instruct=4;
			return 1;
		}
		case 24://5
		{
			*IR_instruct=5;
			return 1;
		}
		case 122://6
		{
			*IR_instruct=6;
			return 1;
		}
		case 16://7
		{
			*IR_instruct=7;
			return 1;
		}
		case 56://8
		{
			*IR_instruct=8;
			return 1;
		}
		case 90://9
		{
			*IR_instruct=9;
			return 1;
		}
		case 66://0
		{
			*IR_instruct=0;
			return 1;
		}
		case 82://DELETE
		{
			*IR_instruct=16;
			return 1;
		}
		default://ERROR
			*IR_instruct=0;
			return 0;
	}
}

/*------------------
IR_instructs:
0: 数字0
1: 数字1
2: 数字2
3: 数字3
4: 数字4
5: 数字5
6: 数字6
7: 数字7
8: 数字8
9: 数字9

10：POWER
11：上
12：下
13：左
14：右
15：确定（play）
16：DELETE

21：VOL+
22：VOL-
23：ALIENTEK
-------------------*/


void IR_Menu_Promote_upgrade(GUI_Menu_InitTypeDef* Menu,IR_Menu_InitTypeDef* IR_Menu,u8 up_down)//菜单等级更改
//up_down>0:升级；up_down=0:降级
{
	if(up_down && IR_Menu->level<IR_Menu->max_level)//升级
	{
		//晋升规则
		if(IR_Menu->promote_rule==0)
		{
			IR_Menu->status*=10;
			IR_Menu->select_status=IR_Menu->status;
			IR_Menu->level++;
		}
		else if(IR_Menu->promote_rule==1)
		{
			IR_Menu->status=((IR_Menu->level++)-1)*10;
			IR_Menu->select_status=IR_Menu->status;
		}
	}
	else//降级
	{
		if(IR_Menu->promote_rule==0 && IR_Menu->level>1)
		{
			IR_Menu->status/=10;
			IR_Menu->select_status=IR_Menu->status;
			IR_Menu->level--;
		}
		else if(IR_Menu->promote_rule==1 && IR_Menu->level>1)
		{
			IR_Menu->status=((IR_Menu->level--)-1)*10;
			IR_Menu->select_status=IR_Menu->status;
		}
	}
}

void IR_Menu_Promote(GUI_Menu_InitTypeDef* Menu,IR_Menu_InitTypeDef* IR_Menu,u8 IR_instruct)//菜单更改
{
	if(10<IR_instruct && IR_instruct<15)
	{
		if(IR_instruct==11)//上
		{
			if(IR_Menu->select_status>IR_Menu->status+1)
				IR_Menu->select_status--;
			else
				IR_Menu->select_status=Menu->table_num;
		}
		else if(IR_instruct==12)//下
		{
			if(IR_Menu->select_status<Menu->table_num)
				IR_Menu->select_status++;
			else
				IR_Menu->select_status=1;
		}
		else if(IR_instruct==13)//左
		{
			IR_Menu_Promote_upgrade(Menu,IR_Menu,0);
		}
		else if(IR_instruct==14)//右
		{
			IR_Menu->status=IR_Menu->select_status;
			IR_Menu_Promote_upgrade(Menu,IR_Menu,1);
		}
	}
	else if(IR_instruct==15)//确定
	{
		IR_Menu->status=IR_Menu->select_status;
		IR_Menu_Promote_upgrade(Menu,IR_Menu,1);
	}
	else if(IR_instruct==10)//返回初始菜单(POWER)
	{
		IR_Menu->select_status=IR_Menu->status=0;
		IR_Menu->level=1;
	}
	
}

void IR_input(IR_IB_Msg* IB_Msg,u8 IR_instruct)
{
	if(IR_instruct<10 || IB_Msg->select==0)
	{
		IB_Msg->GUI_IB->digits[IB_Msg->select_status]=IR_instruct;
		if(IB_Msg->select_status<IB_Msg->GUI_IB->digit_fromer+IB_Msg->GUI_IB->digit_later)
			IB_Msg->select_status++;
	}
	else if(IR_instruct==15)//确定
	{
		IB_Msg->select=1;
		IB_Msg->GUI_IB->number=IR_IB_number_assign(IB_Msg);
	}
	else if(IR_instruct==13)//左
	{
		IB_Msg->select=0;
		if(IB_Msg->select_status>0)
			IB_Msg->select_status--;
		else
			IB_Msg->select_status=IB_Msg->GUI_IB->digit_fromer+IB_Msg->GUI_IB->digit_later-1;
	}
	else if(IR_instruct==14)//右
	{
		IB_Msg->select=0;
		if(IB_Msg->select_status<IB_Msg->GUI_IB->digit_fromer+IB_Msg->GUI_IB->digit_later)
			IB_Msg->select_status++;
		else
			IB_Msg->select_status=0;
	}
	
}

u8 IR_Menu_selet(GUI_Menu_InitTypeDef* Menu,IR_Menu_InitTypeDef* IR_Menu)//菜单
{
	u8 IR_instruct;
	u8 IR_key=IR_read(&IR_instruct);//指令读取
	if(!IR_key)
		return 0;
	
	IR_Menu_Promote(Menu,IR_Menu,IR_instruct);//指令处理
	return 1;
}

u8 IR_Input_Box_select(IR_IB_Msg* IB_Msg)
{
	u8 IR_instruct;
	u8 IR_key=IR_read(&IR_instruct);//指令读取
	if(!IR_key)
		return 0;
	
	IR_input(IB_Msg,IR_instruct);//指令处理
	return 1;
}

float IR_IB_number_assign(IR_IB_Msg* IB_Msg)
{
	float decimals=0;
	u16 integer=0;
	u8 i;
	u8 count=IB_Msg->GUI_IB->digit_fromer-1;
	
	for(i=0;i<IB_Msg->GUI_IB->digit_later;i++)
	{
		decimals+=IB_Msg->GUI_IB->digits[i+IB_Msg->GUI_IB->digit_fromer-1]*pow(0.1,i+1);
	}
	for(i=0;i<IB_Msg->GUI_IB->digit_fromer;i++)
	{
		integer+=IB_Msg->GUI_IB->digits[i]*zooms[count--];
	}
	return decimals+integer;
}

