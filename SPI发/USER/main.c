#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "spi.h"
#include "pwm.h"
#include "exti.h"
#include "wave_tracking.h"

u16 Sin[400]={1800,1828,1856,1884,1913,1941,1969,1997,2025,2053,2081,2109,2137,2165,2192,2220,2247,2274,2302,2329,2356,2383,2409,2436,2462,2488,2514,2540,2566,2591,2617,2642,2667,2691,2716,2740,2764,2788,2811,2835,2858,2880,2903,2925,2947,2969,2990,3011,3032,3052,3072,3092,3112,3131,3150,3168,3186,3204,3222,3239,3256,3272,3288,3304,3319,3334,3349,3363,3377,3390,3403,3416,3428,3440,3451,3462,3473,3483,3493,3502,3511,3520,3528,3536,3543,3550,3556,3562,3568,3573,3577,3582,3585,3589,3592,3594,3596,3598,3599,3599,3600,3599,3599,3598,3596,3594,3592,3589,3585,3582,3577,3573,3568,3562,3556,3550,3543,3536,3528,3520,3511,3502,3493,3483,3473,3462,3451,3440,3428,3416,3403,3390,3377,3363,3349,3334,3319,3304,3288,3272,3256,3239,3222,3204,3186,3168,3150,3131,3112,3092,3072,3052,3032,3011,2990,2969,2947,2925,2903,2880,2858,2835,2811,2788,2764,2740,2716,2691,2667,2642,2617,2591,2566,2540,2514,2488,2462,2436,2409,2383,2356,2329,2302,2274,2247,2220,2192,2165,2137,2109,2081,2053,2025,1997,1969,1941,1913,1884,1856,1828,1800,1771,1743,1715,1686,1658,1630,1602,1574,1546,1518,1490,1462,1434,1407,1379,1352,1325,1297,1270,1243,1216,1190,1163,1137,1111,1085,1059,1033,1008,982,957,932,908,883,859,835,811,788,764,741,719,696,674,652,630,609,588,567,547,527,507,487,468,449,431,413,395,377,360,343,327,311,295,280,265,250,236,222,209,196,183,171,159,148,137,126,116,106,97,88,79,71,63,56,49,43,37,31,26,22,17,14,10,7,5,3,1,0,0,0,0,0,1,3,5,7,10,14,17,22,26,31,37,43,49,56,63,71,79,88,97,106,116,126,137,148,159,171,183,196,209,222,236,250,265,280,295,311,327,343,360,377,395,413,431,449,468,487,507,527,547,567,588,609,630,652,674,696,719,741,764,788,811,835,859,883,908,932,957,982,1008,1033,1059,1085,1111,1137,1163,1190,1216,1243,1270,1297,1325,1352,1379,1407,1434,1462,1490,1518,1546,1574,1602,1630,1658,1686,1715,1743,1771};

u16 DMA_buff_TX[DMA_buff_len];
u16 DMA_buff_RX[DMA_buff_len];
u8 flag_REF;
u8 flag_F;
u16 Frequency_REF;
u16 Frequency_F;
u16 Period_REF;
u16 Period_F;
u16 t;

int main(void)
{
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	PWM_input_Init();
	PWM_init(3600-1,500-1);//20kHz
	SPI1_Init();
	t=0;

	while(1)
	{
		//printf("F_REF=%d, F_F=%d\r\n",Frequency_REF,Frequency_F);
		//printf("P_REF=%d, P_F=%d\r\n",Period_REF,Period_F);
	}
}

