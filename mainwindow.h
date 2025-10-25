#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialportmanager.h"
#include "uart_protocol.h"
#include "transmit.h"

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
    Transmit transmit;
};
#endif // MAINWINDOW_H
