#include "Uart_protocol.h"
#include "stdio.h"

uart_t Uart;
ResPackFlag_t resPackFlag = {0};

uint8_t* uartReqPackKey(UartReqPackKey_t key)
{
	switch(key){
	case UartReqPack_EncoderConfig:
		return (uint8_t*) &Uart.ReqPackEncoderConfig;
	case UartReqPack_EncoderDepth:
		return (uint8_t*) &Uart.ReqPackEncoderDepth;
	case UartReqPack_EncoderStatus:
		return (uint8_t*) &Uart.ReqPackEncoderStatus;
    case UartReqPack_Debug:
        return (uint8_t*) &Uart.ReqPackDebug;
	default:
		break;
	}
	return (uint8_t*) NULL;;
}

uint8_t* uartResPackKey(UartResPackKey_t key){

	switch(key){
	case UartResPack_EncoderConfig:
		return (uint8_t*) &Uart.ResPackEncoderConfig;
	case UartResPack_EncoderDepth:
		return (uint8_t*) &Uart.ResPackEncoderDepth;
	case UartResPack_EncoderStatus:
		return (uint8_t*) &Uart.ResPackEncoderStatus;
    case UartResPack_Debug:
        return (uint8_t*) &Uart.ResPackDebug;
	default:
		break;
	}
	return (uint8_t*) NULL;
}

size_t  sizeOfUartReqPack(UartReqPackKey_t key){

	switch(key){
	case UartReqPack_EncoderConfig:
		return (size_t) sizeof(ReqPackEncoderConfig_t);
	case UartReqPack_EncoderDepth:
		return (size_t) sizeof(ReqPackEncoderDepth_t);
	case UartReqPack_EncoderStatus:
		return (size_t) sizeof(ReqPackEncoderStatus_t);
    case UartReqPack_Debug:
        return (size_t) sizeof(ReqPackEncoderDepth_t);
	default:
		break;
	}
	return 0;
}

size_t  sizeOfUartResPack(UartResPackKey_t key){

	switch(key){
	case UartResPack_EncoderConfig:
		return (size_t) sizeof(ResPackEncoderConfig_t);
	case UartResPack_EncoderDepth:
		return (size_t) sizeof(ResPackEncoderDepth_t);
	case UartResPack_EncoderStatus:
		return (size_t) sizeof(ResPackEncoderStatus_t);
    case UartResPack_Debug:
        return (size_t) sizeof(ResPackEncoderDepth_t);
	default:
		break;
	}
	return 0;
}
uint16_t crc16_modbus(const uint8_t* data, uint32_t len) //CRC16-IBM
{
	uint16_t crc = 0xFFFF;

	for(uint32_t i = 0; i < len; i++)
	{
		crc ^= data[i];

		for(uint8_t bit = 0; bit < 8; bit++)
		{
			if (crc & 0x0001)
				crc = (crc  >> 1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}
	return crc;
}
void crc_fill(uint8_t* data, uint32_t len)
{
	uint16_t crc = crc16_modbus(data, len - 2);
	data[len - 1] = (crc >> 8) & 0xFF ;  // Msb
    data[len - 2] = crc & 0xFF;          // Lsb
}

uint8_t crc_is_valid(uint8_t* data, uint32_t len)
{
   uint16_t checkCrc = crc16_modbus(data, len-2);
   uint8_t crcMsb =  (checkCrc >> 8) & 0xFF;  //Mbs
   uint8_t crcLsb = checkCrc & 0xFF;          //LsB
   if(data[len - 2] == crcLsb  && data[len - 1] == crcMsb)
	   return 1;

   return 0;
}
