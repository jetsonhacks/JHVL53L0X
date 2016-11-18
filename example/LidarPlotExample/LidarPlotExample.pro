#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T15:14:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LidarPlotExample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    VL53L0X.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    VL53L0X.h

FORMS    += mainwindow.ui
