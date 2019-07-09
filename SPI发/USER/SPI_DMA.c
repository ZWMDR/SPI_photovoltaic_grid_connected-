#include "SPI_DMA.h"

void SPI1_DMA1_Init(u16 arr,u16 psc,u8 master_slaver,u8 open_scheduled_transmition)
{
	SPI_InitTypeDef   SPI_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	//SPI初始化
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = (master_slaver==1)?SPI_Mode_Master:SPI_Mode_Slave;//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	
	// SPI接收
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_SPI_buff_RX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_SPI_buff_len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	//SPI 发送
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_SPI_buff_TX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = DMA_SPI_buff_len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);//传输完成中断
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);//传输错误中断
	//DMA_ITConfig(DMA1_Channel2, DMA_IT_TE, ENABLE);
	
	/* Enable SPI1 DMA TX request */
	SPI1->CR2 |= 1<<1;
	
	/* Enable SPI1 DMA RX request */
	SPI1->CR2 |= 1<<0;
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//定时发送
	if(open_scheduled_transmition)
	{
		TIM3_Int_Init(arr,psc);
		TIM3->CNT=0;
		TIM_Cmd(TIM3,ENABLE);
	}
	
	SPI_Cmd(SPI1, ENABLE);
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);//使能SPI外设
}

void SPI_send(const u16 send_buff[DMA_SPI_buff_len])
{
	//数据装载
	int i=0;
	for(i=0;i<DMA_SPI_buff_len;i++)
		DMA_SPI_buff_TX[i]=send_buff[i];
	
	//发送
	DMA1_Channel3->CNDTR=DMA_SPI_buff_len;
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
}

//中断函数
void DMA1_Channel3_IRQHandler(void)//SPI发送完成
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		send_flag=0;
		//printf("%x %x %x %x %x %x %x \r\n",DMA_SPI_buff_TX[0],DMA_SPI_buff_TX[1],DMA_SPI_buff_TX[2],DMA_SPI_buff_TX[3],DMA_SPI_buff_TX[4],DMA_SPI_buff_TX[5],DMA_SPI_buff_TX[6]);
		
		array_init_u16(DMA_SPI_buff_TX,DMA_SPI_buff_len);
		
		//do something
		
		DMA_Cmd(DMA1_Channel3, DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
	/*
	else if(DMA_GetITStatus(DMA_IT_TE)!=RESET)
	{
		printf("ERROR\r\n");
		DMA_ClearITPendingBit(DMA_IT_TE);
	}
	*/
}

void DMA1_Channel2_IRQHandler(void)//SPI接收完成
{
	if (DMA_GetITStatus(DMA1_IT_TC2)!=RESET)
	{
		//do something
		
		
		if(DMA_SPI_buff_RX[1]==0x0B0B)
		{
			LED0=~LED0;
			//printf("SPI: %x %d %d\r\n",DMA_SPI_buff_RX[1],DMA_SPI_buff_RX[2],DMA_SPI_buff_RX[3]);
			if(DMA_SPI_buff_RX[2]<10)
			{
				working_mode=0;
			}
			else if(DMA_SPI_buff_RX[2]<20)
			{
				working_mode=1;
				target=DMA_SPI_buff_RX[3];
			}
			else if(DMA_SPI_buff_RX[2]<30)
			{
				working_mode=2;
				target=DMA_SPI_buff_RX[3];
			}
			else if(DMA_SPI_buff_RX[2]<40)
			{
				working_mode=3;
				target=DMA_SPI_buff_RX[3];
			}
			recv_flag=1;
		}
		
		DMA_Cmd(DMA1_Channel2,DISABLE);
		DMA1_Channel2->CNDTR=DMA_SPI_buff_len;
		DMA_Cmd(DMA1_Channel2,ENABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void TIM3_IRQHandler(void)//SPI发送
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		SPI_send_buff[0]=0x0A0A;
		
		send_flag=1;
		//do something'

		SPI_send(SPI_send_buff);
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}
