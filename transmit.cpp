#include "transmit.h"
#include "Uart_protocol.h"

extern uart_t Uart;


Transmit::Transmit(QObject *parent)
    : QObject(parent)
{
}

void Transmit::sendEncoderConfig(uint16_t resolution, uint8_t direction, uint8_t encoderSpeed)
{
    QByteArray byteArray(32,0);
    Uart.ReqPackEncoderConfig.sop = UART_SOP;
    Uart.ReqPackEncoderConfig.key = UartReqPack_EncoderConfig;
    Uart.ReqPackEncoderConfig.seq = uartSeq();
    Uart.ReqPackEncoderConfig.resolution = resolution;
    Uart.ReqPackEncoderConfig.direction  = direction;
    Uart.ReqPackEncoderConfig.encoderSpeed = encoderSpeed;
    crc_fill((uint8_t *)&Uart.ReqPackEncoderConfig,sizeof(ReqPackEncoderConfig_t));
    memcpy(byteArray.data(), &Uart.ReqPackEncoderConfig,sizeof(ReqPackEncoderConfig_t));
    emit sendData(byteArray);
}

void Transmit::sendEncoderStatus(uint8_t encoderStatus)
{
    QByteArray byteArray(32,0);
    Uart.ReqPackEncoderStatus.sop = UART_SOP;
    Uart.ReqPackEncoderStatus.key = UartReqPack_EncoderStatus;
    Uart.ReqPackEncoderStatus.sop = uartSeq();
    Uart.ReqPackEncoderStatus.encoderStatus = encoderStatus;
    crc_fill((uint8_t *)&Uart.ReqPackEncoderStatus,sizeof(ReqPackEncoderStatus_t));
    memcpy(byteArray.data(),&Uart.ReqPackEncoderStatus,sizeof(ReqPackEncoderStatus_t));
    emit sendData(byteArray);
}

uint8_t Transmit::uartSeq()
{
    if(seq != 255)
        return seq +1;

     return seq = 0;
}



