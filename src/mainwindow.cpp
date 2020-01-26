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

    // x-plane dir stuff
    mXplaneDir = settings.value("mXplaneDir", "").toString();
    if (!mXplaneDir.exists()) {
        if (!SetupXplaneDir()) {
            QTimer::singleShot(0, qApp, &QApplication::quit);
        }
    }    

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
    this->mUi->curPathLabel->setText(mXplaneDir.path());
    this->mUi->progressBar->setValue(0);
    this->mUi->listWidget->addItem(tr("X-CSL-Updater, Ver.:") + gProgramVersion);

    // 
    /*QLocale Loc;
    QString local(Loc.name());
    this->ui->listWidget->addItem(local);*/
    this->mUi->listWidget->addItem(tr("Specify the X-Plane executable file location and then click \"Index\""));
    this->mUi->listWidget->addItem(tr("to determine files need to be updated."));
    this->mUi->listWidget->scrollToBottom();

    // Vars init

    // Objs Init
    this->AboutWin = new About(this);
    this->SettingsWin = new Settings(this);
    this->Inf = new PackageAdditionalInfo(this, this->mUi);
    this->Indx = new IndexStep(this, this->mUi, this->Inf);
    this->Updt = new UpdateStep(this, this->mUi);

    // connects
    connect(this->mUi->actionAbout, &QAction::triggered, this, &MainWindow::AboutSlot);
    connect(this->mUi->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(this->mUi->actionSetFolder, &QAction::triggered, this, &MainWindow::SetupXplaneDir);
    connect(this->mUi->actionSettings, &QAction::triggered, this, &MainWindow::SettingSlot);
    connect(this->mUi->listWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::ListContextMenu);
    connect(this->mUi->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::TableContextMenu);
    connect(this->mUi->SelAllButton, &QPushButton::pressed, this, &MainWindow::TableSelAll);
    connect(this->mUi->NextButton, &QPushButton::pressed, this, &MainWindow::UpdateSlot);
    connect(this->mUi->PrevButton, &QPushButton::pressed, this, &MainWindow::IndexSlot);
    connect(this->mUi->ButtonSetFolder, &QPushButton::pressed, this, &MainWindow::SetupXplaneDir);
    connect(this->mUi->actionSet_Custom_Path, &QAction::triggered, this, &MainWindow::SetCustomFolder);
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

QString MainWindow::removeCslSpecifiedPathIfNeeded(const QString & inPath) {
    static QString xIvApSpecificPath = "Resources/plugins/X-IvAp Resources/CSL";
    if (inPath.endsWith(xIvApSpecificPath)) {
        return inPath.left(inPath.length() - xIvApSpecificPath.length());
    }
    return inPath;
}

void MainWindow::TableInfo() {
    this->Inf->OpenInfoWin();
}

bool MainWindow::SetupXplaneDir() {
    QString const startDir = mXplaneDir.path();

#ifdef Q_OS_WIN32
     QString const newDir = QFileDialog::getOpenFileName(this,
                                         tr("Specify the X-Plane executable file location:"),
                                         startDir,
                                         "X-Plane*.exe (X-Plane*.exe)");
#elif defined Q_OS_LINUX
	 QString const newDir = QFileDialog::getOpenFileName(this,
		tr("Specify the X-Plane executable file location:"),
		startDir,
		"X-Plane* (X-Plane*)");
#else
	 QString const newDir = QFileDialog::getOpenFileName(this,
		tr("Specify the X-Plane executable file location:"),
		startDir,
		"X-Plane*.app (X-Plane*.app)");
#endif

    QFileInfo const fileInfo(newDir);
    QDir const simPluginsDir(fileInfo.dir().path() + "/" + gSimPluginsDir.path());
    if (fileInfo.exists() && simPluginsDir.exists()) {
        mXplaneDir = fileInfo.dir();
        QSettings settings(gSettingsFileName, QSettings::IniFormat);
        settings.setValue("mXplaneDir", mXplaneDir.path());
        mUi->curPathLabel->setText(removeCslSpecifiedPathIfNeeded(mXplaneDir.path()));
        return true;
    }
    this->mUi->listWidget->addItem(tr("Error: The selected X-Plane executable path are not valid!"));
    this->mUi->listWidget->scrollToBottom();
    return false;
}

void MainWindow::SetCustomFolder() {
    QMessageBox::warning(this, PROGRAM_NAME, tr("Warning! This function is designed for advanced users. Please use it only if you understand what you are doing otherwise the program can become unusable!"), QMessageBox::Ok);
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
