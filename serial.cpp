#include "serial.h"

Serial::Serial() {
    stateIsReading = false;
    uartPort = new QSerialPort();
    connect(uartPort, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
}

Serial::~Serial() {
    // Close connection
    if(uartPort->isOpen()) {
        uartPort->close();
    }
}

void Serial::onConnect(QString portName,
                       int baudRate,
                       int dataBits,
                       int parity,
                       int stopBits){
    if(!uartPort->isOpen()) {
        uartPort->setPortName(portName);
        uartPort->open(QSerialPort::ReadWrite);
        uartPort->setBaudRate(baudRate);

        if (dataBits == 8) {
            uartPort->setDataBits(QSerialPort::Data8);
        } else {
            uartPort->setDataBits(QSerialPort::Data7);
        }

        if (parity == 0) {
            uartPort->setParity(QSerialPort::NoParity);
        } else if (parity == 1) {
            uartPort->setParity(QSerialPort::OddParity);
        } else {
            uartPort->setParity(QSerialPort::EvenParity);
        }

        if (stopBits == 1) {
            uartPort->setStopBits(QSerialPort::OneStop);
        } else {
            uartPort->setStopBits(QSerialPort::TwoStop);
        }
    }
}

void Serial::onDisconnect(){
    uartPort->close();
}

void Serial::onOpenLoop(double input) {
    QString command = "(OL/" + QString::number(input, 'f', 2) + ")";
    if(uartPort->isWritable())
        uartPort->write(command.toStdString().c_str());
}

void Serial::onClosedLoop(double sp, double kp, double ki, double kd) {
    QString command = "(CL/" +
            QString::number(sp, 'f', 2) + "/" +
            QString::number(kp, 'f', 2) + "/" +
            QString::number(ki, 'f', 2) + "/" +
            QString::number(kd, 'f', 2) + ")";
    if(uartPort->isWritable())
        uartPort->write(command.toStdString().c_str());
}

void Serial::onReadyRead() {
    QByteArray uartBuffer = uartPort->readAll();
    if (!uartBuffer.isEmpty()) {
        char *ptrBuffer = uartBuffer.data();
        for (int i = 0; ptrBuffer[i] != '\0'; i++) {
            if (stateIsReading == false){
                if (ptrBuffer[i] == '<') {
                    stateIsReading = true;
                    dataExtracted.clear();
                }
            } else {
                if (ptrBuffer[i] == '>') {
                    stateIsReading = false;
                    emit dataReceived(dataExtracted);
                } else if (isdigit(ptrBuffer[i]) ||
                           ptrBuffer[i] == '/' ||
                           ptrBuffer[i] == '-' ||
                           ptrBuffer[i] == '.') {
                    dataExtracted.append(ptrBuffer[i]);
                }
            }
        }
    }
}
