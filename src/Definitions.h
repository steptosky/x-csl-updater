#ifndef TYPES_H
#define TYPES_H

#include <QApplication>
#include <QtGui>
#include "Info.h"

Q_DECLARE_LOGGING_CATEGORY(fromGUI)
inline Q_LOGGING_CATEGORY(fromGUI, "GUI")

static const QString gProgramVersion = QString(STS_XCSL_VERSION_STRING) + "+" + STS_XCSL_REVISION;

#define ORGANIZATION STS_XCSL_ORGANIZATION_NAME
#define PROGRAM_NAME STS_XCSL_PROJECT_SHORT_NAME

static const QString gSimPluginsDir = "Resources/plugins";

// static const QString gXIvApResDir = gSimPluginsDir + "/X-IvAp Resources";
// static const QString gXIvApCslDir = gXIvApResDir + "/CSL";

inline extern QString tempDir(){
    return QApplication::applicationDirPath() + "/tmp";
}

inline extern QString logFileName(){
    return QApplication::applicationDirPath() + "/log.txt";
}

inline extern QString settingsFileName(){
    return QApplication::applicationDirPath() + "/settings.ini";
}

inline extern QString translationFileName(const QString & inLangId) {
    if (inLangId == "ru") {
        static const QString name = "x-csl-updater_ru.qm";
        if (QFileInfo::exists(QApplication::applicationDirPath() + "/" + name)) {
            return QApplication::applicationDirPath() + "/" + name;
        }
        else {
            return ":/" + name;
        }
    }
    return "default";
}

enum eFileType {
    CSL_PACK_FILE = 0,
    ADDITIONAL_FILE
};

enum eFileState {
    CLIENT_FILE_STATUS_NONE = -999,
    CLIENT_FILE_STATUS_LOST = -1,
    CLIENT_FILE_STATUS_CHANGE = 1,
    CLIENT_FILE_STATUS_OK = 0
};

inline extern const char * localFileState2Text(eFileState state) {
    switch (state) {
        case CLIENT_FILE_STATUS_OK: return "Up-to-date";
            break;
        case CLIENT_FILE_STATUS_CHANGE: return "Out-of-date";
            break;
        case CLIENT_FILE_STATUS_LOST: return "Not installed";
            break;
        default: ;
        case CLIENT_FILE_STATUS_NONE: return "Unknown state";
            break;
    }
}

enum ePackageState {
    CLIENT_PACKAGE_STATUS_NONE = -999,
    CLIENT_PACKAGE_STATUS_LOST = -1,
    CLIENT_PACKAGE_STATUS_CHANGE = 1,
    CLIENT_PACKAGE_STATUS_OK = 0
};

inline extern const char * packageState2Text(ePackageState state) {
    switch (state) {
        case CLIENT_PACKAGE_STATUS_LOST: return "Not installed";
            break;
        case CLIENT_PACKAGE_STATUS_CHANGE: return "Out-of-date";
            break;
        case CLIENT_PACKAGE_STATUS_OK: return "Up-to-date";
            break;
        default: ;
        case CLIENT_PACKAGE_STATUS_NONE: return "Unknown state";
            break;
    }
}

#endif // TYPES_H
