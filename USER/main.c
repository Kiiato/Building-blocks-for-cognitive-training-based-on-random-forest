#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
//#include "lcd.h"
#include "usart.h"
#include "hc05.h"
#include "usart3.h"			 	 
#include "string.h"	   
//#include "usmart.h"	
#include "adc.h"
 
 /************************************************
 ��չʵ��1��ALIENTEK STM32F103������ 
 ATK-HC05��������ģ��ʵ��-�⺯���汾  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
	
extern u16 average_pressure_value[16];	
extern u16 time2,time3;
 int main(void)
{	 
	u8 t,i,Res;
	u8 key;
	u8 sendmask=0;
	u8 bltflag=0;
//	u8 sendcnt=0;
//	u8 sendbuf[20];	  
	u8 reclen=0;  	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	//LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//��ʼ������
	TIM2_Int_Init(4999,7199);
	TIM_Cmd(TIM2,DISABLE);
	Adc_Init();
	//usmart_dev.init(72); 	//��ʼ��USMART		
	 
	delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
 	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
	{
		printf("ATK-HC05 Error!");
	}	 						
	printf("��ʼ���ɹ�������\r\n");
	delay_ms(100);
	USART3_RX_STA=0;
 	while(1)
	{	
		key=KEY_Scan(0);
		delay_ms(1000);
//		if(key==KEY1_PRES)						//�л�ģ����������
//		{
//   			key=HC05_Get_Role();
//			if(key!=0XFF)
//			{
//				key=!key;  					//״̬ȡ��	   
//				if(key==0)
//				{
//					HC05_Set_Cmd("AT+ROLE=0");
//					printf("�ӻ�ģʽ");
//				}
//				else 
//				{
//					HC05_Set_Cmd("AT+ROLE=1");
//					printf("����ģʽ");
//				}
//				HC05_Set_Cmd("AT+RESET");	//��λATK-HC05ģ��
//				delay_ms(200);
//			}
//		}else 
		
//		if(key==KEY0_PRES)
//		{
			//sendmask=!sendmask;				//����/ֹͣ����  
			Get_once_data();
			for(i=0;i<16;i++)
			{
				//delay_ms(20);
				//u3_printf("%d,",average_pressure_value[i]);		//���͵�����ģ��
				//printf("%d,",average_pressure_value[i
				if(i<15)
					u3_printf("%d,",average_pressure_value[i]);
				else 
					u3_printf("%d,q",average_pressure_value[i]);
				average_pressure_value[i]=0;
			}	
			//printf("\r\n");
//		}
//		else if(key==WKUP_PRES)
//		{
//			u3_printf("quit");		//���͵�����ģ��
//		}
//		else delay_ms(10);	   
//		if(t==100)
//		{
//			t=0;
//			if(sendmask)					//��ʱ����
//			{
//				Get_once_data();
//				for(i=0;i<16;i++)
//				{
//					u3_printf((char*)(i*10));		//���͵�����ģ��
//					printf((char*)(i*10));
//				}	
//			}     
//		}	  
//		t++;		
		if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  	USART3_RX_BUF[reclen]=0;	 	//���������
			if(strstr((const char*)USART3_RX_BUF,"start")) 
			{
				TIM_Cmd(TIM2,ENABLE);
			}
			
			
			if(strstr((const char*)USART3_RX_BUF,"end")) 
			{
				TIM_Cmd(TIM2,DISABLE);
				Get_once_data();
				for(i=0;i<16;i++){
					if(i<15)
						u3_printf("%d,",average_pressure_value[i]);
					else 
						u3_printf("%d,q",average_pressure_value[i]);
					average_pressure_value[i]=0;
				}
				u3_printf("%d,",time3);
				time2=0;time3=0;
			}
//			Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
//			u3_printf("%s",Res);
			printf("%s",USART3_RX_BUF);
 			USART3_RX_STA=0;	 
		}
	}
}
