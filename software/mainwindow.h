#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialportmanager.h"
#include "receive.h"
#include "uart_protocol.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fillCbComPort();
    void displayDepthData(int32_t value, uint16_t speed);
public slots:
    void sendDataReceived(uint8_t* data);
private slots:
    void on_btnConnect_clicked();
    void on_btnRefresh_clicked();
    void on_btnSaveConfig_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnReset_clicked();
private:
    Ui::MainWindow *ui;
    SerialPortManager serialManager;
    Receive           receive;
};
#endif // MAINWINDOW_H
