#-------------------------------------------------
#
# Project created by QtCreator 2013-01-14T23:45:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = gnaClasses
TEMPLATE = lib
#CONFIG += staticlib
DEFINES += GNALIB_LIBRARY

SOURCES += \
    gnavariant.cpp \
    gnatreeitem.cpp \
    gnastring.cpp \
    gnaMidi.cpp \
    gnafile.cpp

HEADERS += \
    gnavariant.h \
    gnaUndoHandler.h \
    gnatreeitem.h \
    gnastring.h \
    gnaMidi.h \
    gnaGlobal.h \
    gnafile.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
