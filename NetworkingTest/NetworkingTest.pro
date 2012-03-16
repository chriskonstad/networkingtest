#-------------------------------------------------
#
# Project created by QtCreator 2011-07-04T00:16:04
#
#-------------------------------------------------

QT       += core gui\
        network

TARGET = NetworkingTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

macx {
ICON = networkingtest.icns
}

win32 {
RC_FILE = networkingtest.rc
}
