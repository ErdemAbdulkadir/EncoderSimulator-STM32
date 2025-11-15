/*
* rx_processor.c
*
*  Created on: Oct 26, 2025
*      Author: Abdulkadir
*/
#include "rx_processor.h"
#include "uart_protocol.h"

#define RX_BUFFER_SIZE       128
#define RX_TEMP_BUFFER_SIZE   32

UartResPackKey_t uartResPackey;
uint8_t rxPacket[RX_BUFFER_SIZE];
uint8_t rxTemp[RX_TEMP_BUFFER_SIZE];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;
volatile uint8_t rxCount = 0;
uint8_t key, sz, keyIndex = 1;
uint8_t* res;

void rxProgressData(uint8_t* data, uint16_t size)
{
	for(uint8_t i = 0; i < size; i++)
	{
		rxPacket[head] = data[i];
		head = (head + 1) % RX_BUFFER_SIZE;
		if(rxCount < RX_BUFFER_SIZE)
			rxCount++;
		else
			tail = (tail + 1) % RX_BUFFER_SIZE;
	}
    while (rxCount >= RX_TEMP_BUFFER_SIZE)
    {
        for (uint8_t i = 0; i < RX_TEMP_BUFFER_SIZE; i++)
        {
            rxTemp[i] = rxPacket[tail];
            tail = (tail + 1) % RX_BUFFER_SIZE;
        }
        rxCount -= RX_TEMP_BUFFER_SIZE;
        if(rxTemp[0] != UART_SOP)
        	  continue;

        key = rxTemp[keyIndex];
        uartResPackey = (UartResPackKey_t)key;
        res = uartResPackKey(uartResPackey);
        sz = sizeOfUartResPack(uartResPackey);

        if (!crc_is_valid(rxTemp, sz))
           continue;

        memcpy(res, rxTemp, sz);

        switch (key)
        {
        case UartResPack_EncoderConfig:
            resPackFlag.resEncoderConfigFlag = 1;
            break;
        case UartResPack_EncoderDepth:
            resPackFlag.resEncoderDepthFlag = 1;
            break;
        case UartResPack_EncoderStatus:
            resPackFlag.resEncoderStatusFlag = 1;
            break;
        default:
            break;
        }
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
        rxProgressData(rxBuffer, Size);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, RX_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    }
}
