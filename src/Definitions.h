#ifndef TYPES_H
#define TYPES_H

#include <QtGui>
#include "Info.h"

static const QString gProgramVersion = QString(STS_XCSL_VERSION_STRING) + "+" + STS_XCSL_REVISION;

#define ORGANIZATION STS_XCSL_ORGANIZATION_NAME
#define PROGRAM_NAME STS_XCSL_PROJECT_SHORT_NAME

static const QString gSettingsFileName = "settings.ini";
static const QString gTempDir = "tmp";

static const QString gSimPluginsDir = "Resources/plugins";

// static const QString gXIvApResDir = gSimPluginsDir + "/X-IvAp Resources";
// static const QString gXIvApCslDir = gXIvApResDir + "/CSL";

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

enum ePackageState {
    CLIENT_PACKAGE_STATUS_NONE = -999,
    CLIENT_PACKAGE_STATUS_LOST = -1,
    CLIENT_PACKAGE_STATUS_CHANGE = 1,
    CLIENT_PACKAGE_STATUS_OK = 0
};

#endif // TYPES_H
