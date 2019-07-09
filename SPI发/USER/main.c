#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "SPI_DMA.h"
#include "exti.h"
#include "PWM_input_capture.h"
#include "PID_control.h"
#include "SPWM_output.h"
#include "ADC_input.h"
#include "sp_math.h"
#include "MPPT.h"



const s16 Sin[SINTABLE_LEN]={0,28,56,84,113,141,169,197,225,253,281,309,337,365,392,420,447,474,502,529,556,583,609,636,662,688,714,740,766,791,817,842,867,891,916,940,964,988,1011,1035,1058,1080,1103,1125,1147,1169,1190,1211,1232,1252,1272,1292,1312,1331,1350,1368,1386,1404,1422,1439,1456,1472,1488,1504,1519,1534,1549,1563,1577,1590,1603,1616,1628,1640,1651,1662,1673,1683,1693,1702,1711,1720,1728,1736,1743,1750,1756,1762,1768,1773,1777,1782,1785,1789,1792,1794,1796,1798,1799,1799,1800,1799,1799,1798,1796,1794,1792,1789,1785,1782,1777,1773,1768,1762,1756,1750,1743,1736,1728,1720,1711,1702,1693,1683,1673,1662,1651,1640,1628,1616,1603,1590,1577,1563,1549,1534,1519,1504,1488,1472,1456,1439,1422,1404,1386,1368,1350,1331,1312,1292,1272,1252,1232,1211,1190,1169,1147,1125,1103,1080,1058,1035,1011,988,964,940,916,891,867,842,817,791,766,740,714,688,662,636,609,583,556,529,502,474,447,420,392,365,337,309,281,253,225,197,169,141,113,84,56,28,0,-28,-56,-84,-113,-141,-169,-197,-225,-253,-281,-309,-337,-365,-392,-420,-447,-474,-502,-529,-556,-583,-609,-636,-662,-688,-714,-740,-766,-791,-817,-842,-867,-891,-916,-940,-964,-988,-1011,-1035,-1058,-1080,-1103,-1125,-1147,-1169,-1190,-1211,-1232,-1252,-1272,-1292,-1312,-1331,-1350,-1368,-1386,-1404,-1422,-1439,-1456,-1472,-1488,-1504,-1519,-1534,-1549,-1563,-1577,-1590,-1603,-1616,-1628,-1640,-1651,-1662,-1673,-1683,-1693,-1702,-1711,-1720,-1728,-1736,-1743,-1750,-1756,-1762,-1768,-1773,-1777,-1782,-1785,-1789,-1792,-1794,-1796,-1798,-1799,-1799,-1800,-1799,-1799,-1798,-1796,-1794,-1792,-1789,-1785,-1782,-1777,-1773,-1768,-1762,-1756,-1750,-1743,-1736,-1728,-1720,-1711,-1702,-1693,-1683,-1673,-1662,-1651,-1640,-1628,-1616,-1603,-1590,-1577,-1563,-1549,-1534,-1519,-1504,-1488,-1472,-1456,-1439,-1422,-1404,-1386,-1368,-1350,-1331,-1312,-1292,-1272,-1252,-1232,-1211,-1190,-1169,-1147,-1125,-1103,-1080,-1058,-1035,-1011,-988,-964,-940,-916,-891,-867,-842,-817,-791,-766,-740,-714,-688,-662,-636,-609,-583,-556,-529,-502,-474,-447,-420,-392,-365,-337,-309,-281,-253,-225,-197,-169,-141,-113,-84,-56,-28};

u16 DMA_SPI_buff_TX[DMA_SPI_buff_len];
u16 SPI_send_buff[DMA_SPI_buff_len];
u16 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u16 DMA_buff[DMA_buff_len_ADC];
u8 flag_REF;
u8 flag_F;
u8 recv_flag;
u8 send_flag;
u8 capture_flag;
u8 ADC_flag;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 Current;
u16 t;
float voltage_scale_rate;
float voltage_scale_rate_pre;
float voltage;
float current;
float power;
float power_pre;
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
	u16 voltage_REF;
	u16 voltage_F;
	PID vcc_PID,phase_PID;
	SPWM_InitTypeDef spwm_init_typedef;
	PWM_capture_InitTypeDef init_typedef1,init_typedef2;
	ADC_cs_InitTypeDef ADC_cs;
	ADC_Channel channels[2];
	MPPT_TypeDef MPPT;
	
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
	
	PWM_input_capture_Init(&init_typedef1,&init_typedef2,0xFFFF-1,72-1);
	MY_NVIC_Init(1,0,TIM4_IRQn,2);
	MY_NVIC_Init(1,0,TIM5_IRQn,2);
	
	delay_ms(200);
	LED0=~LED0;
	
	//正弦调制输出初始化
	spwm_init_typedef.TIMx=TIM8;
	spwm_init_typedef.RCC_APB2Periph_TIMx=RCC_APB2Periph_TIM8;
	spwm_init_typedef.GPIOx=GPIOC;
	spwm_init_typedef.Pin=GPIO_Pin_6|GPIO_Pin_7;
	spwm_init_typedef.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
	SPWM_output_Init(&spwm_init_typedef,3600-1,1-1);//20kHz

	delay_ms(200);
	LED0=~LED0;
	
	//ADC采样初始化
	ADC_cs.channel_num=2;//通道数
	ADC_cs.Channels=channels;
	ADC_cs.ScanConvMode=1;//开启扫描模式
	ADC_cs.arr=1000-1;
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
	
	ADC_continuous_sampling_Init(&ADC_cs);
	
	delay_ms(200);
	LED0=~LED0;
	
	//PID调控初始化
	PID_Init(&vcc_PID,0.18,0.09,0.04,0x0FFF);
	PID_Init(&phase_PID,0.45,0.1,0.09,0x0000);
	
	delay_ms(200);
	LED0=~LED0;
	
	//SPI初始化
	SPI1_DMA1_Init(2500-1,7200-1,1,1);
	
	//MPPT初始化
	MPPT_Init(&MPPT);
	
	delay_ms(200);
	LED0=~LED0;
	LED0=1;
	
	t=0;
	power_pre=0;
	working_mode=0;
	send_flag=0;
	recv_flag=0;
	capture_flag=0;
	ADC_flag=0;
	voltage_scale_rate=0;
	voltage_scale_rate_pre=0;
	flag_REF=flag_F=0;
	Period_REF=Period_F=0;
	Frequency_REF=Frequency_F=25000;
	
	ADC_continuous_sampling_enable();

	
	while(1)
	{
		if(capture_flag)
		{
			if(SPI_send_buff[1]<40000 && SPI_send_buff[1]>10000)
			{
				TIM6->ARR=(u16)(SPI_send_buff[1]*0.18)-1;
			}
			capture_flag=0;
			voltage_scale_rate_pre=0;
		}
		
		
		//输出模式
		if(working_mode==0)//静态模式
		{
			voltage_scale_rate=0;
			power_pre=0;
		}
		else if(working_mode==1)//MPPT模式
		{
			if(ADC_flag)
			{
				get_average_duplex(DMA_buff,DMA_buff_len_ADC,&voltage_F,&Current);
				voltage=voltage_F*0.012687623;
				current=Current*0.002685546875;

				MPPT.this_power=current*voltage;
				MPPT_Caculate(&MPPT);
				voltage_scale_rate=MPPT.next_rate;
				
				SPI_send_buff[3]=voltage_F*1.2687623;
				SPI_send_buff[4]=current*50;
				ADC_flag=0;
			}
		}
		else if(working_mode==2)//稳压模式
		{
			if(ADC_flag)
			{
				get_average_duplex(DMA_buff,DMA_buff_len_ADC,&voltage_F,&Current);
				voltage_scale_rate+=get_iIncpid(&vcc_PID,target*0.767689697,voltage_F)*0.00024414;
				
				//printf("%d %d\r\n",voltage_F,Current);
				if(voltage_scale_rate>1)
					voltage_scale_rate=1;
				else if(voltage_scale_rate<0)
					voltage_scale_rate=0;
				
				SPI_send_buff[3]=voltage_F*1.2687623;
				SPI_send_buff[4]=SPI_send_buff[3];
				ADC_flag=0;
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
}
