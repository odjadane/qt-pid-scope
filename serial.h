#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>

class Serial : public QWidget {
    Q_OBJECT

public:
    Serial();
    ~Serial();

private:
    bool stateIsReading;
    QSerialPort *uartPort;

    QString dataExtracted;

signals:
    void dataReceived(QString);

private slots:
    void onConnect(QString, int, int, int, int);
    void onDisconnect();
    void onOpenLoop(double);
    void onClosedLoop(double, double, double, double);
    void onReadyRead();
};
#endif // SIDEBAR_H
