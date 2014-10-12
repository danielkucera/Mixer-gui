#-------------------------------------------------
#
# Project created by QtCreator 2013-10-20T11:46:28
#
#-------------------------------------------------

QT += core gui
QT += network
QT += webkit webkitwidgets
QT += multimedia multimediawidgets
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mixer
TEMPLATE = app
QMAKE_CXXFLAGS=-std=c++11 -O2

LIBS += -lturbojpeg


SOURCES += main.cpp\
        mainwindow.cpp \
    capture.cpp \
    buffer.cpp \
    preview.cpp \
    fader.cpp \
    save.cpp \
    load.cpp \
    overlay.cpp \
    hdmi.cpp \
    webview.cpp \
    overlayworker.cpp \
    hdmireciever.cpp \
    externaloutput.cpp \
    pip.cpp \
    pipworker.cpp \
    externalinput.cpp

HEADERS  += mainwindow.h \
    capture.h \
    buffer.h \
    preview.h \
    fader.h \
    save.h \
    load.h \
    overlay.h \
    hdmi.h \
    webview.h \
    overlayworker.h \
    hdmireciever.h \
    externaloutput.h \
    pip.h \
    pipworker.h \
    externalinput.h

FORMS    += mainwindow.ui \
    input.ui \
    capture.ui \
    preview.ui \
    fader.ui \
    load.ui \
    overlay.ui \
    hdmi.ui \
    webview.ui \
    externaloutput.ui \
    pip.ui \
    externalinput.ui

OTHER_FILES += \
    README.md
