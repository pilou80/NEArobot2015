#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T14:45:43
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = rbtserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    rbtserverarduinocom.cpp \
    rbtneatolidar.cpp \
    rbtserversocket.cpp

HEADERS += \
    rbtserverarduinocom.h \
    rbtneatolidar.h \
    rbtserversocket.h
