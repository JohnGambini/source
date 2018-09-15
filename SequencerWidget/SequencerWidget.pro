#-------------------------------------------------
#
# Project created by QtCreator 2014-03-04T23:58:35
#
#-------------------------------------------------

QT       += core gui

RC_FILE = SequencerWidget.rc

TARGET = SequencerWidget
TEMPLATE = app
INCLUDEPATH += ../gnaClasses
INCLUDEPATH += ../portmidi
INCLUDEPATH += ../midi-common

SOURCES += main.cpp\
        mainwindow.cpp \
    tracksettingswidget.cpp \
    sequencerwidget.cpp \
    sequencerview.cpp \
    sequencerheader.cpp \
    playclass.cpp \
    patchchnlwidget.cpp \
    notewidget.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    tracksettingswidget.h \
    sequencerwidget.h \
    sequencerview.h \
    sequencerheader.h \
    playclass.h \
    patchchnlwidget.h \
    notewidget.h \
    settingsdialog.h \
    midi.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    SequencerWidget.qrc

#for gnaClasses
win32:CONFIG(release, debug|release): LIBS += -L../gnaClasses-build-desktop/release/ -lgnaClasses
else:win32:CONFIG(debug, debug|release): LIBS += -L../gnaClasses-build-desktop/debug/ -lgnaClasses

#for portmidi
win32:CONFIG(release, debug|release): LIBS += -L../portmidi-build-desktop/release/ -lportmidi
else:win32:CONFIG(debug, debug|release): LIBS += -L../portmidi-build-desktop/debug/ -lportmidi

#for midi-common
win32:CONFIG(release, debug|release): LIBS += -L../midi-common-build-desktop/release/ -lmidi-common
else:win32:CONFIG(debug, debug|release): LIBS += -L../midi-common-build-desktop/debug/ -lmidi-common
