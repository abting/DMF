#-------------------------------------------------
#
# Project created by QtCreator 2016-05-24T11:19:34
#
#-------------------------------------------------

#need to include the serialport manually
QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DMFGui
TEMPLATE = app


SOURCES += main.cpp\
        dmfgui.cpp

HEADERS  += dmfgui.h

FORMS    += dmfgui.ui

RESOURCES += \
    dmf_scene_v2.qrc \
    container.qrc
