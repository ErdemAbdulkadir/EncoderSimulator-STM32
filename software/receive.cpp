#include "receive.h"
#include "uart_protocol.h"

UartResPackKey_t resPackKey;
extern uart_t Uart;
Receive::Receive(QObject *parent)
    :QObject{parent}
{
}

void Receive::progressReceiveData(uint8_t *data)
{
    QByteArray arr(reinterpret_cast<char*>(data), 32);
    qDebug().noquote() << "RX:" << arr.toHex(' ').toUpper();
    uint8_t key, sz, keyIndex = 1;
    uint8_t* res;
    key = data[keyIndex];
    qDebug() <<"key:" << key;
    resPackKey = static_cast<UartResPackKey_t>(key);
    res = uartResPackKey(resPackKey);
    if(!res)
    {
        qDebug() <<"res pointer is null ";
        return;
    }
    sz  = sizeOfUartResPack(resPackKey);
    qDebug() << "size" << sz;
    if(!crc_is_valid(data,sz))
    {
        qDebug() << "crc error";
        return;
    }
    memcpy(res, data,sz);
    switch (key) {
    case UartResPack_EncoderConfig:
        break;

    case UartResPack_EncoderDepth:
    {
        int32_t depth  =  Uart.ResPackEncoderDepth.depth;
        uint16_t speed =  Uart.ResPackEncoderDepth.speed;
        emit sendDepthData(depth, speed);
        break;
    }
    case UartResPack_EncoderStatus:
        break;
    case UartResPack_Debug:
    {
        uint8_t len = Uart.ResPackDebug.msgLen;
        QString msg ="[STM]: ";
        for(uint8_t i = 0; i < len; i++)
        {
            msg  += QChar(Uart.ResPackDebug.msg[i]);
        }
        qDebug() << "msg:" << msg;
        emit sendLog(msg);
        msg.clear();
    }
    default:
        break;
    }
}
