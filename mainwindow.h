#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>

#include "sidebar.h"
#include "serial.h"
#include "plot.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupInterface();
    void setupSignals();

    WidgetSideBar *widgetSideBar;
    WidgetPlot *widgetPlot;
    Serial *serial;

    QHBoxLayout *hbox;
    QWidget *central;
};
#endif // MAINWINDOW_H
