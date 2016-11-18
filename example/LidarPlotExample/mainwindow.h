#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "VL53L0X.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    VL53L0X *sensor ;
    ~MainWindow();
    void setupDistancePlot(QCustomPlot *customPlot) ;

private slots:
  void distanceDataSlot() ;

private:
    Ui::MainWindow *ui;
    QString demoName;
    QTimer distanceDataTimer ;

};

#endif // MAINWINDOW_H
