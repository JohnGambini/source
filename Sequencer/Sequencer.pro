#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T00:35:42
#
#-------------------------------------------------

QT       += core gui

RC_FILE = Sequencer.rc

TARGET = Sequencer
TEMPLATE = app
INCLUDEPATH += ../gnaClasses
INCLUDEPATH += ../libmidi
DEFINES += WIN32

SOURCES += main.cpp\
        mainwindow.cpp \
    projectwidget.cpp \
    projectview.cpp \
    projectheader.cpp \
    header.cpp \
    datamodel.cpp \
    sequencerwidget.cpp \
    sequencerheader.cpp \
    playclass.cpp \
    patchchnlwidget.cpp \
    notewidget.cpp \
    sequencerview.cpp \
    tracksettingswidget.cpp

HEADERS  += mainwindow.h \
    projectwidget.h \
    projectview.h \
    projectheader.h \
    header.h \
    datamodel.h \
    sequencerwidget.h \
    sequencerheader.h \
    playclass.h \
    patchchnlwidget.h \
    notewidget.h \
    sequencerview.h \
    tracksettingswidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    Sequencer.qrc

#for gnaClasses
win32:CONFIG(release, debug|release): LIBS += -L../gnaClasses-build-desktop/release/ -lgnaClasses
else:win32:CONFIG(debug, debug|release): LIBS += -L../gnaClasses-build-desktop/debug/ -lgnaClasses

#for libmidi
win32:CONFIG(release, debug|release): LIBS += -L../libmidi-build-desktop/release/ -llibmidi
else:win32:CONFIG(debug, debug|release): LIBS += -L../libmidi-build-desktop/debug/ -llibmidi
