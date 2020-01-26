#ifndef TYPES_H
#define TYPES_H

#include <QtGui>
#include "Info.h"

static const QString gProgramVersion = QString(STS_XCSL_VERSION_STRING) + "+" + STS_XCSL_REVISION;

#define ORGANIZATION STS_XCSL_ORGANIZATION_NAME
#define PROGRAM_NAME STS_XCSL_PROJECT_SHORT_NAME

static const QString gSettingsFileName = "settings.ini";

static const QDir gXIvApResDir("Resources/plugins/X-IvAp Resources");
static const QDir gXIvApCslDir(gXIvApResDir.path() + "/CSL");
static const QDir gAltitudeResDir("Resources/plugins/IVAO_CSL");
static const QDir gAltitudeCslDir(gXIvApResDir.path() + "/CSL");

enum eFileState {
    CLIENT_FILE_STATUS_NONE	     = -999,
    CLIENT_FILE_STATUS_LOST	     = -1,
    CLIENT_FILE_STATUS_CHANGE    = 1,
    CLIENT_FILE_STATUS_OK	     = 0
};

enum ePackageState {
    CLIENT_PACKAGE_STATUS_NONE	 = -999,
    CLIENT_PACKAGE_STATUS_LOST	 = -1,
    CLIENT_PACKAGE_STATUS_CHANGE = 1,
    CLIENT_PACKAGE_STATUS_OK	 = 0
};

struct FileInfoStruct
{
    int Type;
    QString FileName;
    long int size;
    QString FileHash;
    QString FileDate;
    QString FileTime;
    QString FileDateTime;
    QString FileAllStr;
    int Flag;
};

constexpr auto MY_MAX_PATH = 2048;

#endif // TYPES_H
