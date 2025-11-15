#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillCbComPort();
    connect(&serialManager, &SerialPortManager::dataReceived,this,
            &MainWindow::sendDataReceived);
    connect(&receive, &Receive::sendDepthData, this,
            &MainWindow::displayDepthData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillCbComPort()
{
    ui->cmbComport->clear();
    for(const auto &item : serialManager.getComPortlist())
        ui->cmbComport->addItem(item.portName);
    ui->btnSaveConfig->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnStop->setDisabled(true);
}

void MainWindow::displayDepthData(int32_t value, uint16_t speed)
{
    ui->lcdDepth->display(value);
    ui->lcdSpeed->display(speed);
}

void MainWindow::sendDataReceived(uint8_t *data)
{
    receive.progressReceiveData(data);
}

void MainWindow::on_btnConnect_clicked()
{
    QString comPort = ui->cmbComport->currentText();
    QString connectBtnName = ui->btnConnect->text();
    bool connect;
    if( connectBtnName == "Connect")
    {
        connect = serialManager.openSerialPort(comPort,BAUD_RATE);
        if(!connect)
        {
            qDebug() << "comport is not open";
            return;
        }
        ui->btnConnect->setText("Disconnect");
        ui->btnSaveConfig->setDisabled(false);
        ui->btnStart->setDisabled(false);
        ui->btnStop->setDisabled(false);
        return;
    }
    serialManager.closeSerialPort();
    ui->btnConnect->setText("Connect");
    ui->btnSaveConfig->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnStop->setDisabled(true);
}

void MainWindow::on_btnRefresh_clicked()
{
    ui->cmbComport->clear();
    fillCbComPort();
}

void MainWindow::on_btnSaveConfig_clicked()
{
    bool isResolutionEmpty = ui->leResolution->text().isEmpty();
    if(isResolutionEmpty)
    {
        QMessageBox::warning(this,"Warning","Encoder Resolution must be empty");
        return;
    }
    bool ok;
    uint8_t resolutionValue = ui->leResolution->text().toInt(&ok);
    if(!resolutionValue)
    {
        QMessageBox::warning(this,"Warning","Encoder Resolution must be a number");
        return;
    }
    bool isDirectionSelected = (ui->radioBtnDown->isChecked() ^ ui->radioBtnUp->isChecked());
    uint8_t direction;
    if(isDirectionSelected)
    {
        if(ui->radioBtnDown->isChecked())
            direction = encoderDirectionDown;
        else if(ui->radioBtnUp->isChecked())
            direction = encoderDirectionUp;
    }
    else
    {
        QMessageBox::warning(this,"Warning","Encoder Direction must be selected");
        return;
    }
    // üçü seçili olma durumda 1 olarak döner ama üçü aynı anda seçilme durumu yok hatta ikisinin bile
    bool isSpeedSelected = ((ui->radioBtnHigh->isChecked() ^ ui->radioBtnMedium->isChecked()) ^ ui->radioBtnLow->isChecked());
    uint8_t speed;
    if(isSpeedSelected)
    {
        if(ui->radioBtnHigh->isChecked())
            speed = encoderSpeedHigh;
        else if(ui->radioBtnMedium->isChecked())
            speed = encoderSpeedMedium;
        else if(ui->radioBtnLow->isChecked())
            speed = encoderSpeedLow;
        else
           speed = encoderSpeedNone;
    }
    else
    {
        QMessageBox::warning(this,"Warning","Encoder Speed must be selected");
        return;
    }
    qDebug() << resolutionValue;
    qDebug() << direction;
    qDebug() << speed;
    serialManager.getTransmit()->sendEncoderConfig(resolutionValue, direction, speed);
}

void MainWindow::on_btnStart_clicked()
{
   serialManager.getTransmit()->sendEncoderStatus(encoderStatusStart);
   ui->btnReset->setDisabled(true);
}

void MainWindow::on_btnStop_clicked()
{
    serialManager.getTransmit()->sendEncoderStatus(encoderStatusStop);
    ui->btnReset->setEnabled(true);
}

void MainWindow::on_btnReset_clicked()
{
    ui->lcdDepth->display(0);
    serialManager.getTransmit()->sendEncoderDetph(0);
}

