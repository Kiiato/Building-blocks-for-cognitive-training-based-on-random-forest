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
 扩展实验1：ALIENTEK STM32F103开发板 
 ATK-HC05蓝牙串口模块实验-库函数版本  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为9600
	//LED_Init();				//初始化与LED连接的硬件接口
	KEY_Init();				//初始化按键
	TIM2_Int_Init(4999,7199);
	TIM_Cmd(TIM2,DISABLE);
	Adc_Init();
	//usmart_dev.init(72); 	//初始化USMART		
	 
	delay_ms(1000);			//等待蓝牙模块上电稳定
 	while(HC05_Init()) 		//初始化ATK-HC05模块  
	{
		printf("ATK-HC05 Error!");
	}	 						
	printf("初始化成功！！！\r\n");
	delay_ms(100);
	USART3_RX_STA=0;
 	while(1)
	{	
		key=KEY_Scan(0);
		delay_ms(1000);
//		if(key==KEY1_PRES)						//切换模块主从设置
//		{
//   			key=HC05_Get_Role();
//			if(key!=0XFF)
//			{
//				key=!key;  					//状态取反	   
//				if(key==0)
//				{
//					HC05_Set_Cmd("AT+ROLE=0");
//					printf("从机模式");
//				}
//				else 
//				{
//					HC05_Set_Cmd("AT+ROLE=1");
//					printf("主机模式");
//				}
//				HC05_Set_Cmd("AT+RESET");	//复位ATK-HC05模块
//				delay_ms(200);
//			}
//		}else 
		
//		if(key==KEY0_PRES)
//		{
			//sendmask=!sendmask;				//发送/停止发送  
			Get_once_data();
			for(i=0;i<16;i++)
			{
				//delay_ms(20);
				//u3_printf("%d,",average_pressure_value[i]);		//发送到蓝牙模块
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
//			u3_printf("quit");		//发送到蓝牙模块
//		}
//		else delay_ms(10);	   
//		if(t==100)
//		{
//			t=0;
//			if(sendmask)					//定时发送
//			{
//				Get_once_data();
//				for(i=0;i<16;i++)
//				{
//					u3_printf((char*)(i*10));		//发送到蓝牙模块
//					printf((char*)(i*10));
//				}	
//			}     
//		}	  
//		t++;		
		if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  	USART3_RX_BUF[reclen]=0;	 	//加入结束符
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
//			Res =USART_ReceiveData(USART3);	//读取接收到的数据
//			u3_printf("%s",Res);
			printf("%s",USART3_RX_BUF);
 			USART3_RX_STA=0;	 
		}
	}
}
