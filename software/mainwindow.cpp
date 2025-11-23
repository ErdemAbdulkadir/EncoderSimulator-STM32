#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings("config.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    fillCbComPort();
    connect(&serialManager, &SerialPortManager::dataReceived,this,
            &MainWindow::sendDataReceived);
    connect(&receive, &Receive::sendDepthData, this,
            &MainWindow::displayDepthData);
    connect(&receive, &Receive::sendLog, this,
            &MainWindow::sendLog);
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
    settings.setValue("depthValue", value);
}

void MainWindow::sendLog(QString msg)
{
    ui->pteLog->appendPlainText(msg);
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
            sendLogMsgQT("comport is not open");
            return;
        }
        ui->btnConnect->setText("Disconnect");
        ui->btnSaveConfig->setDisabled(false);
        ui->btnStart->setDisabled(false);
        ui->btnStop->setDisabled(false);
        readConfigValue();
        sendLogMsgQT("Connect");
        return;
    }
    serialManager.closeSerialPort();
    ui->btnConnect->setText("Connect");
    ui->btnSaveConfig->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnStop->setDisabled(true);
    sendLogMsgQT("Disconnect");
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
    settings.setValue("resolution",resolutionValue);
    settings.setValue("direction", direction);
    settings.setValue("speed", speed);
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

void MainWindow::readConfigValue()
{
    QString resolution = settings.value("resolution").toString();
    uint8_t resolutionValue = settings.value("resolution").toUInt();
    uint8_t direction = settings.value("direction").toInt();
    uint8_t speed = settings.value("speed").toUInt();
    int32_t depth = settings.value("depthValue").toInt();

    ui->leResolution->setText(resolution);
    ui->lcdDepth->display(depth);
    if(direction == encoderDirectionUp)
        ui->radioBtnUp->setChecked(true);
    else if(direction == encoderDirectionDown)
        ui->radioBtnDown->setChecked(true);
    else
        return;

    if(speed == encoderSpeedLow)
        ui->radioBtnLow->setChecked(true);
    else if(speed == encoderSpeedMedium)
        ui->radioBtnMedium->setChecked(true);
    else if(speed == encoderSpeedHigh)
        ui->radioBtnHigh->setChecked(true);
    else
        return;

    serialManager.getTransmit()->sendEncoderConfig(resolutionValue, direction, speed);
}

void MainWindow::on_btnClearLog_clicked()
{
    ui->pteLog->clear();
}

void MainWindow::sendLogMsgQT(QString msg)
{
    msg.prepend("[QT]: ");
    ui->pteLog->appendPlainText(msg);
}

