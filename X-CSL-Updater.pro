# -------------------------------------------------
# Project created by QtCreator 2009-09-21T21:00:01
# -------------------------------------------------
QT += network
TARGET = X-CSL-Updater
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    about.cpp \
    index.cpp \
    base_steps.cpp \
    update.cpp \
    settings.cpp \
    info.cpp
HEADERS += mainwindow.h \
    about.h \
    types.h \
    index.h \
    base_steps.h \
    update.h \
    settings.h \
    info.h
FORMS += mainwindow.ui \
    about.ui \
    settings.ui \
    info.ui
RESOURCES += x-csl-updater.qrc
TRANSLATIONS = english.ts

win32 {
    RC_FILE += x-csl-updater-win32.rc
    DEFINES += WIN
}

unix:!macx {
    DEFINES += LIN
}

macx {
    DEFINES += MAC
}
