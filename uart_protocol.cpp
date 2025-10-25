#include "Uart_protocol.h"
#include "stdio.h"

uart_t Uart;
CRC_FILL_IMP()
CRC_IS_VALID_IMP()

uint8_t* uartReqPackKey(UartReqPackKey_t key)
{
    switch(key){
    case UartReqPack_EncoderConfig:
        return (uint8_t*) &Uart.ReqPackEncoderConfig;
    case UartReqPack_EncoderDepth:
        return (uint8_t*) &Uart.ReqPackEncoderDepth;
    case UartReqPack_EncoderStatus:
        return (uint8_t*) &Uart.ReqPackEncoderStatus;
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
    default:
        break;
    }
    return 0;
}
