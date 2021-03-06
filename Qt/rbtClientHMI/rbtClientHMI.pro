#-------------------------------------------------
#
# Project created by QtCreator 2015-09-03T10:14:28
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rbtClientHMI
TEMPLATE = app


SOURCES += main.cpp\
        nearobothmi.cpp \
    lidarviewer.cpp \
    rbtclientsocket.cpp \
    ../rbtCommon/rbtcommonserializer.cpp

HEADERS  += nearobothmi.h \
    lidarviewer.h \
    rbtclientsocket.h \
    ../rbtCommon/rbtcommonserializer.h

INCLUDEPATH += ../rbtCommon/

FORMS    += nearobothmi.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

