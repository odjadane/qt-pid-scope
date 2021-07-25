#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qcustomplot.h>

class WidgetPlot : public QWidget {
    Q_OBJECT

public:
    WidgetPlot();

private:
    void setupInterface();
    void setupSignals();

    QCustomPlot *fig;
    QLabel *labelCoordinates;
    QLabel *labelAuthor;
    QHBoxLayout *hboxLabels;
    QVBoxLayout *layout;

    int indexLastPoint;
    int settingsPoints;
    double settingsYMin;
    double settingsYMax;

private slots:
    void updateAxes(int, double, double);
    void resetPlot();
    void updatePlot(QString);
    void onSelectionChanged(bool);
};
#endif // PLOT_H
