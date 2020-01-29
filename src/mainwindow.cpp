#include "mainwindow.h"
#include <QAction>
#include <QtWidgets>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
      mUi(new Ui::MainWindow) {

    QCoreApplication::setApplicationName(PROGRAM_NAME);
    QCoreApplication::setApplicationVersion(gProgramVersion);

    // setup UI
    mUi->setupUi(this);

    QSettings const settings(gSettingsFileName, QSettings::IniFormat);
    mSimDir = settings.value("mSimDir", "").toString();
    mIsSimDirCustom = settings.value("mIsSimDirCustom", false).toBool();

    // load settings
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
    mIndexStep = new IndexStep(this, mUi, mPackInfoWin, mTargetDir, mTargetCslDir);
    mUpdateStep = new UpdateStep(this, mUi, mTargetDir, mTargetCslDir);

    // connects
    connect(mUi->actionAbout, &QAction::triggered, this, &MainWindow::aboutSlot);
    connect(mUi->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mUi->actionSetFolder, &QAction::triggered, this, &MainWindow::selectSimDirSlot);
    connect(mUi->actionSettings, &QAction::triggered, this, &MainWindow::settingSlot);
    connect(mUi->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::listContextMenu);
    connect(mUi->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::tableContextMenu);
    connect(mUi->selAllButton, &QPushButton::pressed, this, &MainWindow::tableSelAll);
    connect(mUi->updateButton, &QPushButton::pressed, this, &MainWindow::updateSlot);
    connect(mUi->indexButton, &QPushButton::pressed, this, &MainWindow::indexSlot);
    connect(mUi->browseSimDirButton, &QPushButton::pressed, this, &MainWindow::selectSimDirSlot);
    connect(mUi->actionSet_Custom_Path, &QAction::triggered, this, &MainWindow::selectCustomDirSlot);

    //
    mUi->indexButton->setDisabled(true);
    mUi->updateButton->setDisabled(true);
    mUi->curPathLabel->setText("UNDEFINED...");

    //
    QTimer::singleShot(0, this, &MainWindow::init);
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

void MainWindow::parseCliArgs() {
    mCliParser.setApplicationDescription(STS_XCSL_PROJECT_DESCRIPTION);
    const auto helpOpt = mCliParser.addHelpOption();
    const auto verOpt = mCliParser.addVersionOption();
    QCommandLineOption const simDir(QStringList() << "d" << "sim-dir",
                                    "X-Plane root directory where the X-CSL library will be installed.",
                                    "dir");
    QCommandLineOption const indexAutoStart(QStringList() << "i" << "index-auto-start",
                                            "If set indexing process will be started automatically.");
    mCliParser.addOption(indexAutoStart);

    mCliParser.addOption(simDir);
    QCommandLineOption const customDir("custom-tgt-dir",
                                       "Custom target directory where the X-CSL library will be installed."
                                       "\nNo any target directory checks or auto target directory suffixes will be applied!"
                                       "\nRecommended only for advanced users who are sure what they are doing!");
    mCliParser.addOption(customDir);

    //parse
#ifdef  Q_OS_WIN32
    bool res = mCliParser.parse(QApplication::arguments());
    if (!res) {
        QMessageBox::critical(this, PROGRAM_NAME + tr(" :: ERROR!"),
                              "<html><body>An error has occured during parsing command line input.<br>" +
                              mCliParser.errorText() + "<br><pre>" + mCliParser.helpText() + "</pre></body></html>", QMessageBox::Ok);
        QApplication::exit(1);
        return;
    }
    if (mCliParser.isSet(helpOpt)) {
        QMessageBox::information(this, PROGRAM_NAME + tr(" :: Command line usage:"),
                                 "<html><body><pre>" + mCliParser.helpText() + "</pre></body></html>", QMessageBox::Ok);
        QApplication::exit(1);
        return;
    }
    if (mCliParser.isSet(verOpt)) {
        QMessageBox::information(this, PROGRAM_NAME,
                                 "Version: " + gProgramVersion, QMessageBox::Ok);
        QApplication::exit(1);
        return;
    }
#else
    // on unix systems it should normally work through terminal
    mCliParser.process(*qApp);
#endif

    mIsIndexAutoStart = mCliParser.isSet(indexAutoStart);
    if (mCliParser.isSet(customDir)) {
        mSimDir = mCliParser.value(customDir);
        mIsSimDirCustom = true;
        setupNewCustomDir(mSimDir);
    }
    else if (mCliParser.isSet(simDir)) {
        mSimDir = mCliParser.value(simDir);
        mIsSimDirCustom = false;
        setupNewSimDir(mSimDir);
    }
}

QString MainWindow::browseSimDirDialog(const QString & inStartPath) {
#ifdef Q_OS_WIN32
    return QFileDialog::getOpenFileName(this,
                                        PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location ..."),
                                        inStartPath,
                                        "X-Plane*.exe (X-Plane*.exe)");
#elif defined Q_OS_LINUX
	 return QFileDialog::getOpenFileName(this,
		PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location ..."),
		inStartPath,
		"X-Plane* (X-Plane*)");
#else
	 return QFileDialog::getOpenFileName(this,
		PROGRAM_NAME + tr(" :: Specify the X-Plane executable file location ..."),
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

bool MainWindow::setupNewCustomDir(const QString & newCustomDir) {
    mIsSimDirCustom = true;
    mSimDir = newCustomDir;
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    settings.setValue("mSimDir", mSimDir);
    settings.setValue("mIsSimDirCustom", mIsSimDirCustom);
    mUi->curPathLabel->setText(mSimDir);
    return true;
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
    mUi->indexButton->setEnabled(true);
    mUi->listWidget->addItem(tr("Now click \"Index\" to determine files which need to be updated."));
}

/**************************************************************************************************/
//////////////////////////////////////////* SLOTS */////////////////////////////////////////////
/**************************************************************************************************/

void MainWindow::init() {
    //
    parseCliArgs();
    //
    if (!mIsSimDirCustom) {
        while (!isSimDirValid(mSimDir)) {
            QMessageBox::warning(this, PROGRAM_NAME,
                                 tr("Please specify the X-Plane executable file location!\n\n"
                                    "Navigate to the folder where your X-Plane is installed and select the X-Plane executable file."), QMessageBox::Ok);
            QString const selectedSimFile = browseSimDirDialog(mSimDir);
            if (selectedSimFile.isEmpty()) {
                QApplication::quit();
                return;
            }
            setupNewSimDir(QFileInfo(selectedSimFile).dir().path());
        }
        if (isSimDirValid(mSimDir)) {
            setupTargetDirs();
            if (mIsIndexAutoStart) {
                QTimer::singleShot(0, this, &MainWindow::indexSlot);
            }
        }
    }
    else {
        setupTargetDirs();
        if (mIsIndexAutoStart) {
            QTimer::singleShot(0, this, &MainWindow::indexSlot);
        }
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
        setupNewCustomDir(newDir);
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
    mUi->indexButton->setDisabled(true);
    mUi->updateButton->setDisabled(true);
    mUpdateStep->StartUpdate(mIndexStep->mEntryList, mIndexStep);
}

void MainWindow::indexSlot() const {
    mUi->indexButton->setDisabled(true);
    mUi->updateButton->setDisabled(true);
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
