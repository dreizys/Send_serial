#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include  <QSerialPort>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
private slots:
    void readSerial();

    void updateLCD(const QString);
private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray  serialData;
    QString serialBuffer;
};

#endif // DIALOG_H
