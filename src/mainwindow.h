#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QtGui>
#include "Definitions.h"
#include "about.h"
#include "settings.h"
#include "IndexStep.h"
#include "UpdateStep.h"
#include "PackageAdditionalInfo.h"
#include "AltitudeDefs.h"

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

    //-------------------------------------------------------------------------
public slots:

    //-------------------------------------------------------------------------
private slots:
    void init();
    void selectSimDirSlot();
    void selectCustomDirSlot();
    //
    void aboutSlot() const;
    void settingSlot() const;   
    void updateSlot() const;
    void indexSlot() const;
    // list context menu
    void listContextMenu(const QPoint & pos);
    void listClear() const;
    void listSelAll() const;
    // table context menu
    void tableContextMenu(const QPoint & pos);
    void tableSelAll() const;
    void tableInfo() const;

    //-------------------------------------------------------------------------
private:
    AltitudeDefs * mAltitudeDefs;

    bool mIsSimDirCustom = false;
    QString mSimDir;
    QString mTargetDir;
    QString mTargetCslDir;
    //
    Ui::MainWindow * mUi;
    About * mAboutWin;
    Settings * mSettingsWin;
    IndexStep * mIndexStep;
    UpdateStep * mUpdateStep;
    PackageAdditionalInfo * mPackInfoWin;

    // list context menu
    QAction * mListClearAct;
    QAction * mListSelAllAct;
    // table context menu
    QAction * mTableSelAllAct;
    QAction * mTableInfoAct;

    //
    QCommandLineParser mCliParser;
    bool mIsIndexAutoStart = false;
    

//-------------------------------------------------------------------------
    void parseCliArgs();

    QString browseSimDirDialog(const QString & inStartPath);
    bool setupNewSimDir(const QString & newSimDir);
    bool setupNewCustomDir(const QString & newCustomDir);
    static bool isSimDirValid(const QString & dir);
    void setupTargetDirs();
};

#endif // MAINWINDOW_H
