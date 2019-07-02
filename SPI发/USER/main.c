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

const s16 Sin[SINTABLE_LEN]={0,28,56,84,113,141,169,197,225,253,281,309,337,365,392,420,447,474,502,529,556,583,609,636,662,688,714,740,766,791,817,842,867,891,916,940,964,988,1011,1035,1058,1080,1103,1125,1147,1169,1190,1211,1232,1252,1272,1292,1312,1331,1350,1368,1386,1404,1422,1439,1456,1472,1488,1504,1519,1534,1549,1563,1577,1590,1603,1616,1628,1640,1651,1662,1673,1683,1693,1702,1711,1720,1728,1736,1743,1750,1756,1762,1768,1773,1777,1782,1785,1789,1792,1794,1796,1798,1799,1799,1800,1799,1799,1798,1796,1794,1792,1789,1785,1782,1777,1773,1768,1762,1756,1750,1743,1736,1728,1720,1711,1702,1693,1683,1673,1662,1651,1640,1628,1616,1603,1590,1577,1563,1549,1534,1519,1504,1488,1472,1456,1439,1422,1404,1386,1368,1350,1331,1312,1292,1272,1252,1232,1211,1190,1169,1147,1125,1103,1080,1058,1035,1011,988,964,940,916,891,867,842,817,791,766,740,714,688,662,636,609,583,556,529,502,474,447,420,392,365,337,309,281,253,225,197,169,141,113,84,56,28,0,-28,-56,-84,-113,-141,-169,-197,-225,-253,-281,-309,-337,-365,-392,-420,-447,-474,-502,-529,-556,-583,-609,-636,-662,-688,-714,-740,-766,-791,-817,-842,-867,-891,-916,-940,-964,-988,-1011,-1035,-1058,-1080,-1103,-1125,-1147,-1169,-1190,-1211,-1232,-1252,-1272,-1292,-1312,-1331,-1350,-1368,-1386,-1404,-1422,-1439,-1456,-1472,-1488,-1504,-1519,-1534,-1549,-1563,-1577,-1590,-1603,-1616,-1628,-1640,-1651,-1662,-1673,-1683,-1693,-1702,-1711,-1720,-1728,-1736,-1743,-1750,-1756,-1762,-1768,-1773,-1777,-1782,-1785,-1789,-1792,-1794,-1796,-1798,-1799,-1799,-1800,-1799,-1799,-1798,-1796,-1794,-1792,-1789,-1785,-1782,-1777,-1773,-1768,-1762,-1756,-1750,-1743,-1736,-1728,-1720,-1711,-1702,-1693,-1683,-1673,-1662,-1651,-1640,-1628,-1616,-1603,-1590,-1577,-1563,-1549,-1534,-1519,-1504,-1488,-1472,-1456,-1439,-1422,-1404,-1386,-1368,-1350,-1331,-1312,-1292,-1272,-1252,-1232,-1211,-1190,-1169,-1147,-1125,-1103,-1080,-1058,-1035,-1011,-988,-964,-940,-916,-891,-867,-842,-817,-791,-766,-740,-714,-688,-662,-636,-609,-583,-556,-529,-502,-474,-447,-420,-392,-365,-337,-309,-281,-253,-225,-197,-169,-141,-113,-84,-56,-28};

u16 DMA_SPI_buff_TX[DMA_SPI_buff_len];
u16 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u16 DMA_buff[DMA_buff_len_ADC];
u8 flag_REF;
u8 flag_F;
u8 send_flag;
u8 ADC_flag;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 t;
float voltage_scale_rate;

int main(void)
{
	u16 VCC_REF;
	u16 VCC_F;
	PID vcc_PID,phase_PID;
	SPWM_InitTypeDef spwm_init_typedef;
	PWM_capture_InitTypeDef init_typedef1,init_typedef2;
	ADC_cs_InitTypeDef ADC_cs; 
	ADC_Channel channels[2];
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();		  		 //初始化与LED连接的硬件接口
	
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
	
	//正弦调制输出初始化
	spwm_init_typedef.TIMx=TIM8;
	spwm_init_typedef.RCC_APB2Periph_TIMx=RCC_APB2Periph_TIM8;
	spwm_init_typedef.GPIOx=GPIOC;
	spwm_init_typedef.Pin=GPIO_Pin_6|GPIO_Pin_7;
	spwm_init_typedef.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
	SPWM_output_Init(&spwm_init_typedef,3600-1,1-1);//20kHz
	
	//ADC采样初始化
	ADC_cs.channel_num=2;//通道数
	ADC_cs.Channels=channels;
	ADC_cs.ScanConvMode=ENABLE;//开启扫描模式
	//通道1（连续转换只能使用ADC1）
	ADC_cs.Channels[0].num=1;
	ADC_cs.Channels[0].ADC_Channel=ADC_Channel_2;
	ADC_cs.Channels[0].ADC_SampleTime=ADC_SampleTime_71Cycles5;
	ADC_cs.Channels[0].Pin=GPIO_Pin_2;
	//通道2
	ADC_cs.Channels[1].num=2;
	ADC_cs.Channels[1].ADC_Channel=ADC_Channel_3;
	ADC_cs.Channels[1].ADC_SampleTime=ADC_SampleTime_71Cycles5;
	ADC_cs.Channels[1].Pin=GPIO_Pin_3;
	
	ADC_continuous_sampling_Init(&ADC_cs,_5kHz);
	
	//PID调控初始化
	PID_Init(&vcc_PID,0.45,0.1,0.09,0x0FFF);
	PID_Init(&phase_PID,0.45,0.1,0.09,0x0FFF);
	
	//SPI初始化
	SPI1_DMA1_Init(1000-1,7200-1,1,1);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	t=0;
	send_flag=0;
	ADC_flag=0;
	voltage_scale_rate=1;
	flag_REF=flag_F=0;
	Period_REF=Period_F=0;
	Frequency_REF=Frequency_F=25000;

	while(1)
	{
		if(send_flag)
		{
			if(DMA_SPI_buff_TX[0]<50000 && DMA_SPI_buff_TX[0]>10000)
			{
				TIM6->ARR=(u16)(DMA_SPI_buff_TX[0]*0.18)-1;
			}
			send_flag=0;
		}
		if(ADC_flag)
		{
			get_vcc_duplex(DMA_buff,DMA_buff_len_ADC,&VCC_REF,&VCC_F);
			voltage_scale_rate+=get_iIncpid(&vcc_PID,VCC_REF,VCC_F)*0.00024414;
			if(voltage_scale_rate>1)
				voltage_scale_rate=1;
			else if(voltage_scale_rate<0)
				voltage_scale_rate=0;
			DMA_Cmd(DMA1_Channel1,ENABLE);
			TIM_Cmd(TIM2,ENABLE);
			ADC_flag=0;
		}
	}
}
