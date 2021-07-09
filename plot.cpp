#include "plot.h"

WidgetPlot::WidgetPlot(){
    indexLastPoint = 0;
    settingsPoints = 100;
    settingsYMin = -1;
    settingsYMax = 6;

    setupInterface();
    setupSignals();
}

void WidgetPlot::setupInterface() {
    /* ***************
     * *** Figures ***
     * *************** */
    // Instantiate - graph(0): input / graph(1): output
    fig = new QCustomPlot;
    fig->addGraph();
    fig->addGraph();
    // Legends
    fig->legend->setVisible(true);
    fig->graph(0)->setName("Input");
    fig->graph(1)->setName("Ouput");
    // Styles : Line: Blue/Red - Points : Cross, 2px
    fig->graph(0)->setLineStyle(QCPGraph::lsStepLeft);
    fig->graph(0)->setPen(QPen(Qt::blue));
    fig->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 1));
    fig->graph(1)->setLineStyle(QCPGraph::lsLine);
    fig->graph(1)->setPen(QPen(Qt::red));
    fig->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 1));
    // Selection style : Black - 5px
    fig->graph(1)->selectionDecorator()->setPen(QPen(Qt::black, 5));
    // Definie initial limits of axes
    fig->xAxis->setRange(0, settingsPoints);
    fig->yAxis->setRange(settingsYMin, settingsYMax);
    // Behaviour for data selection : none for input / select a point for output
    fig->setInteraction(QCP::iSelectPlottables, true);
    fig->graph(0)->setSelectable(QCP::stNone);
    fig->graph(1)->setSelectable(QCP::stSingleData);

    /* **************
     * *** Labels ***
     * ************** */
    // Label to display coordinates
    labelCoordinates = new QLabel("No selected points");
    labelCoordinates->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    // Label to display author
    labelAuthor = new QLabel(
                "<a href=\"https://github.com/odjadane/qt-pid-scope\">"
                "github.com/odjadane/qt-pid-scope</a>");
    labelAuthor->setTextFormat(Qt::RichText);
    labelAuthor->setTextInteractionFlags(Qt::TextBrowserInteraction);
    labelAuthor->setOpenExternalLinks(true);
    // Horizontal layout for labels
    hboxLabels = new QHBoxLayout();
    hboxLabels->addWidget(labelCoordinates, 1);
    hboxLabels->addWidget(labelAuthor);

    /* **************
     * *** Layout ***
     * ************** */
    layout = new QVBoxLayout;
    layout->addWidget(fig, 1);
    layout->addLayout(hboxLabels);
    this->setLayout(layout);
}

void WidgetPlot::setupSignals() {
    connect(fig->graph(1), SIGNAL(selectionChanged(bool)),
            this, SLOT(onSelectionChanged(bool)));
}

void WidgetPlot::updateAxes(int x, int ymin, int ymax){
    settingsPoints = x;
    settingsYMin = ymin;
    settingsYMax = ymax;

    if (indexLastPoint > settingsPoints) {
        fig->xAxis->setRange(indexLastPoint-settingsPoints, indexLastPoint);
    } else {
        fig->xAxis->setRange(0, settingsPoints);
    }
    fig->yAxis->setRange(settingsYMin, settingsYMax);
    fig->replot();
}

void WidgetPlot::resetPlot(){
    indexLastPoint = 0;
    fig->xAxis->setRange(0, settingsPoints);
    fig->graph(0)->data()->clear();
    fig->graph(1)->data()->clear();
    fig->replot();
}

void WidgetPlot::updatePlot(QString dataExtracted) {
    QStringList dataSplitted = dataExtracted.split('/');
    double dataInput = dataSplitted.at(0).toDouble();
    double dataOutput = dataSplitted.at(1).toDouble();
    indexLastPoint++;
    if (indexLastPoint > settingsPoints) {
        fig->xAxis->setRange(indexLastPoint-settingsPoints, indexLastPoint);
    }
    fig->graph(0)->addData(indexLastPoint, dataInput);
    fig->graph(1)->addData(indexLastPoint, dataOutput);
    fig->replot(QCustomPlot::rpQueuedReplot);
}

void WidgetPlot::onSelectionChanged(bool isSelected) {
    if (isSelected) {
        QCPGraphDataContainer::const_iterator it = fig->graph(1)->data()->at(
                    fig->graph(1)->selection().dataRange().begin());
        labelCoordinates->setText(QString("Selected point : (%1, %2)").arg(
                                      QString::number(it->key),
                                      QString::number(it->value, 'f', 2)));
    } else {
        labelCoordinates->setText("No selected points");
    }
}
