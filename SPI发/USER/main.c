#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "PWM_input_capture.h"
#include "PID_control.h"
#include "SPWM_output.h"
#include "ADC_input.h"
#include "sp_math.h"
#include "MPPT.h"
#include "relay_module.h"
#include "NRF24L01_module.h"

const s16 Sin[400]={0,56,113,169,226,282,338,395,451,507,563,618,674,730,785,840,895,949,1004,1058,1112,1166,1219,1272,1325,1377,1429,1481,1532,1583,1634,1684,1734,1783,1832,1880,1928,1976,2023,2070,2116,2161,2206,2250,2294,2338,2380,2422,2464,2505,2545,2585,2624,2662,2700,2737,2773,2809,2844,2878,2912,2945,2977,3008,3039,3069,3098,3127,3154,3181,3207,3232,3257,3281,3303,3325,3347,3367,3387,3405,3423,3440,3457,3472,3486,3500,3513,3525,3536,3546,3555,3564,3571,3578,3584,3588,3592,3596,3598,3599,3600,3599,3598,3596,3592,3588,3584,3578,3571,3564,3555,3546,3536,3525,3513,3500,3486,3472,3457,3440,3423,3405,3387,3367,3347,3325,3303,3281,3257,3232,3207,3181,3154,3127,3098,3069,3039,3008,2977,2945,2912,2878,2844,2809,2773,2737,2700,2662,2624,2585,2545,2505,2464,2422,2380,2338,2294,2250,2206,2161,2116,2070,2023,1976,1928,1880,1832,1783,1734,1684,1634,1583,1532,1481,1429,1377,1325,1272,1219,1166,1112,1058,1004,949,895,840,785,730,674,618,563,507,451,395,338,282,226,169,113,56,0,-56,-113,-169,-226,-282,-338,-395,-451,-507,-563,-618,-674,-730,-785,-840,-895,-949,-1004,-1058,-1112,-1166,-1219,-1272,-1325,-1377,-1429,-1481,-1532,-1583,-1634,-1684,-1734,-1783,-1832,-1880,-1928,-1976,-2023,-2070,-2116,-2161,-2206,-2250,-2294,-2338,-2380,-2422,-2464,-2505,-2545,-2585,-2624,-2662,-2700,-2737,-2773,-2809,-2844,-2878,-2912,-2945,-2977,-3008,-3039,-3069,-3098,-3127,-3154,-3181,-3207,-3232,-3257,-3281,-3303,-3325,-3347,-3367,-3387,-3405,-3423,-3440,-3457,-3472,-3486,-3500,-3513,-3525,-3536,-3546,-3555,-3564,-3571,-3578,-3584,-3588,-3592,-3596,-3598,-3599,-3600,-3599,-3598,-3596,-3592,-3588,-3584,-3578,-3571,-3564,-3555,-3546,-3536,-3525,-3513,-3500,-3486,-3472,-3457,-3440,-3423,-3405,-3387,-3367,-3347,-3325,-3303,-3281,-3257,-3232,-3207,-3181,-3154,-3127,-3098,-3069,-3039,-3008,-2977,-2945,-2912,-2878,-2844,-2809,-2773,-2737,-2700,-2662,-2624,-2585,-2545,-2505,-2464,-2422,-2380,-2338,-2294,-2250,-2206,-2161,-2116,-2070,-2023,-1976,-1928,-1880,-1832,-1783,-1734,-1684,-1634,-1583,-1532,-1481,-1429,-1377,-1325,-1272,-1219,-1166,-1112,-1058,-1004,-949,-895,-840,-785,-730,-674,-618,-563,-507,-451,-395,-338,-282,-226,-169,-113,-56};

u8 DMA_SPI_buff_TX[DMA_SPI_buff_len];
u16 SPI_send_buff[SPI_send_buff_len];
u16 SPI_recv_buff[SPI_send_buff_len];
u8 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u16 DMA_buff[DMA_buff_len_ADC1];
u16 DMA_buff_2[DMA_BUFF_LEN_ADC_2];
u8 flag_REF;
u8 flag_F;
u8 recv_flag;
u8 send_flag;
u8 capture_flag;
u8 ADC_flag;
u8 NRF_status;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 Current;
u16 t;

float voltage_scale_rate;
float voltage;
float current;
u8 working_mode;
u16 target;
/* ----------------------
	working mode:
	1: 幅度跟踪模式
	2: 限幅模式
	3: 限流模式
---------------------- */


int main(void)
{
	u16 voltage_F;//ADC采样值
	u8 exception_flag;//异常状态标志
	PID vcc_PID,phase_PID,frequency_PID;
	float delta_rate;
	//float delta_phase;
	u16 PID_step;
	SPWM_InitTypeDef spwm_init_typedef;
	PWM_capture_InitTypeDef init_typedef1,init_typedef2;
	ADC_cs_InitTypeDef ADC_cs;
	ADC_Channel channels[2];
	MPPT_TypeDef MPPT;
	relay_module_InitTypeDef relay_module;
	relay_module_InitTypeDef buzzer;
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//uart_init(19200);
	LED_Init();		  		 //初始化与LED连接的硬件接口
	delay_ms(200);
	LED0=0;
	
	//输入捕获初始化
	init_typedef1.TIMx=TIM5;
	init_typedef1.RCC_APB1Periph_TIMx=RCC_APB1Periph_TIM5;
	init_typedef1.GPIOx=GPIOA;
	init_typedef1.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOA;
	init_typedef1.Pin=GPIO_Pin_0;
	
	init_typedef2.TIMx=TIM4;
	init_typedef2.RCC_APB1Periph_TIMx=RCC_APB1Periph_TIM4;
	init_typedef2.GPIOx=GPIOB;
	init_typedef2.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOB;
	init_typedef2.Pin=GPIO_Pin_6;
	
	PWM_input_capture_Init(&init_typedef1,&init_typedef2,0xFFFF-1,36-1);
	MY_NVIC_Init(3,3,TIM4_IRQn,2);
	MY_NVIC_Init(3,3,TIM5_IRQn,2);
	
	delay_ms(200);
	LED0=~LED0;
	
	//正弦调制输出初始化
	spwm_init_typedef.TIMx=TIM8;
	spwm_init_typedef.RCC_APB2Periph_TIMx=RCC_APB2Periph_TIM8;
	spwm_init_typedef.GPIOx=GPIOC;
	spwm_init_typedef.Pin=GPIO_Pin_8|GPIO_Pin_7;
	spwm_init_typedef.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
	SPWM_output_Init(&spwm_init_typedef,3600-1,1-1);//20kHz

	delay_ms(200);
	LED0=~LED0;
	
	//ADC采样初始化
	ADC_cs.channel_num=2;//通道数
	ADC_cs.Channels=channels;
	ADC_cs.ScanConvMode=1;//开启扫描模式
	ADC_cs.arr=800-1;
	ADC_cs.psc=72-1;
	//通道1（连续转换只能使用ADC1）
	ADC_cs.Channels[0].num=1;
	ADC_cs.Channels[0].ADC_Channel=ADC_Channel_2;
	ADC_cs.Channels[0].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[0].Pin=GPIO_Pin_2;
	//通道2
	ADC_cs.Channels[1].num=2;
	ADC_cs.Channels[1].ADC_Channel=ADC_Channel_4;
	ADC_cs.Channels[1].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[1].Pin=GPIO_Pin_4;
	
	//ADC1_continuous_sampling_Init(&ADC_cs,DMA_buff,DMA_buff_len_ADC1);
	
	delay_ms(200);
	LED0=~LED0;
	
	//PID调控初始化
	PID_Init(&vcc_PID,0.12,0.1,0.05,0x0FFF);
	PID_Init(&phase_PID,0.2,0.08,0.06,0x0000);
	PID_Init(&frequency_PID,0.3,0.16,0.09,25000);
	
	delay_ms(200);
	LED0=~LED0;
	
	//SPI初始化
	//SPI1_DMA1_Init(1000-1,7200-1,1,1);
	NRF24L01_Init(1,1000-1,7200-1);
	
	//MPPT初始化
	MPPT_Init(&MPPT);
	
	//继电器初始化
	relay_module.GPIOx=GPIOB;
	relay_module.Pin=GPIO_Pin_0;
	relay_module.mode=0;
	relay_module_Init(&relay_module);
	
	//蜂鸣器初始化
	buzzer.GPIOx=GPIOB;
	buzzer.Pin=GPIO_Pin_1;
	buzzer.mode=1;
	relay_module_Init(&buzzer);
	
	delay_ms(200);
	LED0=~LED0;
	LED0=1;
	
	t=0;
	working_mode=0;
	send_flag=0;
	recv_flag=0;
	capture_flag=0;
	ADC_flag=0;
	voltage_scale_rate=0;
	flag_REF=flag_F=0;
	Period_REF=Period_F=0;
	Frequency_REF=Frequency_F=25000;
	ADC1_continuous_sampling_enable();

	
	while(1)
	{
		if(ADC_flag)
		{
			LED0=~LED0;
			ADC_flag=0;
		}
		if(exception_flag==0)
		{
			if(flag_REF)//频率跟踪
			{
				if(SPI_send_buff[1]<50000 && SPI_send_buff[1]>33333)
				{
					TIM6->ARR=Frequency_REF*0.09-1;
				}
				
				flag_REF=0;
			}
			/*
			if(capture_flag)//相位跟踪
			{
				delta_phase=((float)(Period_F<(Frequency_REF>>1)?Period_F:Period_F-Frequency_REF)/Frequency_REF)*1000;
				t=(t+400+get_iIncpid(&phase_PID,0,delta_phase))%400;
				capture_flag=0;
			}
			*/
			
			//输出模式
			if(working_mode==0)//静态模式
			{
				relay_module_Close(&relay_module);
				voltage_scale_rate=0.1;
				delta_rate=0;
				PID_step=0;
			}
			else if(working_mode==1)//MPPT模式
			{
				if(ADC_flag)
				{
					get_average_duplex(DMA_buff,DMA_buff_len_ADC1,&voltage_F,&Current);
					voltage=voltage_F*0.012687623;
					current=Current*0.002685546875;

					MPPT.this_power=current*voltage;
					MPPT_Caculate(&MPPT);
					voltage_scale_rate=MPPT.next_rate;
					
					SPI_send_buff[3]=voltage_F*1.2687623;
					SPI_send_buff[4]=current*50;
					ADC_flag=0;
					
					ADC1_continuous_sampling_enable();
				}
			}
			else if(working_mode==2)//稳压模式
			{
				relay_module_Open(&relay_module);
				if(ADC_flag)
				{
					get_average_duplex(DMA_buff,DMA_buff_len_ADC1,&voltage_F,&Current);
					
					if(Current*0.7461>1500)//1.5A过流保护
					{
						relay_module_Close(&relay_module);//关断输出
						relay_module_Open(&buzzer);//蜂鸣器警报
						SPI_send_buff[5]=0xAAAA;
						exception_flag=1;
					}
					else
					{
						relay_module_Close(&buzzer);
					}
					
					delta_rate=get_iIncpid(&vcc_PID,target*0.767689697,voltage_F)*0.00024414;
					
					if(fabs(delta_rate)<0.0005)
					{
						if(PID_step<10)
							PID_step++;
					}
					else
						PID_step=0;
					
					if(PID_step<10)
					{
						voltage_scale_rate+=delta_rate;
						
						if(voltage_scale_rate>1)
							voltage_scale_rate=1;
						else if(voltage_scale_rate<0)
							voltage_scale_rate=0;
					}
					
					SPI_send_buff[3]=voltage_F*1.2687623;
					SPI_send_buff[4]=Current;
					
					ADC_flag=0;
					ADC1_continuous_sampling_enable();
				}
			}
			else if(working_mode==3)//稳流模式
			{
				if(recv_flag)
				{
					recv_flag=0;
				}
			}
			else
				working_mode=0;
		}
		
		else//异常状态处理
		{
			
		}
	}
}
