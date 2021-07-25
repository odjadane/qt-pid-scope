#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QMessageBox>
#include <QSerialPortInfo>


class WidgetSideBar : public QWidget {
    Q_OBJECT

public:
    WidgetSideBar();

private:
    void setupInterface();
    void setupSignals();

    void _disableWidgetsNoPort();
    void _disableWidgetsLoop();

    QList<QSerialPortInfo> listPortsAvailable;
    bool isCommandClosedLoop;

    QLabel *labelPortName;
    QLabel *labelPortBaud;
    QLabel *labelPortData;
    QLabel *labelPortParity;
    QLabel *labelPortStop;
    QComboBox *comboPortName;
    QComboBox *comboPortBaud;
    QComboBox *comboPortData;
    QComboBox *comboPortParity;
    QComboBox *comboPortStop;
    QPushButton *buttonConnect;
    QPushButton *buttonDisconnect;
    QGridLayout *gridPort;
    QGroupBox *groupPort;

    QRadioButton *radioOpenLoop;
    QRadioButton *radioClosedLoop;
    QHBoxLayout *hboxLoop;
    QLabel *labelInput;
    QLineEdit *lineInput;
    QLabel *labelSetpoint;
    QLineEdit *lineSetpoint;
    QLabel *labelProportional;
    QLineEdit *lineProportional;
    QLabel *labelIntegral;
    QLineEdit *lineIntegral;
    QLabel *labelDerivative;
    QLineEdit *lineDerivative;
    QPushButton *buttonRun;
    QGridLayout *gridCommand;
    QVBoxLayout *vboxCommand;
    QGroupBox *groupCommand;

    QLabel *labelX;
    QLabel *labelYMin;
    QLabel *labelYMax;
    QSpinBox *spinX;
    QDoubleSpinBox *spinYMin;
    QDoubleSpinBox *spinYMax;
    QPushButton *buttonReset;
    QGridLayout *gridPlot;
    QGroupBox *groupPlot;

    QVBoxLayout *vbox;

signals:
    void sendButtonConnectClicked(QString, int, int, int, int);
    void sendButtonDisconnectClicked();
    void sendButtonResetClicked();
    void sendSpinValueChanged(int, double, double);
    void sendOpenLoop(double);
    void sendClosedLoop(double, double, double, double);

private slots:
    void onButtonConnectClicked();
    void onButtonDisconnectClicked();
    void onButtonResetClicked();
    void onButtonRunClicked();
    void onRadioLoopToggled();
    void onSpinValueChanged();
};
#endif // SIDEBAR_H
