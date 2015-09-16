#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T14:45:43
#
#-------------------------------------------------

QT       += core network serialport

QT       -= gui

TARGET = rbtserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    rbtserverarduinocom.cpp \
    rbtneatolidar.cpp \
    rbtserversocket.cpp \
    nearobotcore.cpp \
    ../rbtCommon/rbtcommonserializer.cpp \
    rbtlogger.cpp

HEADERS += \
    rbtserverarduinocom.h \
    rbtneatolidar.h \
    rbtserversocket.h \
    nearobotcore.h \
    ../rbtCommon/rbtcommonserializer.h \
    rbtlogger.h

INCLUDEPATH += ../../Arduino/rbt2015/
INCLUDEPATH += ../rbtCommon/
