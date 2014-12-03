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

// types of programm

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

#endif // TYPES_H
