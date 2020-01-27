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
    void targetDirsSetupSlot();
    // About
    void aboutSlot();
    // Setting
    void settingSlot();
    void selectSimDirSlot();
    void selectCustomDirSlot();
    void updateSlot();
    void indexSlot();
    // list context menu
    void listContextMenu(const QPoint & pos);
    void listClear();
    void listSelAll();
    // table context menu
    void tableContextMenu(const QPoint & pos);
    void tableSelAll();
    void tableInfo();

    //-------------------------------------------------------------------------
private:
    QString mXplaneDir;
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

    //-------------------------------------------------------------------------
    QString browseSimDirDialog(const QString & inStartPath);
    bool setupNewSimDir(const QString & newSimDir);
    static bool isSimDirValid(const QString & dir);
    void setupTargetDirs();
};

#endif // MAINWINDOW_H
