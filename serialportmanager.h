#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include "transmit.h"

class
    SerialPortManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    bool openSerialPort(const QString comPort, int baudRate);
    bool closeSerialPort();
    struct serialOption {
        QString text;
        int value;
    };
    struct serialComport {
        QString portName;
    };
    static QVector<serialOption> getStopBitList();
    static QVector<serialOption> getParityList();
    static QVector<serialOption> getBaudRateList();
    static QVector<serialComport> getComPortlist();
    //void transmitData(uint8_t cmd, QByteArray &data);
    quint8 calculateCRC(const QByteArray &data);
    void goAddressData(uint32_t address);
    void sendWriteData(uint32_t address, QByteArray &data);
    void progressData(QByteArray rxData);
public slots:
    void sendData(QByteArray data);
signals:
    void dataReceived(uint8_t* data);



private:
    void handleReadyRead();
    QSerialPort serial;
    Transmit  trnsmt;
    uint8_t rxBuffer[256];
    uint8_t head = 0, tail = 0;
    uint8_t packetSize = 32;
    uint8_t packet[32];
};

#endif // SERIALPORTMANAGER_H
