 #include "adc.h"
 #include "delay.h"
 #include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

u16 average_pressure_value[16];
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOG |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//16选1的控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;       //16选一控制引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
  //GPIO_ResetBits(GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);


	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   


	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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

























