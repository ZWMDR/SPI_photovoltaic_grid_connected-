#include "NRF24L01_module.h"

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ

u8 SPI1_ReadWriteByte(u8 TxData)
{
	u8 retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}

u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}

u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
 	NRF24L01_CSN = 0;          //ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	


void NRF24L01_Init(u8 mode,u16 arr,u16 psc)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	SPI_InitTypeDef         SPI_InitStructure; 
	DMA_InitTypeDef       DMA_InitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;
	EXTI_InitTypeDef       EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE );	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;   //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// SPI����
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_SPI_buff_RX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_SPI_buff_len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	//SPI ����
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA_SPI_buff_TX;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = DMA_SPI_buff_len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);//��������ж�
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE);//��������ж�
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
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);
	
	if(mode==1)
	{
		TIM3_Int_Init(arr,psc);
		TIM3->CNT=0;
		TIM_Cmd(TIM3,ENABLE);
	}
	
	SPI_Cmd(SPI1, DISABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	NRF24L01_CE=0; 	//ʹ��24L01
	NRF24L01_CSN=1;	//SPIƬѡȡ��	 	
	
}

void NRF24L01_RX_Mode(void)//����Ϊ����ģʽ
{
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    	//ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);	//ʹ��ͨ��0�Ľ��յ�ַ  	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	    	//����RFͨ��Ƶ��		  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);	//����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);		//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}

void NRF24L01_TX_Mode(void)//����Ϊ����ģʽ
{
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_Cmd(DMA1_Channel3,DISABLE);
	
	NRF24L01_CE=0;
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
 	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
 	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
 	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
 	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
 	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
 	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE=1;//CEΪ��,10us����������
}

//�жϺ���
void DMA1_Channel3_IRQHandler(void)//SPI������ɣ��������߷���
{
	if (DMA_GetITStatus(DMA1_IT_TC3)!=RESET)
	{
		NRF24L01_CSN = 1;//�ر�SPI����Ƭѡ
		NRF24L01_CE=1;//��������
		
		array_init_u8(DMA_SPI_buff_TX,DMA_SPI_buff_len);
		DMA_Cmd(DMA1_Channel3,DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC3);
	}
}

void DMA1_Channel2_IRQHandler(void)//SPI�������
{
	u8 i;
	if (DMA_GetITStatus(DMA1_IT_TC2)!=RESET)
	{
		NRF24L01_CSN=1;
		NRF24L01_Write_Reg(FLUSH_RX,0xff);
		for(i=0;i<SPI_send_buff_len;i++)
		{
			SPI_recv_buff[i]=((u16)DMA_SPI_buff_RX[i*2]<<8)|DMA_SPI_buff_RX[i*2+1];
		}
		
		if(SPI_recv_buff[1]==0x0B0B)
		{
			LED0=~LED0;
			if(SPI_recv_buff[2]<10)
			{
				working_mode=0;
			}
			else if(SPI_recv_buff[2]<20)
			{
				working_mode=1;
				target=SPI_recv_buff[3];
			}
			else if(SPI_recv_buff[2]<30)
			{
				working_mode=2;
				target=SPI_recv_buff[3];
			}
			else if(SPI_recv_buff[2]<40)
			{
				working_mode=3;
				target=SPI_recv_buff[3];
			}
			recv_flag=1;
		}
		DMA_Cmd(DMA1_Channel2,DISABLE);
		
		DMA_ClearITPendingBit(DMA1_IT_TC2);
	}
}

void EXTI1_IRQHandler(void)//�����ź��ж�
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		if(send_flag)//�������
		{
			LED1=~LED1;
			NRF24L01_RX_Mode();//����Ϊ����ģʽ���ȴ�����
			send_flag=0;
		}
		else//���߽�����ɣ�����SPI����
		{
			NRF_status=NRF24L01_Read_Reg(STATUS);
			//if(NRF_status)
			//{
			NRF24L01_CSN = 0;
			SPI1_ReadWriteByte(RD_RX_PLOAD);
			DMA1_Channel2->CNDTR=DMA_SPI_buff_len;
			DMA_Cmd(DMA1_Channel2,ENABLE);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void TIM3_IRQHandler(void)//SPI��������
{
	u8 i;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		SPI_Cmd(SPI1,ENABLE);
		NRF24L01_TX_Mode();
		NRF_status = SPI1_ReadWriteByte(WR_TX_PLOAD);
		
		SPI_send_buff[0]=0x0A0A;
		for(i=0;i<SPI_send_buff_len;i++)
		{
			//���ֽ���ǰ�����ֽ��ں�
			DMA_SPI_buff_TX[2*i]=(SPI_send_buff[i]>>8)&0x00FF;
			DMA_SPI_buff_TX[2*i+1]=SPI_send_buff[i]&0x00FF;
		}
		
		NRF24L01_CSN = 0;//ʹ��SPI
		NRF24L01_CE=0;//ֹͣ����
		
		//ʹ��DMA����ͨ��
		DMA1_Channel3->CNDTR=DMA_SPI_buff_len;
		DMA_Cmd(DMA1_Channel3,ENABLE);
		send_flag=1;//������״̬��־��1
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}
