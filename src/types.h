#ifndef TYPES_H
#define TYPES_H

#include <QtGui>
#include "Info.h"

#define VERSION_MAJOR    1
#define VERSION_MINOR    2
#define VERSION_PATCH    0

static QString VerProg = QString(STS_XCSL_VERSION_STRING) + "+" STS_XCSL_REVISION;

// some constants
#define ORGANISATION STS_XCSL_ORGANIZATION_NAME
#define PROGRAM_NAME STS_XCSL_PROJECT_SHORT_NAME

// types of program
#define _CLIENT_FILE_STATUS_LOST	-1;
#define _CLIENT_FILE_STATUS_CHANGE	1;
#define _CLIENT_FILE_STATUS_OK		0;


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

#ifndef MY_MAX_PATH
#define MY_MAX_PATH 1024
#endif

#endif // TYPES_H
