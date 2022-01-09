 #include "adc.h"
 #include "delay.h"
 #include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

u16 average_pressure_value[16];
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOG |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//16ѡ1�Ŀ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;       //16ѡһ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
  //GPIO_ResetBits(GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);


	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   


	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(4);
	}
	return temp_val/times;
} 	 

void Get_once_data(void)
{
	u8 i,j[4],k;
	for(i=0;i<16;i++)
	{
//		switch (i)
//		{
//			case 0:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 1:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 2:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 3:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 4:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 5:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 6:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 7:GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 8:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 9:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 10:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 11:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 12:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 13:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			case 14:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_3);break;
//			case 15:GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);break;
//			default:break;
//		}
		//GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_3);
		GPIO_WriteBit(GPIOG, GPIO_Pin_6,(i & 0x08) == 0 ? Bit_RESET : Bit_SET ); 
		GPIO_WriteBit(GPIOG, GPIO_Pin_5,(i & 0x04) == 0 ? Bit_RESET : Bit_SET );
		GPIO_WriteBit(GPIOG, GPIO_Pin_4,(i & 0x02) == 0 ? Bit_RESET : Bit_SET ); 
		GPIO_WriteBit(GPIOG, GPIO_Pin_3,(i & 0x01) == 0 ? Bit_RESET : Bit_SET ); 
		//delay_ms(50);
//		j[0]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6);
//		j[1]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5);
//		j[2]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4);
//		j[3]=GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3);
//		for(k=0;k<4;k++)
//			printf("%d ",j[k]);
//		printf("\r\n");
		average_pressure_value[i]=Get_Adc_Average(ADC_Channel_1,3);
	}
}

























