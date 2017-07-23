#-------------------------------------------------
#
# Project created by QtCreator 2017-06-16T11:37:00
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Quadcopter_TCP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myserver.cpp

HEADERS  += mainwindow.h \
    myserver.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
