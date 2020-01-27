#include "mainwindow.h"
#include <QAction>
#include <QtWidgets>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
      mUi(new Ui::MainWindow) {
    // setup UI
    mUi->setupUi(this);

    // load settings
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    resize(settings.value("size", QSize(850, 615)).toSize());

    // List context menu
    this->ListClearAct = new QAction(tr("Clear"), this);
    connect(this->ListClearAct, &QAction::triggered, this, &MainWindow::ListClear);
    this->ListSelAllAct = new QAction(tr("Select All"), this);
    this->ListSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(this->ListSelAllAct, &QAction::triggered, this, &MainWindow::ListSelAll);

    // table context menu
    this->TableSelAllAct = new QAction(tr("Select All"), this);
    this->TableSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(this->TableSelAllAct, &QAction::triggered, this, &MainWindow::TableSelAll);
    this->TableInfoAct = new QAction(tr("Info"), this);
    connect(this->TableInfoAct, &QAction::triggered, this, &MainWindow::TableInfo);

    // table colons widths
    this->mUi->tableWidget->setColumnWidth(0, 30);  //ID
    this->mUi->tableWidget->setColumnWidth(1, 150); //Title
    this->mUi->tableWidget->setColumnWidth(2, 230); //Info
    this->mUi->tableWidget->setColumnWidth(3, 130); //Ver
    this->mUi->tableWidget->setColumnWidth(4, 80);  //sizeMB
    this->mUi->tableWidget->setColumnWidth(5, 120); //status
    this->mUi->tableWidget->setColumnWidth(6, 20);  //code
    this->mUi->tableWidget->setColumnHidden(6, true);

    //
    this->mUi->progressBar->setValue(0);
    this->mUi->listWidget->addItem(tr("X-CSL-Updater, Ver.:") + gProgramVersion);

    // Objs Init
    this->AboutWin = new About(this);
    this->SettingsWin = new Settings(this);
    this->Inf = new PackageAdditionalInfo(this, this->mUi);
    this->Indx = new IndexStep(this, this->mUi, this->Inf);
    this->Updt = new UpdateStep(this, this->mUi);

    // connects
    connect(this->mUi->actionAbout, &QAction::triggered, this, &MainWindow::AboutSlot);
    connect(this->mUi->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(this->mUi->actionSetFolder, &QAction::triggered, this, &MainWindow::setupXplaneDirSlot);
    connect(this->mUi->actionSettings, &QAction::triggered, this, &MainWindow::SettingSlot);
    connect(this->mUi->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::ListContextMenu);
    connect(this->mUi->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::TableContextMenu);
    connect(this->mUi->SelAllButton, &QPushButton::pressed, this, &MainWindow::TableSelAll);
    connect(this->mUi->NextButton, &QPushButton::pressed, this, &MainWindow::UpdateSlot);
    connect(this->mUi->PrevButton, &QPushButton::pressed, this, &MainWindow::IndexSlot);
    connect(this->mUi->ButtonSetFolder, &QPushButton::pressed, this, &MainWindow::setupXplaneDirSlot);
    connect(this->mUi->actionSet_Custom_Path, &QAction::triggered, this, &MainWindow::SetupCustomDirSlot);

    //
    mUi->PrevButton->setDisabled(true);
    mUi->NextButton->setDisabled(true);

    //
    QTimer::singleShot(0, this, &MainWindow::targetDirsSetupSlot);
}

MainWindow::~MainWindow() {
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    delete Indx;
    delete Updt;
    delete Inf;
    delete mUi;
}

void MainWindow::ListClear() {
    this->mUi->listWidget->clear();
}

void MainWindow::ListSelAll() {
    this->mUi->listWidget->selectAll();
}

void MainWindow::TableSelAll() {
    this->mUi->tableWidget->selectAll();
    this->mUi->tableWidget->setFocus();
}

void MainWindow::AboutSlot() {
    this->AboutWin->show();
}

void MainWindow::SettingSlot() {
    this->SettingsWin->LoadSettings();
    this->SettingsWin->show();
}

void MainWindow::ListContextMenu(const QPoint & pos) {
    QMenu menu(this);
    menu.addAction(this->ListClearAct);
    menu.addAction(this->ListSelAllAct);
    menu.exec(this->mUi->listWidget->mapToGlobal(pos));
}

void MainWindow::TableContextMenu(const QPoint & pos) {
    QMenu menu(this);
    if (this->mUi->tableWidget->rowCount() < 1) {
        this->TableInfoAct->setDisabled(true);
    }
    else {
        this->TableInfoAct->setEnabled(true);
    }
    menu.addAction(this->TableInfoAct);
    menu.addAction(this->TableSelAllAct);
    menu.exec(this->mUi->tableWidget->mapToGlobal(pos));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *) {
    /*QMenu menu(this);
     menu.addAction(this->ui->actionAbout);
     menu.addAction(this->ui->actionAbout_Qt);
     menu.exec(event->globalPos());*/
}

QString MainWindow::browseSimDirDialog(const QString & inStartPath) {
#ifdef Q_OS_WIN32
    return QFileDialog::getOpenFileName(this,
                                        tr("Specify the X-Plane executable file location"),
                                        inStartPath,
                                        "X-Plane*.exe (X-Plane*.exe)");
#elif defined Q_OS_LINUX
	 return QFileDialog::getOpenFileName(this,
		tr("Specify the X-Plane executable file location"),
		inStartPath,
		"X-Plane* (X-Plane*)");
#else
	 return QFileDialog::getOpenFileName(this,
		tr("Specify the X-Plane executable file location"),
		inStartPath,
		"X-Plane*.app (X-Plane*.app)");
#endif
}

bool MainWindow::setupNewSimDir(const QString & newSimDir) {
    if (isSimDirValid(newSimDir)) {
        mXplaneDir = newSimDir;
        QSettings settings(gSettingsFileName, QSettings::IniFormat);
        settings.setValue("mXplaneDir", mXplaneDir);
        return true;
    }
    // if sim dir is not ok
    QMessageBox::critical(this, "ERROR!",
                          "The specified X-Plane executable file path is not valid!"
                          "\nOr X-Plane installation located at the specified path is not valid or broken."
                          "\nYou can try to reinstall or repair your X-Plane installation.", QMessageBox::Ok);
    return false;
}

bool MainWindow::isSimDirValid(const QString & dir) {
    QString const simPluginsDir(dir + "/" + gSimPluginsDir);
    if (QDir(dir).exists() && QDir(simPluginsDir).exists()) {
        return true;
    }
    return false;
}

void MainWindow::setupTargetDirs() {
    // now we use only Altitude suffixes, but should think about support x-ivap later
    mTargetDir = mXplaneDir + "/" + gAltitudeResDir;
    mTargetCslDir = mXplaneDir + "/" + gAltitudeCslDir;
    // QDir const tgtDir(mTargetDir);
    // QDir const tgtCslDir(mTargetCslDir);
    // if (tgtDir.exists() && tgtCslDir.exists()) {
    //      mUi->PrevButton->setEnabled(true);
    //     return true;
    // }
    // if (!tgtDir.exists()) {
    //     if (!tgtDir.mkpath(mTargetDir)) {
    //         QMessageBox::critical(this, PROGRAM_NAME,tr("Error! Cannot create the common target path: ") +  mTargetDir, QMessageBox::Ok);
    //     }
    // }
    // if (!tgtCslDir.exists()) {
    //     if (!tgtCslDir.mkpath(mTargetCslDir)) {
    //         QMessageBox::critical(this, PROGRAM_NAME,tr("Error! Cannot create the CSL Library target path: ") +  mTargetCslDir, QMessageBox::Ok);
    //     }
    // }
    // if (tgtDir.exists() && tgtCslDir.exists()) {
    //      mUi->PrevButton->setEnabled(true);
    //     return true;
    // }
    //
    // mTargetDir = "";
    // mTargetCslDir = "";
    // mUi->PrevButton->setEnabled(false);
    // mUi->NextButton->setEnabled(false);
    // return false;
    //

    this->mUi->curPathLabel->setText(mXplaneDir);
    mUi->PrevButton->setEnabled(true);
    mUi->listWidget->addItem(tr("Now click \"Index\" to determine files which need to be updated."));
}

void MainWindow::TableInfo() {
    this->Inf->OpenInfoWin();
}

void MainWindow::setupXplaneDirSlot() {
    QString const oldSimDir = mXplaneDir;
    mXplaneDir = "";
    while (!isSimDirValid(mXplaneDir)) {
        QString const selectedSimFile = browseSimDirDialog(oldSimDir);
        if (selectedSimFile.isEmpty()) {
            mXplaneDir = oldSimDir;
            return;
        }
        setupNewSimDir(QFileInfo(selectedSimFile).dir().path());
    }
}

void MainWindow::targetDirsSetupSlot() {
    QSettings const settings(gSettingsFileName, QSettings::IniFormat);
    // setup and check target dirs stuff on startup
    mXplaneDir = settings.value("mXplaneDir", "").toString();
    while (!isSimDirValid(mXplaneDir)) {
        QString const selectedSimFile = browseSimDirDialog(mXplaneDir);
        if (selectedSimFile.isEmpty()) {
            QApplication::quit();
            return;
        }
        setupNewSimDir(QFileInfo(selectedSimFile).dir().path());
    }
    if (isSimDirValid(mXplaneDir)) {
        setupTargetDirs();
    }
}

void MainWindow::SetupCustomDirSlot() {
    QMessageBox::warning(this, PROGRAM_NAME,
                         tr("Warning! This function is designed for advanced users."
                            "Please use it only if you understand what you are doing otherwise the program can become unusable!"), QMessageBox::Ok);
    // this->mXplaneDir = QFileDialog::getExistingDirectory(this,
    //                                                      tr("X-CSL-Updater :: Specify target folder"), this->mXplaneDir, QFileDialog::ShowDirsOnly);
    //
    // if (!this->mXplaneDir.isEmpty()) {
    //     QSettings settings(gSettingsFileName, QSettings::IniFormat);
    //     settings.setValue("mXplaneDir", this->mXplaneDir);
    //     this->mUi->curPathLabel->setText(this->mXplaneDir);
    // }
}

void MainWindow::UpdateSlot() {
    this->mUi->PrevButton->setDisabled(true);
    this->mUi->NextButton->setDisabled(true);
    this->Updt->StartUpdate(this->Indx->mEntryList, this->Indx);
}

void MainWindow::IndexSlot() {
    this->mUi->PrevButton->setDisabled(true);
    this->mUi->NextButton->setDisabled(true);
    this->Indx->StartIndex();
}
