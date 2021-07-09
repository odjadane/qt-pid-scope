#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    serial = new Serial();
    setupInterface();
    setupSignals();
}

void MainWindow::setupInterface() {
    widgetPlot = new WidgetPlot();
    widgetSideBar = new WidgetSideBar();

    hbox = new QHBoxLayout;
    hbox->addWidget(widgetSideBar);
    hbox->addWidget(widgetPlot, 1);

    central = new QWidget;
    central->setLayout(hbox);

    this->setCentralWidget(central);
    this->showMaximized();
    this->setWindowTitle("qt-pid-scope");
}

void MainWindow::setupSignals() {
    connect(serial, SIGNAL(dataReceived(QString)),
            widgetPlot, SLOT(updatePlot(QString)));
    connect(widgetSideBar, SIGNAL(sendButtonResetClicked()),
            widgetPlot, SLOT(resetPlot()));
    connect(widgetSideBar, SIGNAL(sendSpinValueChanged(int, int, int)),
            widgetPlot, SLOT(updateAxes(int, int, int)));

    connect(widgetSideBar, SIGNAL(sendButtonConnectClicked(QString, int, int, int, int)),
            serial, SLOT(onConnect(QString, int, int, int, int)));
    connect(widgetSideBar, SIGNAL(sendButtonDisconnectClicked()),
            serial, SLOT(onDisconnect()));
    connect(widgetSideBar, SIGNAL(sendOpenLoop(double)),
            serial, SLOT(onOpenLoop(double)));
    connect(widgetSideBar, SIGNAL(sendClosedLoop(double, double, double, double)),
            serial, SLOT(onClosedLoop(double, double, double, double)));
}
