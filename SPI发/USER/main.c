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
#include "24l01.h"
#include "timer.h"

const s16 Sin[400]={0,56,113,169,226,282,338,395,451,507,563,618,674,730,785,840,895,949,1004,1058,1112,1166,1219,1272,1325,1377,1429,1481,1532,1583,1634,1684,1734,1783,1832,1880,1928,1976,2023,2070,2116,2161,2206,2250,2294,2338,2380,2422,2464,2505,2545,2585,2624,2662,2700,2737,2773,2809,2844,2878,2912,2945,2977,3008,3039,3069,3098,3127,3154,3181,3207,3232,3257,3281,3303,3325,3347,3367,3387,3405,3423,3440,3457,3472,3486,3500,3513,3525,3536,3546,3555,3564,3571,3578,3584,3588,3592,3596,3598,3599,3600,3599,3598,3596,3592,3588,3584,3578,3571,3564,3555,3546,3536,3525,3513,3500,3486,3472,3457,3440,3423,3405,3387,3367,3347,3325,3303,3281,3257,3232,3207,3181,3154,3127,3098,3069,3039,3008,2977,2945,2912,2878,2844,2809,2773,2737,2700,2662,2624,2585,2545,2505,2464,2422,2380,2338,2294,2250,2206,2161,2116,2070,2023,1976,1928,1880,1832,1783,1734,1684,1634,1583,1532,1481,1429,1377,1325,1272,1219,1166,1112,1058,1004,949,895,840,785,730,674,618,563,507,451,395,338,282,226,169,113,56,0,-56,-113,-169,-226,-282,-338,-395,-451,-507,-563,-618,-674,-730,-785,-840,-895,-949,-1004,-1058,-1112,-1166,-1219,-1272,-1325,-1377,-1429,-1481,-1532,-1583,-1634,-1684,-1734,-1783,-1832,-1880,-1928,-1976,-2023,-2070,-2116,-2161,-2206,-2250,-2294,-2338,-2380,-2422,-2464,-2505,-2545,-2585,-2624,-2662,-2700,-2737,-2773,-2809,-2844,-2878,-2912,-2945,-2977,-3008,-3039,-3069,-3098,-3127,-3154,-3181,-3207,-3232,-3257,-3281,-3303,-3325,-3347,-3367,-3387,-3405,-3423,-3440,-3457,-3472,-3486,-3500,-3513,-3525,-3536,-3546,-3555,-3564,-3571,-3578,-3584,-3588,-3592,-3596,-3598,-3599,-3600,-3599,-3598,-3596,-3592,-3588,-3584,-3578,-3571,-3564,-3555,-3546,-3536,-3525,-3513,-3500,-3486,-3472,-3457,-3440,-3423,-3405,-3387,-3367,-3347,-3325,-3303,-3281,-3257,-3232,-3207,-3181,-3154,-3127,-3098,-3069,-3039,-3008,-2977,-2945,-2912,-2878,-2844,-2809,-2773,-2737,-2700,-2662,-2624,-2585,-2545,-2505,-2464,-2422,-2380,-2338,-2294,-2250,-2206,-2161,-2116,-2070,-2023,-1976,-1928,-1880,-1832,-1783,-1734,-1684,-1634,-1583,-1532,-1481,-1429,-1377,-1325,-1272,-1219,-1166,-1112,-1058,-1004,-949,-895,-840,-785,-730,-674,-618,-563,-507,-451,-395,-338,-282,-226,-169,-113,-56};

u8 DMA_SPI_buff_TX[DMA_SPI_buff_len];
u8 DMA_SPI_buff_RX[DMA_SPI_buff_len];
u16 SPI_send_buff[SPI_send_buff_len];
u16 SPI_recv_buff[SPI_send_buff_len];

u16 DMA_buff[DMA_buff_len_ADC1];
u16 DMA_buff_2[DMA_BUFF_LEN_ADC_2];
u8 NRF_mode;
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

u16 lst_TARGET;
u16 lst_Frequency_REF;


u8 buzzer_count;
u8 buzzer_status;//0: �쳣���رգ�1: �쳣���򿪣�10: �ָ��������رգ�11: �ָ���������

float voltage_scale_rate;
float voltage;
float current;
u8 working_mode;
u16 target;
/* ----------------------
	working mode:
	1: ���ȸ���ģʽ
	2: �޷�ģʽ
	3: ����ģʽ
---------------------- */

int main(void)
{
	float battery_voltage;
	float battery_current;
	u16 battery_Voltage;
	u16 battery_Current;
	//u8 frequency_change_flag;//Ƶ�ʱ仯��־
	u8 target_change_flag;//Ԥ���ѹĿ��ı��־
	u16 TARGET;//�趨Ŀ��
	u16 voltage_F;//ADC����ֵ
	u8 exception_flag;//�쳣״̬��־
	u8 lst_exception_flag;//�쳣״̬ά�ֱ�־
	//s16 frequency_error;//Ƶ�����
	//s16 phase_error;//��λ���
	///s16 voltage_error;//��ѹ���
	u16 steady_count;//�ȶ�״̬����
	//u16 shield_count;//���μ��״̬����
	PID vcc_PID,phase_PID,frequency_PID,battery_PID;//PID�ṹ
	float delta_rate;//ռ�ձȸı�ֵ
	float delta_phase;
	u16 PID_step;//PID������̬
	
	//������ģ���ʼ���ṹ������
	SPWM_InitTypeDef spwm_init_typedef;
	PWM_capture_InitTypeDef init_typedef1,init_typedef2;
	ADC_cs_InitTypeDef ADC_cs;
	ADC_Channel channels[4];
	MPPT_TypeDef MPPT;
	relay_module_InitTypeDef relay_module;
	relay_module_InitTypeDef buzzer;
	
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(19200);
	LED_Init();		  		 //��ʼ����LED���ӵ�Ӳ���ӿ�
	delay_ms(200);
	LED0=0;
	
	//���벶���ʼ��
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
	
	PWM_input_capture_Init(&init_typedef1,&init_typedef2,0xFFFF,36-1);
	MY_NVIC_Init(3,2,TIM4_IRQn,2);
	MY_NVIC_Init(3,2,TIM5_IRQn,2);
	
	delay_ms(200);
	LED0=~LED0;
	
	//���ҵ��������ʼ��
	spwm_init_typedef.TIMx=TIM8;
	spwm_init_typedef.RCC_APB2Periph_TIMx=RCC_APB2Periph_TIM8;
	spwm_init_typedef.GPIOx=GPIOC;
	spwm_init_typedef.Pin=GPIO_Pin_8|GPIO_Pin_7;
	spwm_init_typedef.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
	SPWM_output_Init(&spwm_init_typedef,3600-1,1-1);//20kHz

	delay_ms(200);
	LED0=~LED0;
	
	//ADC������ʼ��������ת��ֻ��ʹ��ADC1��
	ADC_cs.channel_num=4;//ͨ����
	ADC_cs.Channels=channels;
	ADC_cs.ScanConvMode=1;//����ɨ��ģʽ
	ADC_cs.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
	ADC_cs.arr=1000-1;
	ADC_cs.psc=72-1;
	//ͨ��1��������ѹ
	ADC_cs.Channels[0].num=1;
	ADC_cs.Channels[0].ADC_Channel=ADC_Channel_12;
	ADC_cs.Channels[0].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[0].Pin=GPIO_Pin_2;
	ADC_cs.Channels[0].GPIOx=GPIOC;
	//ͨ��2����������
	ADC_cs.Channels[1].num=2;
	ADC_cs.Channels[1].ADC_Channel=ADC_Channel_13;
	ADC_cs.Channels[1].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[1].Pin=GPIO_Pin_3;
	ADC_cs.Channels[1].GPIOx=GPIOC;
	//ͨ��3��ֱ����ѹ
	ADC_cs.Channels[2].num=3;
	ADC_cs.Channels[2].ADC_Channel=ADC_Channel_11;
	ADC_cs.Channels[2].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[2].Pin=GPIO_Pin_1;
	ADC_cs.Channels[2].GPIOx=GPIOC;
	//ͨ��4��ֱ������
	ADC_cs.Channels[3].num=4;
	ADC_cs.Channels[3].ADC_Channel=ADC_Channel_14;
	ADC_cs.Channels[3].ADC_SampleTime=ADC_SampleTime_239Cycles5;
	ADC_cs.Channels[3].Pin=GPIO_Pin_4;
	ADC_cs.Channels[3].GPIOx=GPIOC;
	
	//��ʼ��
	ADC1_continuous_sampling_Init(&ADC_cs,DMA_buff,DMA_buff_len_ADC1);
	
	delay_ms(200);
	LED0=~LED0;
	
	//PID���س�ʼ��
	PID_Init(&vcc_PID,0.09,0.045,0.0075,0x0FFF);
	PID_Init(&phase_PID,0.2,0.08,0.06,0x0000);
	PID_Init(&frequency_PID,0.3,0.16,0.09,40000);
	PID_Init(&battery_PID,0.09,0.045,0.0075,0x0FFF);
	
	delay_ms(200);
	LED0=~LED0;
	
	//SPI��ʼ��
	//SPI1_DMA1_Init(1000-1,7200-1,1,1);
	array_init_u16(SPI_send_buff,SPI_send_buff_len);
	NRF24L01_Init();
	TIM1_Int_Init(400-1,7200-1);
	
	delay_ms(200);
	LED0=~LED0;
	
	//MPPT��ʼ��
	MPPT_Init(&MPPT);
	
	//�̵�����ʼ��
	relay_module.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOB;
	relay_module.GPIOx=GPIOB;
	relay_module.Pin=GPIO_Pin_14;
	relay_module.mode=1;
	relay_module_Init(&relay_module);
	relay_module_Close(&relay_module);
	
	//��������ʼ��
	buzzer.RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOB;
	buzzer.GPIOx=GPIOB;
	buzzer.Pin=GPIO_Pin_15;
	buzzer.mode=1;
	relay_module_Init(&buzzer);
	relay_module_Close(&buzzer);
	
	delay_ms(200);
	LED0=~LED0;
	LED0=1;
	
	TIM3_Int_Init(5000-1,7200-1);
	
	t=0;
	//frequency_change_flag=0;
	target_change_flag=0;
	steady_count=0;
	//shield_count=0;
	working_mode=0;
	exception_flag=0;
	lst_exception_flag=0;
	lst_TARGET=0;
	send_flag=0;
	recv_flag=0;
	capture_flag=0;
	ADC_flag=0;
	voltage_scale_rate=0;
	flag_REF=flag_F=0;
	Period_REF=Period_F=0;
	buzzer_status=0;
	buzzer_count=0;
	Frequency_REF=Frequency_F=40000;
	ADC1_continuous_sampling_enable();
	
	while(1)
	{
		if(NRF_mode==0)
		{
			recv_flag=NRF_recv();
		}
		
		if(flag_REF)//Ƶ�ʸ���
		{
			if(Frequency_REF<50000 && Frequency_REF>33333)
			{
				//frequency_change_flag=abs(Frequency_REF-lst_Frequency_REF)>Frequency_REF*0.05?1:0;
				lst_Frequency_REF=Frequency_REF;
				TIM6->ARR=Frequency_REF*0.09-1;
			}
			flag_REF=0;
		}
		
		if(flag_F)
		{
			//frequency_error=(s16)Frequency_REF-Frequency_F;
			//printf("Frequency_error=%d\r\n",frequency_error);
			flag_F=0;
		}
		
		if(capture_flag)
		{
			//printf("Period_F: %d\r\n",Period_F);
			//phase_error=(s16)Period_F<(Frequency_REF>>1)?Period_F:Period_F-Frequency_REF;
			capture_flag=0;
		}
			
		/*
		if(capture_flag)//��λ����
		{
			delta_phase=((float)(Period_F<(Frequency_REF>>1)?Period_F:Period_F-Frequency_REF)/Frequency_REF)*1000;
			t=(t+400+get_iIncpid(&phase_PID,0,delta_phase))%400;
			capture_flag=0;
		}
		*/
		
		
		if(exception_flag==0)
		{
			if(lst_exception_flag)
			{
				TIM3->ARR=3000-1;
				lst_exception_flag=0;
				buzzer_count=4;
				buzzer_status=10;
			}
			if(buzzer_count>0)
			{
				if(buzzer_status==11)
					relay_module_Open(&buzzer);
				else if(buzzer_status==10)
					relay_module_Close(&buzzer);
			}
			else
			{
				relay_module_Open(&relay_module);
				relay_module_Close(&buzzer);
				TIM_Cmd(TIM3,DISABLE);
				SPI_send_buff[5]=0;
				SPI_send_buff[6]=0;
			}

			//���ģʽ
			if(working_mode==0)//��̬ģʽ
			{
				relay_module_Close(&buzzer);
				relay_module_Close(&relay_module);
				voltage_scale_rate=0.8;
				delta_rate=0;
				PID_step=0;
				LED1=1;
			}
			else if(working_mode==1)//MPPTģʽ
			{
				if(ADC_flag)
				{
					LED1=1;
					TARGET=target*2.4213075-22.566586;
					voltage_F=get_average(DMA_buff,DMA_buff_len_ADC1,0,4);
					Current=get_average(DMA_buff,DMA_buff_len_ADC1,1,4);
					battery_Voltage=get_average(DMA_buff,DMA_buff_len_ADC1,2,4);
					battery_Current=get_average(DMA_buff,DMA_buff_len_ADC1,3,4);
					
					//printf("battery: %d\r\n",battery_Voltage);
					//voltage=voltage_F*0.0413+0.0932;
					//current=Current*0.0007461+0.0047563;
					battery_voltage=battery_Voltage*0.028274-0.0172;
					battery_current=battery_Current;

					//MPPT.this_power=battery_current*battery_voltage;
					//MPPT_Caculate(&MPPT);
					//voltage_scale_rate=MPPT.next_rate;
					target_change_flag=lst_TARGET!=TARGET?1:0;
					
					/*
					if(target_change_flag)
					{
						steady_count=0;
						target_change_flag=0;
					}
					else
						steady_count=steady_count<100?steady_count+1:100;
					
					if(steady_count<100)//�ضϼ̵���
					{
						LED1=0;
						relay_module_Close(&relay_module);
						//printf("close\r\n");
					}
					else//�򿪼̵���
					{
						relay_module_Open(&relay_module);
						lst_Frequency_REF=Frequency_REF;
						//printf("open\r\n");
						LED1=1;
					}
					*/
					
					if(Current*0.7461>1500)//1.5A��������
					{
						LED1=0;
						relay_module_Close(&relay_module);//�ض����
						relay_module_Open(&buzzer);//����������
						exception_flag=1;
						buzzer_count=10;
						buzzer_status=1;
						TIM3->ARR=5000-1;
						TIM3->CNT=0;
						TIM_Cmd(TIM3,ENABLE);
					}
					/*
					else if(battery_voltage<25)//Ƿѹ����
					{
						LED1=0;
						relay_module_Close(&relay_module);//�ض����
						relay_module_Open(&buzzer);//����������
						
						exception_flag=2;
						buzzer_count=10;
						buzzer_status=1;
						TIM3->ARR=5000-1;
						TIM3->CNT=0;
						TIM_Cmd(TIM3,ENABLE);
					}
					*/
					else
					{
						SPI_send_buff[5]=0x0000;
					}
					
					/*
					//if(TARGET>voltage_F*10)
					{
						delta_rate=get_iIncpid(&battery_PID,3000,battery_voltage*100)*0.00024414;
						
						if(abs_f(delta_rate)<0.0003)
						{
							if(PID_step<10)
								PID_step++;
						}
						else
							PID_step=0;
						
						if(PID_step<10)
						{
							voltage_scale_rate-=delta_rate;
							if(voltage_scale_rate>1)
								voltage_scale_rate=1;
							else if(voltage_scale_rate<0.3)
								voltage_scale_rate=0.3;
						}
					}
					/*
					else
					{
						delta_rate=get_iIncpid(&vcc_PID,TARGET,voltage_F*10)*0.00024414;
						
						if(fabs(delta_rate)<0.0004)
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
					}
					*/
					if(battery_voltage>30)
						voltage_scale_rate+=0.0005;
					else if(battery_voltage<30)
						voltage_scale_rate-=0.0005;
					
					SPI_send_buff[1]=battery_voltage*100;
					//SPI_send_buff[1]=battery_Voltage;
					SPI_send_buff[3]=voltage_F;//�����ѹ
					SPI_send_buff[4]=Current;//�������
					ADC_flag=0;
					lst_TARGET=TARGET;
				}
			}
			else if(working_mode==2)//��ѹģʽ
			{
				if(ADC_flag)
				{
					TARGET=target*2.4213075-22.566586;
					voltage_F=get_average(DMA_buff,DMA_buff_len_ADC1,0,4);
					Current=get_average(DMA_buff,DMA_buff_len_ADC1,1,4);
					//voltage_error=(s16)TARGET-voltage_F*10;
					
					target_change_flag=lst_TARGET!=TARGET?1:0;
					
					if(target_change_flag)
					{
						steady_count=0;
						target_change_flag=0;
					}
					else
						steady_count=steady_count<100?steady_count+1:100;
					
					if(steady_count<100)//�ضϼ̵���
					{
						LED1=0;
						relay_module_Close(&relay_module);
						//printf("close\r\n");
					}
					else//�򿪼̵���
					{
						relay_module_Open(&relay_module);
						lst_Frequency_REF=Frequency_REF;
						//printf("open\r\n");
						LED1=1;
					}
					
					if(Current*0.7461>1500)//1.5A��������
					{
						LED1=0;
						relay_module_Close(&relay_module);//�ض����
						relay_module_Open(&buzzer);//����������
						SPI_send_buff[5]=0xAAAA;
						exception_flag=1;
						buzzer_count=10;
						buzzer_status=1;
						TIM3->ARR=5000-1;
						TIM3->CNT=0;
						TIM_Cmd(TIM3,ENABLE);
					}
					else
					{
						SPI_send_buff[5]=0x0000;
					}
					
					delta_rate=get_iIncpid(&vcc_PID,TARGET,voltage_F*10)*0.00024414;
					
					if(fabs(delta_rate)<0.0004)
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
					
					SPI_send_buff[3]=voltage_F;
					SPI_send_buff[4]=Current;
					
					ADC_flag=0;
					lst_Frequency_REF=Frequency_REF;
					lst_TARGET=TARGET;
				}
			}
			else if(working_mode==3)//����ģʽ
			{
				if(recv_flag)
				{
					recv_flag=0;
				}
			}
			else
				working_mode=0;
		}
		else//�쳣״̬����
		{
			lst_exception_flag=exception_flag;
			if(buzzer_count>0)//����
			{
				if(buzzer_status==1)
					relay_module_Open(&buzzer);
				else if(buzzer_status==0 )
					relay_module_Close(&buzzer);
				
				if(exception_flag==1)
				{
					SPI_send_buff[5]=0xAAAA;
					SPI_send_buff[6]=buzzer_count;
				}
				else if(exception_flag==2)
				{
					SPI_send_buff[5]=0xAAAB;
					SPI_send_buff[6]=buzzer_count;
				}
			}
			else//��ѹ�Դ�
			{
				relay_module_Close(&buzzer);
				relay_module_Open(&relay_module);
				voltage_scale_rate=voltage_scale_rate*0.75;
				LED1=1;
				exception_flag=0;
			}
		}
	}
}
