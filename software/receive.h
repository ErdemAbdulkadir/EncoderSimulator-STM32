#ifndef RECEIVE_H
#define RECEIVE_H

#include <serialportmanager.h>
#include <QObject>

class Receive : public QObject
{
    Q_OBJECT
public:
    explicit Receive(QObject* parent = nullptr);
    void progressReceiveData(uint8_t* data);
signals:
    void sendDepthData(int32_t value, uint16_t speed);
    void sendLog(QString msg);
private:

};

#endif // RECEIVE_H
