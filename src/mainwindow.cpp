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
    QTimer::singleShot(0, this, &MainWindow::initialTgtDirsSetupSlot);
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
                                        PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location"),
                                        inStartPath,
                                        "X-Plane*.exe (X-Plane*.exe)");
#elif defined Q_OS_LINUX
	 return QFileDialog::getOpenFileName(this,
		PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location"),
		inStartPath,
		"X-Plane* (X-Plane*)");
#else
	 return QFileDialog::getOpenFileName(this,
		PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location"),
		inStartPath,
		"X-Plane*.app (X-Plane*.app)");
#endif
}

bool MainWindow::setupNewSimDir(const QString & newSimDir) {
    if (isSimDirValid(newSimDir)) {
        mIsSimDirCustom = false;
        mSimDir = newSimDir;
        QSettings settings(gSettingsFileName, QSettings::IniFormat);
        settings.setValue("mSimDir", mSimDir);
        settings.setValue("mIsSimDirCustom", mIsSimDirCustom);
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
    if (!mIsSimDirCustom) {
        mTargetDir = mSimDir + "/" + gAltitudeResDir;
        mTargetCslDir = mSimDir + "/" + gAltitudeCslDir;
    }
    else {
        mTargetDir = mSimDir;
        mTargetCslDir = mSimDir + "/" + gAltitudeCslDir;
    }

    //
    mUi->curPathLabel->setText(mSimDir);
    mUi->PrevButton->setEnabled(true);
    mUi->listWidget->addItem(tr("Now click \"Index\" to determine files which need to be updated."));
}

/**************************************************************************************************/
//////////////////////////////////////////* SLOTS */////////////////////////////////////////////
/**************************************************************************************************/

void MainWindow::initialTgtDirsSetupSlot() {
    QSettings const settings(gSettingsFileName, QSettings::IniFormat);
    mSimDir = settings.value("mSimDir", "").toString();
    mIsSimDirCustom = settings.value("mIsSimDirCustom", false).toBool();
    if (!mIsSimDirCustom) {
        while (!isSimDirValid(mSimDir)) {
            QString const selectedSimFile = browseSimDirDialog(mSimDir);
            if (selectedSimFile.isEmpty()) {
                QApplication::quit();
                return;
            }
            setupNewSimDir(QFileInfo(selectedSimFile).dir().path());
        }
        if (isSimDirValid(mSimDir)) {
            setupTargetDirs();
        }
    }
    else {
        setupTargetDirs();
    }
}

void MainWindow::selectSimDirSlot() {
    QString const oldSimDir = mSimDir;
    mSimDir = "";
    while (!isSimDirValid(mSimDir)) {
        QString const selectedSimFile = browseSimDirDialog(oldSimDir);
        if (selectedSimFile.isEmpty()) {
            mSimDir = oldSimDir;
            return;
        }
        setupNewSimDir(QFileInfo(selectedSimFile).dir().path());
    }
    if (isSimDirValid(mSimDir) && oldSimDir != mSimDir) {
        setupTargetDirs();
    }
}

void MainWindow::selectCustomDirSlot() {
    QMessageBox::warning(this, PROGRAM_NAME,
                         tr("Warning! This function is designed for advanced users."
                            "\nPlease use it only if you are absolutely sure what you are doing otherwise the program can become unusable!"), QMessageBox::Ok);

    QString const newDir = QFileDialog::getExistingDirectory(this,
        PROGRAM_NAME + tr(" :: Specify a folder where the library will be installed"), mSimDir, QFileDialog::ShowDirsOnly);
    
    if (!newDir.isEmpty()) {
        mIsSimDirCustom = true;
        mSimDir = newDir;
        QSettings settings(gSettingsFileName, QSettings::IniFormat);
        settings.setValue("mSimDir", mSimDir);
        settings.setValue("mIsSimDirCustom", mIsSimDirCustom);
        mUi->curPathLabel->setText(mSimDir);
    }
}

//-------------------------------------------------------------------------
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
