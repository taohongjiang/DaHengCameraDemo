#-------------------------------------------------
#
# Project created by QtCreator 2018-11-06T17:22:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usbCmos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/centerWidget/centerwidget.cpp \
    widgets/connectWidget/connectwidget.cpp \
    widgets/displayWidget/displaywidget.cpp

HEADERS  += mainwindow.h \
    widgets/centerWidget/centerwidget.h \
    widgets/connectWidget/connectwidget.h \
    widgets/displayWidget/displaywidget.h

FORMS    += mainwindow.ui

LIBS     += DxImageProc.lib \
            GxIAPI.lib

INCLUDEPATH += "D:\Program Files\Daheng Imavision\MER-Series\Samples\VC SDK\inc"


