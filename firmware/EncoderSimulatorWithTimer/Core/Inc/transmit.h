/*
 * transmit.h
 *
 *  Created on: Nov 6, 2025
 *      Author: Abdulkadir
 */

#ifndef INC_TRANSMIT_H_
#define INC_TRANSMIT_H_

#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart2;
void sendEncoderDepth(int32_t depth,uint16_t speed);
uint8_t uartSeq();
#endif /* INC_TRANSMIT_H_ */
