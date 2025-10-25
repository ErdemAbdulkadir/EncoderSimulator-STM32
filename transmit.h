#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <QObject>


class Transmit : public QObject
{
    Q_OBJECT
public:
    explicit Transmit(QObject * parent = nullptr);
    void sendEncoderConfig(uint16_t resolution, uint8_t diretion, uint8_t encoderSpeed);
    void sendEncoderStatus(uint8_t encoderStatus);
signals:
    void sendData(const QByteArray data);
private:
    uint8_t uartSeq();
    uint8_t seq = 0;
};

#endif // TRANSMIT_H
