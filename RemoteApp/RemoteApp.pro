#-------------------------------------------------
#
# Project created by QtCreator 2017-02-06T07:59:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socketclient.cpp

HEADERS  += mainwindow.h \
    socketclient.h

FORMS    += mainwindow.ui
