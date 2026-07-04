// SPDX-License-Identifier: MPL-2.0

#ifndef BASE_STEPS_H
#define BASE_STEPS_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "Definitions.h"
#include <QtNetwork>

class BaseSteps : public QWidget {
Q_OBJECT

    //-------------------------------------------------------------------------
public:
    struct PackageEntry {
        int ID;
        QStringList data;
        eFileState state;
        eFileType type;
    };

    QVector<PackageEntry> mEntryList;

    //-------------------------------------------------------------------------
protected:
    QWidget * MW;
    Ui::MainWindow * MWUI;

    //-------------------------------------------------------------------------
    BaseSteps(QWidget * _MW, Ui::MainWindow * _MWUI);
    virtual ~BaseSteps();

    //-------------------------------------------------------------------------
    void setMessage(QString msg) const;
    void initProgBar(int inStart, int inEnd, int inCurrent = 0, int inStep = 1);
    void stepProgBar();
    void setValProgBar(int value) const;

    //-------------------------------------------------------------------------
private:
    int mStep = 0;
    int mCurrent = 0;
};

#endif // BASE_STEPS_H
