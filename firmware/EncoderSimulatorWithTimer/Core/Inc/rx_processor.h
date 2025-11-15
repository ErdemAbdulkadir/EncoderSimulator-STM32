/*
 * rx_processor.h
 *
 *  Created on: Oct 26, 2025
 *      Author: Abdulkadir
 */

#ifndef INC_RX_PROCESSOR_H_
#define INC_RX_PROCESSOR_H_

#include "stm32f4xx_hal.h"
#define RX_BUFFER_SIZE 128
#define RX_TEMP_BUFFER_SIZE 32
extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart2;
void  rxProgressData(uint8_t* data, uint16_t size);

#endif /* INC_RX_PROCESSOR_H_ */
