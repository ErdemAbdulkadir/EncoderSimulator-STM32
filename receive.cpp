#include "receive.h"
#include "uart_protocol.h"

UartResPackKey_t resPackKey;

Receive::Receive(QObject *parent)
    :QObject{parent}
{
    connect(&sPortManger, &SerialPortManager::dataReceived,this,
           &Receive::progressReceiveData);
}

void Receive::progressReceiveData(uint8_t *data)
{
    uint8_t key, sz, keyIndex = 1;
    uint8_t* res;
    key = data[keyIndex];
    resPackKey = static_cast<UartResPackKey_t>(key);
    res = uartResPackKey(resPackKey);
    if(!res)
    {
        qDebug() <<"res pointer is null ";
        return;
    }
    sz  = sizeOfUartResPack(resPackKey);
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
        break;

    case UartResPack_EncoderStatus:
        break;
    default:
        break;
    }

}
