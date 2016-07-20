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


SOURCES +=\
    main.cpp \
    funcgen.cpp \
    dmfgui.cpp \
    dialog.cpp \
    nemesys.cpp \
    node.cpp

HEADERS  += dmfgui.h \
    dialog.h \
    visa.h \
    visatype.h \
    funcgen.h \
    nemesys.h \
    nemesys_api.h \
    err_codes.h \
    node.h

FORMS    += dmfgui.ui \
    dialog.ui

RESOURCES += \
    dmf_scene_v2.qrc \
    container.qrc



win32: LIBS += -L$$PWD/WinNT/ -lvisa32

INCLUDEPATH += $$PWD/WinNT
DEPENDPATH += $$PWD/WinNT

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/WinNT/visa32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/WinNT/libvisa32.a

win32: LIBS += -L$$PWD/Win64/ -lvisa32

INCLUDEPATH += $$PWD/Win64
DEPENDPATH += $$PWD/Win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Win64/visa32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/Win64/libvisa32.a

win32: LIBS += -L$$PWD/Win64/ -lvisa64

INCLUDEPATH += $$PWD/Win64
DEPENDPATH += $$PWD/Win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Win64/visa64.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/Win64/libvisa64.a

