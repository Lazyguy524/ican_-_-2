#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Stdbool.h"
#include "stm32f10x.h"

#define FRAME_HEADER 0x7E
#define FRAME_FOOTER 0x7E


int count = 0;
volatile int packetReceived = 0;
volatile int packetReceived2 = 0;


int receive_data[10];
int receive_data2[10];



void USART1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // ?? PA9 ?? USART1 ? TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ?? PA10 ?? USART1 ? RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1 ??
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
		
		 // 使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 配置中断优先级
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}



void USART1_SendByte( int byte) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte);
}

uint8_t USART1_ReceiveByte() {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(USART1);
}


//  帧头为     #define FRAME_HEADER 0x7E
 // 帧尾为     #define FRAME_FOOTER 0x7E
// 入参就是    数组    信息帧大小（单位为字节）


void USART1_SendPacket(uint8_t* data, uint16_t size) {
    USART1_SendByte(FRAME_HEADER); // 
    for (int i = 0; i < size; i++) {
        USART1_SendByte(data[i]); // 
    }
    USART1_SendByte(FRAME_FOOTER); //
}

void USART1_ReceivePacket(int* buffer, uint16_t maxSize) {
  int i = 0;
   int byte;
    do {
			byte = USART1_ReceiveByte();//接收到帧头后再将信息位塞进buffer里 buffer最大10个字节
    } while (byte != FRAME_HEADER);

    while (i < maxSize) {
        byte = USART1_ReceiveByte();
        if (byte == FRAME_FOOTER) {
            break; 
        }
        buffer[i++] = byte;
    }
}
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
			{
        int byte = USART1_ReceiveByte();  // 读取接收到的字节

        // 检查是否接收到帧头，开始接收数据
         if (byte == FRAME_HEADER && count == 0)
					{
             count=0;       // 准备接收数据
           } 
					
				else if (count < 10)
					{
            if (byte == FRAME_FOOTER) 
							{
                // 接收到帧尾，标记数据包接收完成
                packetReceived = 1;
								

							count = 0;  // 重置索引，准备下一次接收
            } 
							
				else 
						{
                // 将接收到的数据存储到缓冲区
               receive_data[count++] = byte;
            }
						
        }

        // 清除中断标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
void USART3_Init(void) {
    // 使能 USART3 和 GPIOB 的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 设置 GPIOB 的第10引脚作为 USART3 的 TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 设置 GPIOB 的第11引脚作为 USART3 的 RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // USART3 配置
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    // 使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    
    // 配置 NVIC 以及中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 USART3
    USART_Cmd(USART3, ENABLE);
}



void USART3_SendByte( int byte) {
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3, byte);
}
uint8_t USART3_ReceiveByte() {
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(USART3);
}

void USART3_ReceivePacket(int* buffer, uint16_t maxSize) {
  int i = 0;
   int byte;
    do {
			byte = USART3_ReceiveByte();//接收到帧头后再将信息位塞进buffer里 buffer最大10个字节
    } while (byte != FRAME_HEADER);

    while (i < maxSize) {
        byte = USART3_ReceiveByte();
        if (byte == FRAME_FOOTER) {
            break; 
        }
        buffer[i++] = byte;
    }
}
void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        int byte = USART3_ReceiveByte();  // 读取接收到的字节

        // 检查是否接收到帧头，开始接收数据
        if (byte == FRAME_HEADER && count == 0) {
            count = 0;       // 准备接收数据
        } else if (count < 10) {
            if (byte == FRAME_FOOTER) {
                // 接收到帧尾，标记数据包接收完成
                packetReceived2 = 1;
                count = 0;  // 重置索引，准备下一次接收
            } else {
                // 将接收到的数据存储到缓冲区
                receive_data2[count++] = byte;
            }
        }

        // 清除中断标志位
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

//void USART2_Init(void) {
//    // 使能 USART2 和 GPIOA 的时钟
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;

//    // 配置 PA2 为 USART2 的 TX
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    // 配置 PA3 为 USART2 的 RX
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    // USART2 配置
//    USART_InitStructure.USART_BaudRate = 9600;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(USART2, &USART_InitStructure);

//    // 使能接收中断
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

//    // 配置中断优先级
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 优先级相对较低
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    // 使能 USART2
//    USART_Cmd(USART2, ENABLE);
//}
//void USART2_SendByte(int byte) {
//    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//    USART_SendData(USART2, byte);
//}

//uint8_t USART2_ReceiveByte() {
//    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
//    return (uint8_t)USART_ReceiveData(USART2);
//}

//void USART2_IRQHandler(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//    {
//        int byte = USART2_ReceiveByte();  // 读取接收到的字节

//        // 检查是否接收到帧头，开始接收数据
//        if (byte == FRAME_HEADER && count == 0) {
//            count = 0;       // 准备接收数据
//        } else if (count < 10) {
//            if (byte == FRAME_FOOTER) {
//                // 接收到帧尾，标记数据包接收完成
//                packetReceived2 = 1;
//                count = 0;  // 重置索引，准备下一次接收
//            } else {
//                // 将接收到的数据存储到缓冲区
//                receive_data2[count++] = byte;
//            }
//        }

//        // 清除中断标志位
//        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//    }
//}