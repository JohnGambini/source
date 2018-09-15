#-------------------------------------------------
#
# Project created by QtCreator 2014-03-09T23:30:42
#
#-------------------------------------------------

QT       += core gui

RC_FILE = MidiTimingMaster.rc

TARGET = MidiTimingMaster
TEMPLATE = app
INCLUDEPATH += ../gnaClasses
INCLUDEPATH += ../portmidi
INCLUDEPATH += ../midi-common

SOURCES += main.cpp\
        mainwindow.cpp \
    timingmasterwidget.cpp \
    header.cpp \
    midiProcs.cpp \
    midimasterstyle.cpp

HEADERS  += mainwindow.h \
    timingmasterwidget.h \
    header.h \
    midiProcs.h \
    midimasterstyle.h

FORMS    += mainwindow.ui

RESOURCES += \
    MidiTimingMaster.qrc

#for gnaClasses
win32:CONFIG(release, debug|release): LIBS += -L../gnaClasses-build-desktop/release/ -lgnaClasses
else:win32:CONFIG(debug, debug|release): LIBS += -L../gnaClasses-build-desktop/debug/ -lgnaClasses

#for portmidi
win32:CONFIG(release, debug|release): LIBS += -L../portmidi-build-desktop/release/ -lportmidi
else:win32:CONFIG(debug, debug|release): LIBS += -L../portmidi-build-desktop/debug/ -lportmidi

#for midi-common
win32:CONFIG(release, debug|release): LIBS += -L../midi-common-build-desktop/release/ -lmidi-common
else:win32:CONFIG(debug, debug|release): LIBS += -L../midi-common-build-desktop/debug/ -lmidi-common

