#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Stdbool.h"
#include "stm32f10x.h"
#include "Usart.h"

extern volatile  int motor_pause;
int rgb_flag;
typedef enum {
 OFF = 0,
 ON = 1,
 MIDDLE=2
} MotorState;
#define OFF 0
#define ON 1
#define MIDDLE 5

int flag=10;
 
//void recall_task();

int judge,pro_start=0;


int main(void)
{	


	Motor_Init();
	Key_Init();
	Delay_ms(1000);	
	USART1_Init();
	USART3_Init();
	motor_pause=MIDDLE;

	while (1)
	{					
//		USART1_ReceivePacket(receive_data,10);//������λ�����
		if(receive_data[0]==1)
		
		pro_start=1;//控制全局
		
   	else if(receive_data[0]==4)
			pro_start=0;
	
		
		
		if(	pro_start==1)
		{
			
		if ( receive_data2[0]==6) //��ϢλΪ1�ͻ�����  
		{
			motor_pause=ON;
		}				
		else if (receive_data[0]==2) // Ϊ2�Ļ��ı�motor״̬�� ��ƽ��ʱֹͣ��ת ����������ᱻ���� 
		{
		  judge=2;
		
		}
	  else if (receive_data[0]==3) 
		{
			judge=3;//评测不通过
		}			
		
		//状态机设置
		
		
		if(motor_pause==ON) 
		{
		
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			Motor_Run(0,3,400);//		
			Delay_ms(1000);     //�����˶��ļ��
			  USART1_SendByte(0x7E);  // 发送数据包的第一个字节 0x7E
    USART1_SendByte(5);     // 发送数据包的第二个字节 5
    USART1_SendByte(0x7E);  // 发送数据包的第三个字节 0x7E

			//	   	receive_data[0]=OFF; 
			motor_pause=OFF;
		 	receive_data2[0]=OFF;//清除颜色传感器标志位和移动指示标志位 
//			Motor_Run(1,8,300,motor_pause);
//			Delay_ms(1000);
	
			}						
	
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);    //����PA5Ϊ�͵�ƽ��ʹ�ܹ��ܿ���
		}	
		
		
				//物体检测通过后继续前
		if(judge==2)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			Motor_Run(0,2,350);//			Delay_ms(1000);     //�����˶��ļ��
			Delay_ms(1000); 
//			Motor_Run(1,8,300,motor_pause);
//			Delay_ms(1000);
			 	receive_data[0]=0;
			judge=MIDDLE;
		receive_data2[0]=0;
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			
			 //����PA5Ϊ�͵�ƽ��ʹ�ܹ��ܿ���
		}	
		
		
		
		if(judge==3)//物体检测不通过后继续前进
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			Motor_Run(1,6,400);//			Delay_ms(1000);     //�����˶��ļ��
			
//			Motor_Run(1,8,300,motor_pause);
			 Delay_ms(1000);
			judge=MIDDLE;
			 receive_data[0]=0;
		}
		
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			
			 //����PA5Ϊ�͵�ƽ��ʹ�ܹ��ܿ���
		}	
	}
	}		
}



//void EXTI0_IRQHandler(void)
//	{
//		if (EXTI_GetITStatus(EXTI_Line0) == SET)
//		{			
//				if (flag == 1)
//				{						
//					GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//					flag = 0;				
//				}
//				else flag=1;
//				EXTI_ClearITPendingBit(EXTI_Line0);								
//		}
//	}
//void USART1_IRQHandler(void) {
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
//        int byte = USART1_ReceiveByte();  // 读取接收到的字节

//        // 检查是否接收到帧头，开始接收数据
//        if (byte == FRAME_HEADER && index == 0) {
//            // 准备接收数据
//        } else if (index > 0 && index < 10) {
//            if (byte == FRAME_FOOTER) {
//                // 接收到帧尾，标记数据包接收完成
//                packetReceived = 1;
//                index = 0;  // 重置索引，准备下一次接收
//            } else {
//                // 将接收到的数据存储到缓冲区
//                buffer[index++] = byte;
//            }
//        }

//        // 清除中断标志位
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//}

//	void recall_task(void)
//	{
//	
//		
//		if(motor_pause==OFF)
//		USART1_SendByte(1);//系统自检正常 处于运行状态
//	}

	


