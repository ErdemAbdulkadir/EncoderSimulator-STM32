#ifndef RECEIVE_H
#define RECEIVE_H

#include <serialportmanager.h>
#include <QObject>

class Receive : public QObject
{
    Q_OBJECT
public:
    explicit Receive(QObject* parent = nullptr);
public slots:
    void progressReceiveData(uint8_t* data);
private:
    SerialPortManager sPortManger;
};

#endif // RECEIVE_H
