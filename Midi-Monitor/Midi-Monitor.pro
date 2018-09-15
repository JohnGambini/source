#-------------------------------------------------
#
# Project created by QtCreator 2014-04-01T16:22:47
#
#-------------------------------------------------

QT       += core gui

RC_FILE = MIDI-Monitor.rc

TARGET = Midi-Monitor
TEMPLATE = app
INCLUDEPATH += ../gnaClasses
INCLUDEPATH += ../portmidi
INCLUDEPATH += ../midi-common

SOURCES += main.cpp\
        mainwindow.cpp \
    midiProcs.cpp \
    eventlistwidget.cpp \
    message.cpp

HEADERS  += mainwindow.h \
    midiProcs.h \
    eventlistwidget.h \
    message.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    Midi-Monitor.qrc


#for gnaClasses
win32:CONFIG(release, debug|release): LIBS += -L../gnaClasses-build-desktop/release/ -lgnaClasses
else:win32:CONFIG(debug, debug|release): LIBS += -L../gnaClasses-build-desktop/debug/ -lgnaClasses

#for portmidi
win32:CONFIG(release, debug|release): LIBS += -L../portmidi-build-desktop/release/ -lportmidi
else:win32:CONFIG(debug, debug|release): LIBS += -L../portmidi-build-desktop/debug/ -lportmidi

#for midi-common
win32:CONFIG(release, debug|release): LIBS += -L../midi-common-build-desktop/release/ -lmidi-common
else:win32:CONFIG(debug, debug|release): LIBS += -L../midi-common-build-desktop/debug/ -lmidi-common
