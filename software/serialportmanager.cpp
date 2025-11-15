#include "serialportmanager.h"
#include "uart_protocol.h"

UartResPackKey_t ResPackKey;
#define RX_BUFFER_SIZE  128
SerialPortManager::SerialPortManager(QObject *parent)
    : QObject(parent)
{
     connect(&serial, &QSerialPort::readyRead, this,
             &SerialPortManager::handleReadyRead);
     connect(&trnsmt, &Transmit::sendData,
                      this, &SerialPortManager::sendData);
}

bool SerialPortManager::openSerialPort(const QString comPort, int baudRate)
{
    serial.setPortName(comPort);
    serial.setBaudRate(baudRate);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setParity(QSerialPort::NoParity);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    if(!serial.open(QIODevice::ReadWrite))
    {
        qDebug()  << "comport is not open";
        return false;
    }
    qDebug()  << "comport is  open";
    return true;
}

bool SerialPortManager::closeSerialPort()
{
    if(serial.isOpen())
    {   serial.close();
        qDebug()  << "comport is close";
        return true;
    }
    return false;
}

void SerialPortManager::progressData(QByteArray rxData)
{
    for (int i = 0; i < rxData.size(); i++)
    {
        rxBuffer[head] = static_cast<uint8_t>(rxData[i]);
        head = (head + 1) % RX_BUFFER_SIZE;
        if(rxCount < RX_BUFFER_SIZE)
            rxCount++;
        else
            tail = (tail + 1) % RX_BUFFER_SIZE;

    }
    while (rxCount >= packetSize)
    {
        for (uint8_t i = 0; i < packetSize; i++)
        {
            packet[i] = rxBuffer[tail];
            tail = (tail + 1) % RX_BUFFER_SIZE;
        }
        rxCount -= packetSize;
        if(packet[0] != UART_SOP)
            continue;
        emit dataReceived(packet);
    }
}

void SerialPortManager::sendData(const QByteArray &data)
{
    qint64 byteWritten =  serial.write(data);
    if(byteWritten != data.size())
        qDebug() << "veri gönderilemdi";

     qDebug() << "veri gönderildi";
}

void SerialPortManager::handleReadyRead()
{

    QByteArray data = serial.readAll();
    progressData(data);
}

// QVector<SerialPortManager::serialOption> SerialPortManager::getStopBitList()
// {
//     QVector<serialOption> list;
//     list.append({"OneStop", QSerialPort::OneStop});
//     list.append({"OneAndHalfStop", QSerialPort::OneAndHalfStop});
//     list.append({"TwoStop",QSerialPort::TwoStop});
//     return list;
// }

// QVector<SerialPortManager::serialOption> SerialPortManager::getParityList()
// {
//     QVector<serialOption> list;
//     list.append({"NoParity", QSerialPort::NoParity});
//     list.append({"EvenParity", QSerialPort::EvenParity});
//     list.append({"OddParity",QSerialPort::OddParity});
//     return list;
// }

// QVector<SerialPortManager::serialOption> SerialPortManager::getBaudRateList()
// {
//     QVector<serialOption> list;
//     list.append({"9600",9600});
//     list.append({"115200", 115200});
//     return list;
// }

QVector<SerialPortManager::serialComport> SerialPortManager::getComPortlist()
{
    QVector<serialComport> list;
    QList<QSerialPortInfo> serialPortsInfos = QSerialPortInfo::availablePorts();
    for( const QSerialPortInfo &info : serialPortsInfos)
        list.append({info.portName()});
    return list;
}
