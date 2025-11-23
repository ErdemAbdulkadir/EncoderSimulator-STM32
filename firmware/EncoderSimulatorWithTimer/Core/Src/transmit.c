/*
 * transmit.c
 *
 *  Created on: Nov 6, 2025
 *      Author: Abdulkadir
 */


#include "transmit.h"
#include "uart_protocol.h"
uint8_t seq = 0;
uint8_t ake = 0;

volatile uint8_t uartBusy = 0;

uint8_t uartSeq()
{
	if(seq >= 255)
		seq = 0;

	return seq++;
}

void sendEncoderDepth(int32_t depth, uint16_t speed)
{
	if (uartBusy)
		return;
	uartBusy = 1;
	Uart.ReqPackEncoderDepth.sop = UART_SOP;
	Uart.ReqPackEncoderDepth.key = UartReqPack_EncoderDepth;
	Uart.ReqPackEncoderDepth.seq = uartSeq();
	Uart.ReqPackEncoderDepth.depth = depth;
	Uart.ReqPackEncoderDepth.speed = speed;
	crc_fill((uint8_t*)&Uart.ReqPackEncoderDepth, sizeof(Uart.ReqPackEncoderDepth));

	uint32_t timeout = HAL_GetTick();
	while (huart2.gState != HAL_UART_STATE_READY)
	{
		if (HAL_GetTick() - timeout > 1000)
			return; // timeout oldu
	}
	if (HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&Uart.ReqPackEncoderDepth, sizeof(Uart.ReqPackEncoderDepth)) != HAL_OK)
		uartBusy = 0;
}

void sendDebugMsg(const uint8_t* msg)
{
	uint8_t msgLen = 0;
	Uart.ReqPackDebug.sop = UART_SOP;
	Uart.ReqPackDebug.key = UartReqPack_Debug;
	Uart.ReqPackDebug.seq = uartSeq();

	while(msg[msgLen] != '\0' && msgLen < 24)
	{
		Uart.ReqPackDebug.msg[msgLen] = msg[msgLen];
		msgLen++;
	}
	Uart.ReqPackDebug.msgLen = msgLen;
	crc_fill((uint8_t*) &Uart.ReqPackDebug, sizeof(Uart.ReqPackDebug));

	uint32_t timeout = HAL_GetTick();
	while(huart2.gState != HAL_UART_STATE_READY)
	{
		if(HAL_GetTick() - timeout >= 1000)
			return;
	}
	if (HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&Uart.ReqPackDebug, sizeof(Uart.ReqPackDebug)) != HAL_OK)
		uartBusy = 0;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
		uartBusy = 0;
}




