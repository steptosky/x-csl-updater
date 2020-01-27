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
    mListClearAct = new QAction(tr("Clear"), this);
    connect(mListClearAct, &QAction::triggered, this, &MainWindow::listClear);
    mListSelAllAct = new QAction(tr("Select All"), this);
    mListSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(mListSelAllAct, &QAction::triggered, this, &MainWindow::listSelAll);

    // table context menu
    mTableSelAllAct = new QAction(tr("Select All"), this);
    mTableSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(mTableSelAllAct, &QAction::triggered, this, &MainWindow::tableSelAll);
    mTableInfoAct = new QAction(tr("Info"), this);
    connect(mTableInfoAct, &QAction::triggered, this, &MainWindow::tableInfo);

    // table colons widths
    mUi->tableWidget->setColumnWidth(0, 30);  //ID
    mUi->tableWidget->setColumnWidth(1, 150); //Title
    mUi->tableWidget->setColumnWidth(2, 230); //Info
    mUi->tableWidget->setColumnWidth(3, 130); //Ver
    mUi->tableWidget->setColumnWidth(4, 80);  //sizeMB
    mUi->tableWidget->setColumnWidth(5, 120); //status
    mUi->tableWidget->setColumnWidth(6, 20);  //code
    mUi->tableWidget->setColumnHidden(6, true);

    //
    mUi->progressBar->setValue(0);
    mUi->listWidget->addItem(tr("X-CSL-Updater, Ver.:") + gProgramVersion);

    // Objs Init
    mAboutWin = new About(this);
    mSettingsWin = new Settings(this);
    mPackInfoWin = new PackageAdditionalInfo(this, mUi);
    mIndexStep = new IndexStep(this, mUi, mPackInfoWin);
    mUpdateStep = new UpdateStep(this, mUi);

    // connects
    connect(mUi->actionAbout, &QAction::triggered, this, &MainWindow::aboutSlot);
    connect(mUi->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mUi->actionSetFolder, &QAction::triggered, this, &MainWindow::selectSimDirSlot);
    connect(mUi->actionSettings, &QAction::triggered, this, &MainWindow::settingSlot);
    connect(mUi->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::listContextMenu);
    connect(mUi->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::tableContextMenu);
    connect(mUi->SelAllButton, &QPushButton::pressed, this, &MainWindow::tableSelAll);
    connect(mUi->NextButton, &QPushButton::pressed, this, &MainWindow::updateSlot);
    connect(mUi->PrevButton, &QPushButton::pressed, this, &MainWindow::indexSlot);
    connect(mUi->ButtonSetFolder, &QPushButton::pressed, this, &MainWindow::selectSimDirSlot);
    connect(mUi->actionSet_Custom_Path, &QAction::triggered, this, &MainWindow::selectCustomDirSlot);

    //
    mUi->PrevButton->setDisabled(true);
    mUi->NextButton->setDisabled(true);
    mUi->curPathLabel->setText("UNDEFINED...");

    //
    QTimer::singleShot(0, this, &MainWindow::targetDirsSetupSlot);
}

MainWindow::~MainWindow() {
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    delete mIndexStep;
    delete mUpdateStep;
    delete mPackInfoWin;
    delete mUi;
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
    //
    mUi->curPathLabel->setText(mXplaneDir);
    mUi->PrevButton->setEnabled(true);
    mUi->listWidget->addItem(tr("Now click \"Index\" to determine files which need to be updated."));
}

/**************************************************************************************************/
//////////////////////////////////////////* SLOTS */////////////////////////////////////////////
/**************************************************************************************************/

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

void MainWindow::selectSimDirSlot() {
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
    if (isSimDirValid(mXplaneDir) && oldSimDir != mXplaneDir) {
        setupTargetDirs();
    }
}

void MainWindow::selectCustomDirSlot() {
    QMessageBox::warning(this, PROGRAM_NAME,
                         tr("Warning! This function is designed for advanced users."
                            "Please use it only if you understand what you are doing otherwise the program can become unusable!"), QMessageBox::Ok);
    // mXplaneDir = QFileDialog::getExistingDirectory(this,
    //                                                      tr("X-CSL-Updater :: Specify target folder"), mXplaneDir, QFileDialog::ShowDirsOnly);
    //
    // if (!mXplaneDir.isEmpty()) {
    //     QSettings settings(gSettingsFileName, QSettings::IniFormat);
    //     settings.setValue("mXplaneDir", mXplaneDir);
    //     mUi->curPathLabel->setText(mXplaneDir);
    // }
}

void MainWindow::aboutSlot() const {
    mAboutWin->show();
}

void MainWindow::settingSlot() const {
    mSettingsWin->LoadSettings();
    mSettingsWin->show();
}

void MainWindow::updateSlot() const {
    mUi->PrevButton->setDisabled(true);
    mUi->NextButton->setDisabled(true);
    mUpdateStep->StartUpdate(mIndexStep->mEntryList, mIndexStep);
}

void MainWindow::indexSlot() const {
    mUi->PrevButton->setDisabled(true);
    mUi->NextButton->setDisabled(true);
    mIndexStep->StartIndex();
}

void MainWindow::listContextMenu(const QPoint & pos) {
    QMenu menu(this);
    menu.addAction(mListClearAct);
    menu.addAction(mListSelAllAct);
    menu.exec(mUi->listWidget->mapToGlobal(pos));
}

void MainWindow::listClear() const {
    mUi->listWidget->clear();
}

void MainWindow::listSelAll() const {
    mUi->listWidget->selectAll();
}

void MainWindow::tableSelAll() const {
    mUi->tableWidget->selectAll();
    mUi->tableWidget->setFocus();
}

void MainWindow::tableContextMenu(const QPoint & pos) {
    QMenu menu(this);
    if (mUi->tableWidget->rowCount() < 1) {
        mTableInfoAct->setDisabled(true);
    }
    else {
        mTableInfoAct->setEnabled(true);
    }
    menu.addAction(mTableInfoAct);
    menu.addAction(mTableSelAllAct);
    menu.exec(mUi->tableWidget->mapToGlobal(pos));
}

void MainWindow::tableInfo() const {
    mPackInfoWin->OpenInfoWin();
}
