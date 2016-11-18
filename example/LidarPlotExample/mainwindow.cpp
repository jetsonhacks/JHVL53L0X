#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdio>
#include <iostream>

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(" MainWindow {background-color: black;}");
    // setStyleSheet(" MainWindow {background-image:  url(:/images/Wallpaper), url(:/images/BorderImage);}");

    ui->distanceLabel->setStyleSheet("QLabel { background-color : black; color : white; }");

    ui->consoleLog->setMaximumBlockCount(14) ;
    ui->consoleLog->setCenterOnScroll(false) ;

    sensor = new VL53L0X();
    // Open the sensor
    if (!sensor->openVL53L0X()) {
        // Trouble
        std::cout << "Unable to open VL53L0X" << std::endl ;
        exit(-1) ;
    }
    sensor->init();
    sensor->setTimeout(500);
#if defined LONG_RANGE
    // lower the return signal rate limit (default is 0.25 MCPS)
    sensor.setSignalRateLimit(0.1);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
    // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
    // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
#endif
    setupDistancePlot(ui->distancePlot);
#if 0
    while (1) {
        float distance = sensor->readRangeSingleMillimeters();
        if (sensor->timeoutOccurred()) {
            std::cout << "Sensor timeout!" << std::endl;
        } else {
            std::cout << "Distance: " << distance << std::endl;
        }
    }
#endif

}

MainWindow::~MainWindow()
{
    sensor->closeVL53L0X();
    delete ui;
}

void MainWindow::setupDistancePlot(QCustomPlot *customPlot) {
    customPlot->addGraph() ;
    //QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    //customPlot->addPlottable(bars1);
    //bars1->setWidth(6);
    // bars1->setData(x3, y3);
    //bars1->setPen(Qt::NoPen);
    //bars1->setBrush(QColor(10, 140, 70, 160));
    customPlot->setBackground(Qt::lightGray);
    QPen greenPen = QPen(QColor(0, 128, 0, 200)) ;
    greenPen.setWidth(3) ;
    // velocityPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setPen(greenPen);

    customPlot->graph(0)->setBrush(QBrush(QColor(10, 140, 70, 160)));
    customPlot->graph(0)->setAntialiasedFill(true);



    // Setup X Axis
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();

    customPlot->yAxis->setRange(0,2200) ;
    customPlot->yAxis->setLabel("Distance") ;

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::distanceDataSlot:
    connect(&distanceDataTimer, SIGNAL(timeout()), this, SLOT(distanceDataSlot()));
    distanceDataTimer.start(0); // Interval 0 means to refresh as fast as possible

}

void MainWindow::distanceDataSlot() {
    // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    double key = 0;
#else
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        // double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
#if 0
        // jlb 11-16-16
        int distance = lidarLite->getDistance();
        int previousDistance = lidarLite->getPreviousDistance();
        // printf("Distance: %dcm\n", dist);
        int velocity = lidarLite->getVelocity();
        printf("Velocity: %08d Distance: %dcm Previous Distance: %dcm\n",velocity, distance,previousDistance);
        char resultBuffer[128] ;
        sprintf(resultBuffer," Distance: %5dcm Previous Distance: %5dcm Velocity: %08d",distance,previousDistance,velocity);
        ui->consoleLog->appendPlainText(resultBuffer);
#endif
        ui->consoleLog->ensureCursorVisible();

        int distance = sensor->readRangeSingleMillimeters();
        // The VL53L0X is 200cm max; Distance is in mm
        if (distance > 2100) {
            distance = 0 ;
        }
        if (distance > 0) {
            char resultBuffer[128] ;
            sprintf(resultBuffer," Distance: %d mm ",distance);
            ui->consoleLog->appendPlainText(resultBuffer);
        }

        // int value0 = lidarLite->getDistance() ;
        int value0 = distance ;
        // add data to bars:
        ui->distancePlot->graph(0)->addData(key, value0);
        // set data of dots:
        // remove data of lines that's outside visible range:
        ui->distancePlot->graph(0)->removeDataBefore(key-8);
        // rescale value (vertical) axis to fit the current data:
        // ui->velocityPlot->graph(0)->rescaleValueAxis();
        lastPointKey = key;

        // Set the distance label
        char buffer[32] ;
        if (distance > 0) {
            sprintf(buffer,"%5d mm",distance) ;
        } else {
            sprintf(buffer,"     ") ;
        }
        ui->distanceLabel->setText(buffer);
        //std::cout << "Distance: " << distance << std::endl ;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->distancePlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->distancePlot->replot();
}

