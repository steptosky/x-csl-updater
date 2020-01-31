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

    const QString & mTargetDir;
    const QString & mTargetCslDir;

    //-------------------------------------------------------------------------
    BaseSteps(QWidget * _MW, Ui::MainWindow * _MWUI, const QString & targetDir, const QString & targetCslDir);
    virtual ~BaseSteps();

    //-------------------------------------------------------------------------
    void SetMessage(QString msg) const;
    void InitProgBar(int inStart, int inEnd, int inCurrent = 0, int inStep = 1);
    void StepProgBar();
    void SetValProgBar(int value) const;

    //-------------------------------------------------------------------------
private:
    int mStep = 0;
    int mCurrent = 0;
};

#endif // BASE_STEPS_H
