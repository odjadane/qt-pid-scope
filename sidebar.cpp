#include "sidebar.h"

WidgetSideBar::WidgetSideBar(){
    listPortsAvailable = QSerialPortInfo::availablePorts();
    isCommandClosedLoop = false;

    setupInterface();
    setupSignals();

    _disableWidgetsNoPort();
    _disableWidgetsLoop();
}

void WidgetSideBar::setupInterface(){
    /* ************
     * *** Port ***
     * ************ */
    labelPortName = new QLabel("Port");
    comboPortName = new QComboBox;
    for (QSerialPortInfo port : listPortsAvailable)
            comboPortName->addItem(port.portName());

    labelPortBaud = new QLabel("Baud");
    comboPortBaud = new QComboBox;
    comboPortBaud->addItem("1200");
    comboPortBaud->addItem("2400");
    comboPortBaud->addItem("4800");
    comboPortBaud->addItem("9600");
    comboPortBaud->addItem("19200");
    comboPortBaud->addItem("38400");
    comboPortBaud->addItem("57600");
    comboPortBaud->addItem("115200");
    comboPortBaud->setCurrentIndex(3);

    labelPortData = new QLabel("Data");
    comboPortData = new QComboBox;
    comboPortData->addItem("8 bits");
    comboPortData->addItem("7 bits");

    labelPortParity = new QLabel("Parity");
    comboPortParity = new QComboBox;
    comboPortParity->addItem("none");
    comboPortParity->addItem("odd");
    comboPortParity->addItem("even");

    labelPortStop = new QLabel("Stop");
    comboPortStop = new QComboBox;
    comboPortStop->addItem("1 bit");
    comboPortStop->addItem("2 bits");

    buttonConnect = new QPushButton("Connect");
    buttonDisconnect = new QPushButton("Disconnect");
    buttonDisconnect->setEnabled(false);

    gridPort = new QGridLayout;
    gridPort->addWidget(labelPortName, 0, 0);
    gridPort->addWidget(comboPortName, 0, 1);
    gridPort->addWidget(labelPortBaud, 1, 0);
    gridPort->addWidget(comboPortBaud, 1, 1);
    gridPort->addWidget(labelPortData, 2, 0);
    gridPort->addWidget(comboPortData, 2, 1);
    gridPort->addWidget(labelPortParity, 3, 0);
    gridPort->addWidget(comboPortParity, 3, 1);
    gridPort->addWidget(labelPortStop, 4, 0);
    gridPort->addWidget(comboPortStop, 4, 1);
    gridPort->addWidget(buttonConnect, 5, 0, 1, 2);
    gridPort->addWidget(buttonDisconnect, 6, 0, 1, 2);

    groupPort = new QGroupBox("Port settings");
    groupPort->setLayout(gridPort);

    /* *******************
     * *** Axes limits ***
     * ******************* */
    labelX = new QLabel("Points");
    spinX = new QSpinBox;
    spinX->setRange(0, 10000);
    spinX->setValue(100);
    spinX->setSingleStep(100);

    labelYMin = new QLabel("ymin");
    spinYMin = new QDoubleSpinBox;
    spinYMin->setRange(-1000, 1000);
    spinYMin->setValue(-1);
    spinYMin->setSingleStep(0.1);
    spinYMin->setDecimals(1);

    labelYMax = new QLabel("ymax");
    spinYMax = new QDoubleSpinBox;
    spinYMax->setRange(-1000, 1000);
    spinYMax->setValue(6);
    spinYMax->setSingleStep(0.1);
    spinYMax->setDecimals(1);

    buttonReset = new QPushButton("Reset");

    gridPlot = new QGridLayout();
    gridPlot->addWidget(labelX, 0, 0);
    gridPlot->addWidget(spinX, 0, 1);
    gridPlot->addWidget(labelYMin, 1, 0);
    gridPlot->addWidget(spinYMin, 1, 1);
    gridPlot->addWidget(labelYMax, 2, 0);
    gridPlot->addWidget(spinYMax, 2, 1);
    gridPlot->addWidget(buttonReset, 3, 0, 1, 2);

    groupPlot = new QGroupBox("Plot settings");
    groupPlot->setLayout(gridPlot);

    /* ***************
     * *** Command ***
     * *************** */
    radioOpenLoop = new QRadioButton("Open-loop");
    radioOpenLoop->setChecked(true);
    radioClosedLoop = new QRadioButton("Closed-loop");
    hboxLoop = new QHBoxLayout();
    hboxLoop->addWidget(radioOpenLoop);
    hboxLoop->addWidget(radioClosedLoop);

    labelInput = new QLabel("Input");
    lineInput = new QLineEdit("0.0");
    labelSetpoint = new QLabel("Setpoint");
    lineSetpoint = new QLineEdit("0.0");
    labelProportional = new QLabel("Proportional");
    lineProportional = new QLineEdit("0.0");
    labelIntegral = new QLabel("Integral");
    lineIntegral = new QLineEdit("0.0");
    labelDerivative = new QLabel("Derivative");
    lineDerivative = new QLineEdit("0.0");
    buttonRun = new QPushButton("Run");

    gridCommand = new QGridLayout();
    gridCommand->addWidget(labelInput, 0, 0);
    gridCommand->addWidget(lineInput, 0, 1);
    gridCommand->addWidget(labelSetpoint, 1, 0);
    gridCommand->addWidget(lineSetpoint, 1, 1);
    gridCommand->addWidget(labelProportional, 2, 0);
    gridCommand->addWidget(lineProportional, 2, 1);
    gridCommand->addWidget(labelIntegral, 3, 0);
    gridCommand->addWidget(lineIntegral, 3, 1);
    gridCommand->addWidget(labelDerivative, 4, 0);
    gridCommand->addWidget(lineDerivative, 4, 1);
    gridCommand->setEnabled(false);

    vboxCommand = new QVBoxLayout();
    vboxCommand->addLayout(hboxLoop);
    vboxCommand->addLayout(gridCommand);
    vboxCommand->addWidget(buttonRun);

    groupCommand = new QGroupBox("Command");
    groupCommand->setLayout(vboxCommand);
    groupCommand->setCheckable(true);

    /* **************
     * *** Layout ***
     * ************** */
    vbox = new QVBoxLayout;
    vbox->addWidget(groupPort);
    vbox->addStretch();
    vbox->addWidget(groupCommand);
    vbox->addStretch();
    vbox->addWidget(groupPlot);

    this->setLayout(vbox);
}

void WidgetSideBar::setupSignals() {
    connect(buttonConnect, SIGNAL(clicked()),
            this, SLOT(onButtonConnectClicked()));
    connect(buttonDisconnect, SIGNAL(clicked()),
            this, SLOT(onButtonDisconnectClicked()));
    connect(radioOpenLoop, SIGNAL(toggled(bool)),
            this, SLOT(onRadioLoopToggled()));
    connect(radioClosedLoop, SIGNAL(toggled(bool)),
            this, SLOT(onRadioLoopToggled()));
    connect(lineInput, SIGNAL(returnPressed()),
            buttonRun, SLOT(click()));
    connect(lineSetpoint, SIGNAL(returnPressed()),
            buttonRun, SLOT(click()));
    connect(lineProportional, SIGNAL(returnPressed()),
            buttonRun, SLOT(click()));
    connect(lineIntegral, SIGNAL(returnPressed()),
            buttonRun, SLOT(click()));
    connect(lineDerivative, SIGNAL(returnPressed()),
            buttonRun, SLOT(click()));
    connect(buttonRun, SIGNAL(clicked()),
            this, SLOT(onButtonRunClicked()));
    connect(buttonReset, SIGNAL(clicked()),
            this, SLOT(onButtonResetClicked()));
    connect(spinX, SIGNAL(valueChanged(int)),
            this, SLOT(onSpinValueChanged()));
    connect(spinYMin, SIGNAL(valueChanged(double)),
            this, SLOT(onSpinValueChanged()));
    connect(spinYMax, SIGNAL(valueChanged(double)),
            this, SLOT(onSpinValueChanged()));
}

void WidgetSideBar::onButtonConnectClicked(){
    QString portName = comboPortName->currentText();
    int baudRate = comboPortBaud->currentText().toInt();
    int dataBits, parity, stopBits;

    if (comboPortData->currentIndex() == 0) {
        dataBits = 8;
    } else {
        dataBits = 7;
    }

    if (comboPortParity->currentIndex() == 0) {
        parity = 0;
    } else if (comboPortParity->currentIndex() == 1) {
        parity = 1;
    } else {
        parity = 2;
    }

    if (comboPortStop->currentIndex() == 0) {
        stopBits = 1;
    } else {
        stopBits = 2;
    }

    emit sendButtonConnectClicked(portName, baudRate, dataBits, parity, stopBits);

    buttonConnect->setEnabled(false);
    buttonDisconnect->setEnabled(true);
}

void WidgetSideBar::onButtonDisconnectClicked() {
    emit sendButtonDisconnectClicked();

    buttonConnect->setEnabled(true);
    buttonDisconnect->setEnabled(false);
}

void WidgetSideBar::onRadioLoopToggled() {
    if(radioOpenLoop->isChecked()) {
        isCommandClosedLoop = false;
    } else {
        isCommandClosedLoop = true;
    }
    _disableWidgetsLoop();
}

void WidgetSideBar::onButtonRunClicked() {
    if(isCommandClosedLoop) {
        double sp, kp, ki, kd;
        sp = lineSetpoint->text().toDouble();
        kp = lineProportional->text().toDouble();
        ki = lineIntegral->text().toDouble();
        kd = lineDerivative->text().toDouble();
        emit sendClosedLoop(sp, kp, ki, kd);
    } else {
        double input;
        input = lineInput->text().toDouble();
        emit sendOpenLoop(input);
    }
}

void WidgetSideBar::onSpinValueChanged(){
    emit sendSpinValueChanged(spinX->value(),
                              spinYMin->value(),
                              spinYMax->value());
}

void WidgetSideBar::onButtonResetClicked() {
    emit sendButtonResetClicked();
}

void WidgetSideBar::_disableWidgetsNoPort(){
    if(listPortsAvailable.length() == 0) {
        groupPort->setEnabled(false);
        groupCommand->setEnabled(false);
        groupPlot->setEnabled(false);

        QMessageBox messageBox;
        messageBox.setText("No serial port was found.");
        messageBox.setStandardButtons(QMessageBox::Close);
        messageBox.exec();
    }
}

void WidgetSideBar::_disableWidgetsLoop() {
    for(int i = 0; i < gridCommand->count(); i++){
        if(i < 2) {
            gridCommand->itemAt(i)->widget()->setEnabled(!isCommandClosedLoop);
        } else {
            gridCommand->itemAt(i)->widget()->setEnabled(isCommandClosedLoop);
        }
    }
}
