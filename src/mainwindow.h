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
    MainWindow(QWidget * parent = 0);
    ~MainWindow();

    QString mXplaneDir;
    QString mTargetDir;
    QString mTargetCslDir;

public slots:
    // Выбор папки
    void setupXplaneDirSlot();

private slots:
    void targetDirsSetupSlot();
    // About
    void AboutSlot();
    // Setting
    void SettingSlot();
    void SetupCustomDirSlot();
    void UpdateSlot();
    void IndexSlot();
    // Контекст меню для списка
    void ListContextMenu(const QPoint & pos);
    void ListClear();
    void ListSelAll();
    // Контекст меню для таблицы
    void TableContextMenu(const QPoint & pos);
    void TableSelAll();
    void TableInfo();
    // Общее контекст меню, если понадобиццо
    void contextMenuEvent(QContextMenuEvent * event);
    /*void SetMsgSlot(QString Msg);
    void InitProgBarSlot(int start, int end, int current = 0, int step = 1);
    void StepProgBarSlot();
    void SetValProgBarSlot(int value);*/
    //void IndexFinish();

private:
    Ui::MainWindow * mUi;
    About * AboutWin;
    Settings * SettingsWin;
    IndexStep * Indx;
    UpdateStep * Updt;
    PackageAdditionalInfo * Inf;

    // контекст меню Списка
    QAction * ListClearAct;
    QAction * ListSelAllAct;
    // контекст меню Таблицы
    QAction * TableSelAllAct;
    QAction * TableInfoAct;

    //-------------------------------------------------------------------------
    QString browseSimDirDialog(const QString & inStartPath);
    bool setupNewSimDir(const QString & newSimDir);
    static bool isSimDirValid(const QString & dir);
    void setupTargetDirs();
};

#endif // MAINWINDOW_H
