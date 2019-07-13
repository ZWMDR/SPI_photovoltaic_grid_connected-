#include "remote.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//红外遥控初始化
//设置IO以及定时器4的输入捕获

void Remote_Init(void)    			  
{  

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	//TIM5 时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1 输入 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_1);	//初始化GPIOA1
	
						  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//预分频器,1M的计数频率,1us加1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM5, &TIM_ICInitStructure);//初始化定时器输入捕获通道

  TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
 
 						
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
 
 	TIM_ITConfig( TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC2IE捕获中断	
	IR_instruct=0;
	IR_flag=0;
	IR_key=0;
}

 
//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta=0;	  	  
u16 Dval;		//下降沿时计数器的值
u32 RmtRec=0;	//红外接收到的数据
u8  RmtCnt=0;	//按键按下的次数
//定时器5中断服务程序
void TIM5_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)//上次有数据被接收到了
		{	
			RmtSta&=~0X10;						//取消上升沿已经被捕获标记
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//标记已经完成一次按键的键值信息采集
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//清空引导标识
				RmtSta&=0XF0;	//清空计数器	
			}						 	   	
		}							    
	}
 	if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA)//上升沿捕获
		{

			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    	TIM_SetCounter(TIM5,0);	   	//清空定时器值
			RmtSta|=0X10;					//标记上升沿已经被捕获
		}else //下降沿捕获
		{			
  			 Dval=TIM_GetCapture2(TIM5);//读取CCR1也可以清CC1IF标志位
			 TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
			if(RmtSta&0X10)					//完成一次高电平捕获 
			{
 				if(RmtSta&0X80)//接收到了引导码
				{
					
					if(Dval>300&&Dval<800)			//560为标准值,560us
					{
						RmtRec<<=1;	//左移一位.
						RmtRec|=0;	//接收到0	   
					}else if(Dval>1400&&Dval<1800)	//1680为标准值,1680us
					{
						RmtRec<<=1;	//左移一位.
						RmtRec|=1;	//接收到1
					}else if(Dval>2200&&Dval<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt++; 		//按键次数增加1次
						RmtSta&=0XF0;	//清空计时器		
					}
 				}else if(Dval>4200&&Dval<4700)		//4500为标准值4.5ms
				{
					RmtSta|=1<<7;	//标记成功接收到了引导码
					RmtCnt=0;		//清除按键次数计数器
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
 TIM_ClearFlag(TIM5,TIM_IT_Update|TIM_IT_CC2);
}

//处理红外键盘
//返回值:
//	 0,没有任何按键按下
//其他,按下的按键键值.
u8 Remote_Scan(void)
{
	u8 sta=0;
    u8 t1,t2;  
	if(RmtSta&(1<<6))//得到一个按键的所有信息了
	{ 
	    t1=RmtRec>>24;			//得到地址码
	    t2=(RmtRec>>16)&0xff;	//得到地址反码 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//检验遥控识别码(ID)及地址 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)sta=t1;//键值正确
		}
		if((sta==0)||((RmtSta&0X80)==0))//按键数据错误/遥控已经没有按下了
		{
		 	RmtSta&=~(1<<6);//清除接收到有效按键标识
			RmtCnt=0;		//清除按键次数计数器
		}
		RmtSta=0,RmtRec=0,RmtCnt=0;
	}
    return sta;
}

void remote_key(void)
{
	IR_key=Remote_Scan();
	if(IR_key)
	{
		switch(IR_key)
		{
			case 226:IR_instruct=16,IR_flag=1;break;  //back to menu(ALIENTEK)
			case 194:IR_instruct=14,IR_flag=1;break;	//right
			case 34:IR_instruct=13,IR_flag=1;break;		//left
			case 2:IR_instruct=15,IR_flag=1;break;		//play
			case 98:IR_instruct=11,IR_flag=1;break;   //up
			case 168:IR_instruct=12,IR_flag=1;break;  //down
			case 162:IR_instruct=10,IR_flag=1;break;  //back to first menu(power)
			case 104:IR_instruct=1,IR_flag=1;break;
			case 152:IR_instruct=2,IR_flag=1;break;
			case 176:IR_instruct=3,IR_flag=1;break;
			case 48:IR_instruct=4,IR_flag=1;break;
			case 24:IR_instruct=5,IR_flag=1;break;
			case 122:IR_instruct=6,IR_flag=1;break;
			case 16:IR_instruct=7,IR_flag=1;break;
			case 56:IR_instruct=8,IR_flag=1;break;
			case 90:IR_instruct=9,IR_flag=1;break;
			case 66:IR_instruct=0,IR_flag=1;break;
			default:IR_flag=0;
		}
		IR_key=0;
	}
	
	if(IR_flag)
	{
		switch(IR_instruct)
		{
			case 10:
				menu_status=status=0;
				key_flag=1;
				assign_flag=1;
				break;
			case 11://上
				if(menu_status==0)
				{
					if(status>0)
						status--;
					else
						status=3;
					
					key_flag=1;
				}
				break;
			case 12://下
				if(menu_status==0)
				{
					if(status<3)
						status++;
					else
						status=1;
					
					key_flag=1;
				}
				break;
			case 13://左
				if(menu_status>=10 && menu_status<20)
				{
					
				}
				else if(menu_status>19 && menu_status<30)
				{
					status=status>21?status-1:24;
					key_flag=1;
				}
				break;
			case 14://右
				if(menu_status>19 && menu_status<30)
				{
					status=status<24?status+1:21;
					key_flag=1;
				}
				break;
			case 15://确定
				if(menu_status==0)
				{
					key_flag=1;
					if(status==1)
					{
						menu_status=10;
						status=10;
						assign_flag=1;
					}
					else if(status==2)
					{
						menu_status=20;
						status=20;
					}
					else if(status==3)
					{
						menu_status=30;
						status=30;
					}
				}
				else if(menu_status>=10 && menu_status<20)
				{
					Set_Voltage=InputBox_assign_u16(digits_MPPT,4);
					if(Set_Voltage>=4000)
					{
						Set_Voltage=3999;
						digits[0]=3;
						digits[1]=digits[2]=digits[3]=9;
					}
					key_flag=1;
					assign_flag=1;
				}
				else if(menu_status>=20 && menu_status<30)
				{
					status=menu_status;
					Set_Voltage=InputBox_assign_u16(digits,4);
					if(Set_Voltage>=6000)
					{
						Set_Voltage=5999;
						digits[0]=5;
						digits[1]=digits[2]=digits[3]=9;
					}
					key_flag=1;
					assign_flag=1;
				}
				
				break;
			
			case 16://返回
				if(menu_status>=10 && menu_status<40)
				{
					status=menu_status/10;
					menu_status=0;
					assign_flag=1;
					key_flag=1;
				}
				break;
			
			default://数字
				if(menu_status>=10 && menu_status<20)
				{
					
				}
				else if(menu_status>=20 && menu_status<30)
				{
					if(status==20)
						status++;

					digits[status-21]=IR_instruct;
					status=(status<24)?status+1:20;
					key_flag=1;
				}
		}
		IR_flag=0;
	}
}

