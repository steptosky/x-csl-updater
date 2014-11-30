# -------------------------------------------------
# Project created by QtCreator 2009-09-21T21:00:01
# -------------------------------------------------
QT += network
TARGET = X-CSL-Updater
TEMPLATE = app
CONFIG += flat

CONFIG(debug, debug|release) {

	DESTDIR = ../bin/debug
	OBJECTS_DIR = Generated/obj
	MOC_DIR = Generated/moc
	RCC_DIR = Generated/qrc
	UI_DIR = Generated/ui

	CONFIG   += console
	
} else {

	DESTDIR = ../bin/release
	OBJECTS_DIR = Generated/obj
	MOC_DIR = Generated/moc
	RCC_DIR = Generated/qrc
	UI_DIR = Generated/ui

}

include(file-list.pri)
TRANSLATIONS = english.ts

win32 {
    RC_FILE += x-csl-updater-win32.rc
}

unix:!macx {

}

macx {

}
