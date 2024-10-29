#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

extern int receive_data[10];
extern int receive_data2[10];
extern int count;
extern volatile int packetReceived;
extern volatile int packetReceived2;

#define FRAME_HEADER 0x7E
#define FRAME_FOOTER 0x7E

void USART1_IRQHandler(void);

// 初始化USART1
void USART1_Init(void);

// 发送一个字节
void USART1_SendByte(int byte);

// 接收一个字节
uint8_t USART1_ReceiveByte(void);

// 发送数据包
void USART1_SendPacket(int* data, uint16_t size);

// 接收数据包
void USART1_ReceivePacket(int* buffer, uint16_t maxSize);


void USART3_IRQHandler(void);

// 初始化USART1
void USART3_Init(void);

// 发送一个字节
void USART3_SendByte(int byte);

// 接收一个字节
uint8_t USART3_ReceiveByte(void);

// 发送数据包
void USART3_SendPacket(int* data, uint16_t size);

// 接收数据包
void USART3_ReceivePacket(int* buffer, uint16_t maxSize);
#endif  __USART_H