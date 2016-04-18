#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->temp_lcdNumber->display("-------");

    arduino = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "";
    serialBuffer = "";


   // qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();

    /*foreach (const QSerialPortInfo &seriaPortInfo, QSerialPortInfo::availablePorts() ){
        qDebug() << "Has vendor ID: " << seriaPortInfo.hasVendorIdentifier();
        if(seriaPortInfo.hasVendorIdentifier()){
            qDebug() << " Vendor ID: " << seriaPortInfo.vendorIdentifier();
            qDebug() << "port name is : " << seriaPortInfo.portName();
            arduino_port_name = seriaPortInfo.portName();
        }*/


    foreach (const QSerialPortInfo &seriaPortInfo, QSerialPortInfo::availablePorts() ){
               if(seriaPortInfo.hasVendorIdentifier()){
                //qDebug() << " Vendor ID: " << seriaPortInfo.vendorIdentifier();
                //qDebug() << "port name is : " << seriaPortInfo.portName();
                arduino_port_name = seriaPortInfo.portName();
                ui->comboBox->addItem(arduino_port_name);
                arduino_is_available = true;
    }
 }
    if(arduino_is_available){
        //open & configure the serial port
        //arduino->setPortName(arduino_port_name);
        QString portn = ui->comboBox->currentText();
        arduino->setPortName(portn);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));

    }
    else{
        //give error message
        QMessageBox::warning(this, "Port error", "Couldn't find the arduino");
    }

   /* for(int i = 0; i < 10; i++)
       {
           ui->comboBox->addItem("item " + QString::number(i)); //testing qcombobox
       }*/
}


Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}

void Dialog::readSerial(){
//qDebug() << "serial port works";
    if(serialBuffer.length()<5){
    serialData = arduino->readLine();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    }
   // qDebug() << serialBuffer;
    else{                                                                //bufferSplit[1] is a good value
    QStringList bufferSplit=serialBuffer.split(",");
    Dialog::updateLCD(serialBuffer);
    //qDebug() << bufferSplit;
    serialBuffer = "";
}

}


void Dialog::updateLCD(const QString sensor_reading){

    ui->temp_lcdNumber->display(sensor_reading);
}
