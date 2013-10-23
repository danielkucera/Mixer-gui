#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:46:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mixer
TEMPLATE = app
QMAKE_CXXFLAGS=-std=c++11



SOURCES += main.cpp\
        mainwindow.cpp \
    capture.cpp \
    preview.cpp \
    fader.cpp \
    test.cpp

HEADERS  += mainwindow.h \
    capture.h \
    preview.h \
    fader.h \
    test.h

FORMS    += mainwindow.ui \
    input.ui \
    test.ui
