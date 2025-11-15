#include "transmit.h"
#include "Uart_protocol.h"
#include "qdebug.h"
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
    memset(Uart.ReqPackEncoderConfig.reserved, 0, sizeof(Uart.ReqPackEncoderConfig.reserved));
    crc_fill((uint8_t *)&Uart.ReqPackEncoderConfig,sizeof(ReqPackEncoderConfig_t));
    memcpy(byteArray.data(), &Uart.ReqPackEncoderConfig,sizeof(ReqPackEncoderConfig_t));
    emit sendData(byteArray);
}

void Transmit::sendEncoderStatus(uint8_t encoderStatus)
{
    QByteArray byteArray(32,0);
    Uart.ReqPackEncoderStatus.sop = UART_SOP;
    Uart.ReqPackEncoderStatus.key = UartReqPack_EncoderStatus;
    Uart.ReqPackEncoderStatus.seq = uartSeq();
    Uart.ReqPackEncoderStatus.encoderStatus = encoderStatus;
    memset(Uart.ReqPackEncoderStatus.reserved, 0, sizeof(Uart.ReqPackEncoderStatus.reserved));
    crc_fill((uint8_t *)&Uart.ReqPackEncoderStatus,sizeof(ReqPackEncoderStatus_t));
    memcpy(byteArray.data(),&Uart.ReqPackEncoderStatus,sizeof(ReqPackEncoderStatus_t));
    emit sendData(byteArray);
}

void Transmit::sendEncoderDetph(uint8_t encoderDepth)
{
    QByteArray byteArray(32,0);
    Uart.ReqPackEncoderDepth.sop = UART_SOP;
    Uart.ReqPackEncoderDepth.key = UartReqPack_EncoderDepth;
    Uart.ReqPackEncoderDepth.seq = uartSeq();
    Uart.ReqPackEncoderDepth.depth = encoderDepth;
    Uart.ReqPackEncoderDepth.speed = 0;
    memset(Uart.ReqPackEncoderDepth.reserved, 0, sizeof(Uart.ReqPackEncoderDepth.reserved));
    crc_fill((uint8_t *)&Uart.ReqPackEncoderDepth,sizeof(ReqPackEncoderDepth_t));
    memcpy(byteArray.data(),&Uart.ReqPackEncoderDepth,sizeof(ReqPackEncoderDepth_t));
    emit sendData(byteArray);
}

uint8_t Transmit::uartSeq()
{
    if(seq != 255)
        return seq +1;

     return seq = 0;
}



