#ifndef TYPES_H
#define TYPES_H

#include <QtGui>

static QString VerProg = "0.1.1.4.beta";

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
