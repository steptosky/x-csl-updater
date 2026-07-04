#include "mainwindow.h"
#include <QAction>
#include <QtWidgets>

bool MainWindow::mIsLogVerbose = false;

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
      mUi(new Ui::MainWindow) {
    //-------------------------------------------------------------------------
    mAltitudeDefs = AltitudeDefs::init(this);
    qInfo() << "Initializing for the Altitude IVAO client.";
    //
    QCoreApplication::setApplicationName(PROGRAM_NAME);
    QCoreApplication::setApplicationVersion(gProgramVersion);
    // setup UI
    mUi->setupUi(this);

    QSettings const settings(settingsFileName(), QSettings::IniFormat);
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
    mUi->tableWidget->setColumnWidth(0, 30); //ID
    mUi->tableWidget->setColumnHidden(0, true);
    mUi->tableWidget->setColumnWidth(6, 20); //code
    mUi->tableWidget->setColumnHidden(6, true);
    // mUi->tableWidget->setColumnWidth(1, settings.value("titleColWidth", 150).toInt()); //Title
    // // mUi->tableWidget->setColumnWidth(2, settings.value("infoColWidth", 280).toInt()); //Info
    // mUi->tableWidget->setColumnWidth(3, settings.value("versionColWidth", 130).toInt()); //Ver
    // mUi->tableWidget->setColumnWidth(4, settings.value("sizeColWidth", 80).toInt());  //size
    // mUi->tableWidget->setColumnWidth(5, settings.value("statusColWidth", 120).toInt());  //status
    mUi->tableWidget->setColumnWidth(1, 150); //Title
    mUi->tableWidget->setColumnWidth(3, 130); //Ver
    mUi->tableWidget->setColumnWidth(4, 80);  //size
    mUi->tableWidget->setColumnWidth(5, 120); //status
    auto * tableHeader = mUi->tableWidget->horizontalHeader();
    tableHeader->setStretchLastSection(false);
    tableHeader->setSectionResizeMode(1, QHeaderView::Fixed);    // Title
    tableHeader->setSectionResizeMode(2, QHeaderView::Stretch);  // Info
    tableHeader->setSectionResizeMode(3, QHeaderView::Fixed);    // Version
    tableHeader->setSectionResizeMode(4, QHeaderView::Fixed);    // Size
    tableHeader->setSectionResizeMode(5, QHeaderView::Fixed);    // Status

    //
    mUi->progressBar->setValue(0);
    setMessage(PROGRAM_NAME + tr(", Ver.:") + gProgramVersion);

    // Objs Init
    mAboutWin = new About(this);
    mPackInfoWin = new PackageAdditionalInfo(this, mUi);
    mSettingsWin = new Settings(this);
    mIndexStep = new IndexStep(this, mUi, mPackInfoWin);
    mUpdateStep = new UpdateStep(this, mUi);

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
    QSettings settings(settingsFileName(), QSettings::IniFormat);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("titleColWidth", mUi->tableWidget->columnWidth(1)); //Title
    settings.setValue("infoColWidth", mUi->tableWidget->columnWidth(2)); //Info
    settings.setValue("versionColWidth", mUi->tableWidget->columnWidth(3)); //Ver
    settings.setValue("sizeColWidth", mUi->tableWidget->columnWidth(4));  //size
    settings.setValue("statusColWidth", mUi->tableWidget->columnWidth(5));  //status
    //
    delete mUpdateStep;
    delete mIndexStep;
    delete mPackInfoWin;
    delete mUi;
    //
    AltitudeDefs::free();
}

void MainWindow::changeEvent(QEvent* e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        mUi->retranslateUi(this);
        if (mIsSimDirCustom) {
            mUi->curPathTitleLabel->setText(tr("Custom dir: "));
        }
        else {
            mUi->curPathTitleLabel->setText(tr("X-Plane dir: "));
        }
        mUi->curPathLabel->setText(mSimDir);
        break;
    default:
        break;
    }
}

bool MainWindow::parseCliArgs() {
    mCliParser.setApplicationDescription(STS_XCSL_PROJECT_DESCRIPTION);
    const auto helpOpt = mCliParser.addHelpOption();
    const auto verOpt = mCliParser.addVersionOption();
    const QCommandLineOption simDir(QStringList() << "t" << "sim-target-dir",
                                    "X-Plane root directory where the X-CSL library will be installed.",
                                    "directory");
    mCliParser.addOption(simDir);

    const QCommandLineOption indexAutoStart(QStringList() << "i" << "index-auto-start",
                                            "If set indexing process will be started automatically.");
    mCliParser.addOption(indexAutoStart);

    const QCommandLineOption isLogVerbose(QStringList() << "d" << "verbose",
                                          "If set the app will write more debug info to the log file and stdout.");
    mCliParser.addOption(isLogVerbose);

    const QCommandLineOption customDir("custom-target-dir",
                                       "Custom target directory where the X-CSL library will be installed."
                                       "\nNo any target directory checks or auto target directory suffixes will be applied!"
                                       "\nRecommended only for advanced users who are sure what they are doing!",
                                       "directory"
                                      );
    mCliParser.addOption(customDir);

    //parse
#ifdef  Q_OS_WIN32
    const bool res = mCliParser.parse(QApplication::arguments());
    if (!res) {
        QMessageBox::critical(this, PROGRAM_NAME + QString(" :: ERROR!"),
                              "<html><body>An error has occured during parsing command line input.<br>" +
                              mCliParser.errorText() + "<br><pre>" + mCliParser.helpText() + "</pre></body></html>", QMessageBox::Ok);
        QApplication::exit(1);
        return false;
    }
    if (mCliParser.isSet(helpOpt)) {
        QMessageBox::information(this, PROGRAM_NAME + QString(" :: Command line usage:"),
                                 "<html><body><pre>" + mCliParser.helpText() + "</pre></body></html>", QMessageBox::Ok);
        QApplication::quit();
        return false;
    }
    if (mCliParser.isSet(verOpt)) {
        QMessageBox::information(this, PROGRAM_NAME,
                                 "Version: " + gProgramVersion, QMessageBox::Ok);
        QApplication::quit();
        return false;
    }
#else
    // on unix systems it should normally work through terminal
    mCliParser.process(*qApp);
#endif

    mIsIndexAutoStart = mCliParser.isSet(indexAutoStart);
    MainWindow::mIsLogVerbose = mCliParser.isSet(isLogVerbose);
    if (mCliParser.isSet(customDir)) {
        mSimDir = mCliParser.value(customDir);
        mIsSimDirCustom = true;
        setupNewCustomDir(mSimDir);
        mUi->browseSimDirButton->setDisabled(true);
        mUi->actionSetFolder->setDisabled(true);
        mUi->actionSet_Custom_Path->setDisabled(true);
    }
    else if (mCliParser.isSet(simDir)) {
        mSimDir = mCliParser.value(simDir);
        mIsSimDirCustom = false;
        setupNewSimDir(mSimDir);
    }

    return true;
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
        QSettings settings(settingsFileName(), QSettings::IniFormat);
        settings.setValue("mSimDir", mSimDir);
        settings.setValue("mIsSimDirCustom", mIsSimDirCustom);
        return true;
    }
    // if sim dir is not ok
    QMessageBox::critical(this, PROGRAM_NAME + tr(" :: ERROR!"),
                          tr("The specified X-Plane executable file path is not valid!"
                          "\nOr the X-Plane installation located at the specified path is not valid or broken."
                          "\nYou can try to reinstall or repair your X-Plane installation."), QMessageBox::Ok);
    return false;
}

bool MainWindow::setupNewCustomDir(const QString & newCustomDir) {
    mIsSimDirCustom = true;
    mSimDir = newCustomDir;
    QSettings settings(settingsFileName(), QSettings::IniFormat);
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

void MainWindow::setupTargetDirs() const {
    // now we use only Altitude suffixes, but should think about support x-ivap later
    mAltitudeDefs->setSimDir(mSimDir, mIsSimDirCustom);
    qInfo() << "New sim target dir is selected.";
    qInfo() << "The sim target dir is set to: <" << mSimDir << ">";
    qInfo() << "Custom target dir mode is " << (mIsSimDirCustom ? "enabled." : "disabled.");
    //
    mIndexStep->resetIndex();
    if (mIsSimDirCustom){
        mUi->curPathTitleLabel->setText(tr("Custom dir: "));
    }
    else{
        mUi->curPathTitleLabel->setText(tr("X-Plane dir: "));
    }
    mUi->curPathLabel->setText(mSimDir);
    mUi->indexButton->setEnabled(true);
    setMessage(tr("Now click \"Index\" to determine files which need to be updated."));
}

/**************************************************************************************************/
//////////////////////////////////////////* SLOTS */////////////////////////////////////////////
/**************************************************************************************************/

void MainWindow::init() {
    if (!parseCliArgs()){
        QApplication::quit();
        return;
    }
    //
    const QDir currDir;
    const QDir tmpDir(tempDir());
    if (!tmpDir.exists()) {
        if (!currDir.mkdir(tempDir())) {
            qCritical() << "Cannot create temporary folder: " << tmpDir.absolutePath();
            QMessageBox::information(this, PROGRAM_NAME,
                                     tr("Cannot create temporary folder: ") + tmpDir.absolutePath(), QMessageBox::Ok);
            QApplication::exit(1);
            return;
        }
        qDebug() << "Created tmp folder: " << tmpDir.absolutePath();
    }
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
                            "\nPlease use it only if you are absolutely sure what you are doing otherwise the program can become unusable!"
                            "\nNote: no additional files will be installed/updated, only the X-CSL library files will be installed/updated."), QMessageBox::Ok);

    QString const newDir = QFileDialog::getExistingDirectory(this,
                                                             PROGRAM_NAME + tr(" :: Specify a folder where the library will be installed"), mSimDir, QFileDialog::ShowDirsOnly);
    if (!newDir.isEmpty()) {
        QString const oldSimDir = mSimDir;
        setupNewCustomDir(newDir);
        if (oldSimDir != mSimDir) {
            setupTargetDirs();
        }
    }
}

//-------------------------------------------------------------------------
void MainWindow::aboutSlot() const {
    qDebug() << "About slot has been called.";
    mAboutWin->show();
}

void MainWindow::settingSlot() const {
    qDebug() << "Settings slot has been called.";
    mSettingsWin->LoadSettings();
    mSettingsWin->show();
}

void MainWindow::updateSlot() const {
    qDebug() << "Update slot has been called.";
    mUi->indexButton->setDisabled(true);
    mUi->updateButton->setDisabled(true);
    mUpdateStep->StartUpdate(mIndexStep->mEntryList, mIndexStep);
}

void MainWindow::indexSlot() const {
    qDebug() << "Index slot has been called.";
    mUi->indexButton->setDisabled(true);
    mUi->updateButton->setDisabled(true);
    mIndexStep->startIndex();
}

void MainWindow::listContextMenu(const QPoint & pos) {
    qDebug() << "List context menu has been called.";
    QMenu menu(this);
    menu.addAction(mListClearAct);
    menu.addAction(mListSelAllAct);
    menu.exec(mUi->listWidget->mapToGlobal(pos));
}

void MainWindow::listClear() const {
    mUi->listWidget->clear();
    qDebug() << "List has been cleared.";
}

void MainWindow::listSelAll() const {
    qDebug() << "List select all slot has been called.";
    mUi->listWidget->selectAll();
}

void MainWindow::tableSelAll() const {
    qDebug() << "Table select all slot has been called.";
    mUi->tableWidget->selectAll();
    mUi->tableWidget->setFocus();
}

void MainWindow::tableContextMenu(const QPoint & pos) {
    qDebug() << "Table context menu has been called.";
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
    qDebug() << "Table info slot has been called.";
    mPackInfoWin->OpenInfoWin();
}

void MainWindow::setMessage(QString msg) const {
    mUi->listWidget->addItem(msg);
    mUi->listWidget->scrollToBottom();
    qInfo() << "[GUI]" << msg;
}
