#-------------------------------------------------
#
# Project created by QtCreator 2014-03-30T13:49:47
#
#-------------------------------------------------

TARGET = midi-common
TEMPLATE = lib
INCLUDEPATH += ../gnaClasses

DEFINES += MIDICOMMON_LIBRARY

SOURCES += midicommon.cpp \
    transportcontrol.cpp \
    midisettings.cpp \
    clockdisplay.cpp \
    timesignaturebuttons.cpp \
    datamodel.cpp

HEADERS += midicommon.h\
        midi-common_global.h \
    transportcontrol.h \
    midisettings.h \
    midiappcolumns.h \
    clockdisplay.h \
    timesignaturebuttons.h \
    datamodel.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xECF468F8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = midi-common.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

RESOURCES += \
    midi-common.qrc

FORMS += \
    midisettings.ui

#for gnaClasses
win32:CONFIG(release, debug|release): LIBS += -L../gnaClasses-build-desktop/release/ -lgnaClasses
else:win32:CONFIG(debug, debug|release): LIBS += -L../gnaClasses-build-desktop/debug/ -lgnaClasses


