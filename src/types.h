#ifndef TYPES_H
#define TYPES_H

#include <QtGui>

#include "../version.h"

#define VERSION_MAJOR    1
#define VERSION_MINOR    1
#define VERSION_PATCH    0

//static QString VerProg = "0.1.1.4.beta";

static QString VerProg = 
	QString::number(VERSION_MAJOR)
	+ "." +
	QString::number(VERSION_MINOR)
	+ "." +
	QString::number(VERSION_PATCH)
	+ " (" STS_REVISION_GLOBAL ")"
;

// some constants
#define ORGANISATION "VA X-Air Team && StepToSky Team"
#define PROGRAM_NAME "X-CSL-Updater"

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
