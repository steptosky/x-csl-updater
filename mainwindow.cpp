#include "mainwindow.h"
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // setup UI
    ui->setupUi(this);

    // установка нужного кодека
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    QTextCodec::setCodecForTr(codec);

    // сепаратор
    this->separator = "/"/*(QString)QDir::separator()*/;

    // цепляем настройки из сохраненых
    QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
    move(settings.value("pos", QPoint(200, 200)).toPoint());

#ifdef WIN
    QString default_path =
            tr("C:")+this->separator+
            tr("X-Plane")+this->separator+
            tr("Resources")+this->separator+
            tr("plugins")+this->separator+
            tr("X-IvAp Resources")+this->separator+
            tr("CSL");
#elif LIN
    //TODO : make a correct default path for unix system
    QString default_path =
            tr("C:")+this->separator+
            tr("X-Plane")+this->separator+
            tr("Resources")+this->separator+
            tr("plugins")+this->separator+
            tr("X-IvAp Resources")+this->separator+
            tr("CSL");
#else
    QString default_path =
            QDir::homePath()+this->separator+
            tr("X-Plane")+this->separator+
            tr("Resources")+this->separator+
            tr("plugins")+this->separator+
            tr("X-IvAp Resources")+this->separator+
            tr("CSL");
#endif

    this->FolderName = settings.value("FolderName", default_path).toString();

    // контекст меню ЛИСТА
    this->ListClearAct = new QAction (tr("Очистить"), this);
    connect(this->ListClearAct, SIGNAL(activated()), this, SLOT(ListClear()));
    this->ListSelAllAct = new QAction (tr("Выделить Все"), this);
    this->ListSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(this->ListSelAllAct, SIGNAL(activated()), this, SLOT(ListSelAll()));

    // контекст меню Таблицы
    this->TableSelAllAct = new QAction (tr("Выделить Все"), this);
    this->TableSelAllAct->setShortcut(tr("Ctrl+A"));
    connect(this->TableSelAllAct, SIGNAL(activated()), this, SLOT(TableSelAll()));
    this->TableInfoAct = new QAction (tr("Информация"), this);
    connect(this->TableInfoAct, SIGNAL(activated()), this, SLOT(TableInfo()));

    // ширина колонок таблицы
    this->ui->tableWidget->setColumnWidth(0, 30);//ID
    this->ui->tableWidget->setColumnWidth(1, 150);//Title
    this->ui->tableWidget->setColumnWidth(2, 200);//Info
    this->ui->tableWidget->setColumnWidth(3, 130);//Ver
    this->ui->tableWidget->setColumnWidth(4, 80);//sizeMB
    this->ui->tableWidget->setColumnWidth(5, 120);//status
    this->ui->tableWidget->setColumnWidth(6, 20);//code

    // всяко разно пишем в окне и другое
    this->ui->curPathLabel->setText(removeCslSpecifiedPath(this->FolderName));
    this->ui->progressBar->setValue(0);
    this->ui->listWidget->addItem(tr("X-CSL-Updater, Ver.:")+ VerProg);

    // Начало
    /*QLocale Loc;
    QString local(Loc.name());
    this->ui->listWidget->addItem(local);*/
    this->ui->listWidget->addItem(tr("Укажите путь к файлу X-Plane.exe и нажмите \"Индексировать\""));
    this->ui->listWidget->addItem(tr("для индексирования и определения файлов, нуждающихся в обновлении."));
    this->ui->listWidget->scrollToBottom();

    // инициализация переменных

    // инициализация обьектов
    this->AboutWin = new About(this);
    this->SettingsWin = new Settings(this);
    this->Inf = new info(this, this->ui);
    this->Indx = new Index(this, this->ui, this->Inf);
    this->Updt = new Update(this, this->ui);

    // коннекты
    connect(this->ui->actionAbout, SIGNAL(activated()), this, SLOT(AboutSlot()));
    connect(this->ui->actionAbout_Qt, SIGNAL(activated()), qApp, SLOT(aboutQt()));
    connect(this->ui->actionSetFolder, SIGNAL(activated()), this, SLOT(SetFolder()));
    connect(this->ui->actionSettings, SIGNAL(activated()), this, SLOT(SettingSlot()));
    connect(this->ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ListContextMenu(const QPoint &)));
    connect(this->ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TableContextMenu(const QPoint &)));
    connect(this->ui->SelAllButton, SIGNAL(pressed()), this, SLOT(TableSelAll()));
    connect(this->ui->NextButton, SIGNAL(pressed()), this, SLOT(UpdateSlot()));
    connect(this->ui->PrevButton, SIGNAL(pressed()), this, SLOT(IndexSlot()));
    connect(this->ui->ButtonSetFolder, SIGNAL(clicked()), this, SLOT(SetFolder()));
    connect(this->ui->actionSet_Custom_Path, SIGNAL(activated()), this, SLOT(SetCustomFolder()));
}

MainWindow::~MainWindow()
{
    QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
    settings.setValue("pos", pos());
    this->Indx->~Index();
    this->Updt->~Update();
    this->Inf->~info();
    delete ui;
}

void MainWindow::ListClear()
{
    this->ui->listWidget->clear();
}

void MainWindow::ListSelAll()
{
    this->ui->listWidget->selectAll();
}

void MainWindow::TableSelAll()
{
    this->ui->tableWidget->selectAll();
    this->ui->tableWidget->setFocus();
}

void MainWindow::AboutSlot()
{    
    this->AboutWin->show();
}

void MainWindow::SettingSlot()
{
    this->SettingsWin->LoadSettings();
    this->SettingsWin->show();
}

void MainWindow::ListContextMenu(const QPoint & pos)
{
    QMenu menu(this);
    menu.addAction(this->ListClearAct);
    menu.addAction(this->ListSelAllAct);
    menu.exec(this->ui->listWidget->mapToGlobal(pos));
}

void MainWindow::TableContextMenu(const QPoint & pos)
{
    QMenu menu(this);
    if (this->ui->tableWidget->rowCount() < 1)
    {
        this->TableInfoAct->setDisabled(true);
    }
    else
    {
        this->TableInfoAct->setEnabled(true);
    }
    menu.addAction(this->TableInfoAct);
    menu.addAction(this->TableSelAllAct);
    menu.exec(this->ui->tableWidget->mapToGlobal(pos));
}

void MainWindow::contextMenuEvent(QContextMenuEvent * event)
{
    /*QMenu menu(this);
     menu.addAction(this->ui->actionAbout);
     menu.addAction(this->ui->actionAbout_Qt);
     menu.exec(event->globalPos());*/
}

QString MainWindow::removeCslSpecifiedPath(const QString &inPath)
{
    return inPath.left(inPath.length()-38);
}

void MainWindow::TableInfo()
{
    this->Inf->OpenInfoWin();
}


void MainWindow::SetFolder()
{
    QString _FName;
    if (this->FolderName.endsWith("Resources/plugins/X-IvAp Resources/CSL"))
    {
        _FName = removeCslSpecifiedPath(this->FolderName);
    }
    else
    {
        _FName = this->FolderName;
    }
    QString FName;

    #ifdef WIN
        FName = QFileDialog::getOpenFileName(this,
                                         tr("Укажите путь к исполняемому файлу X-Plane"),
                                         _FName,
                                         "X-Plane*.exe (X-Plane*.exe)");
    #elif LIN
        FName = QFileDialog::getOpenFileName(this,
                                         tr("Укажите путь к исполняемому файлу X-Plane"),
                                         _FName,
                                         "X-Plane* (X-Plane*)");
    #else
        FName = QFileDialog::getOpenFileName(this,
                                         tr("Укажите путь к исполняемому файлу X-Plane"),
                                         _FName,
                                         "X-Plane*.app (X-Plane*.app)");
    #endif

    if (!FName.isEmpty())
    {
        int pos = FName.lastIndexOf("/");
        FName = FName.left(pos+1);
        FName = FName+tr("Resources/plugins/X-IvAp Resources/CSL");
        QDir dir(FName);
        if (dir.exists())
        {
            this->FolderName = FName;
            QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
            settings.setValue("FolderName", this->FolderName);
            this->ui->curPathLabel->setText(removeCslSpecifiedPath(this->FolderName));
        }
        else
        {
            this->ui->listWidget->addItem(tr("Ошибка: В выбранной версии X-Plane не установлен плагин X-IvAp!"));
            this->ui->listWidget->scrollToBottom();
        }
    }
    return;
}

void MainWindow::SetCustomFolder()
{
    QMessageBox::warning(this, "X-CSL-Updater", tr("Внимание! Данная функция предназначена для профессионального использования. Возможно программа станет неработоспособной!"), QMessageBox::Ok);
    this->FolderName = QFileDialog::getExistingDirectory(this,
                                                         tr("X-CSL-Updater :: Выберите папку X-Plane"), this->FolderName, QFileDialog::ShowDirsOnly);

    if (!this->FolderName.isEmpty())
    {
        QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
        settings.setValue("FolderName", this->FolderName);
        this->ui->curPathLabel->setText(this->FolderName);
        //this->ui->cur_ver_client->setText(this->ChVer->GetCurVerClient(this->FolderName));
    }
    return;
}

void MainWindow::UpdateSlot()
{
    this->Updt->StartUpdate(this->Indx->FilesList, this->Indx);
    this->ui->PrevButton->setDisabled(true);
    this->ui->NextButton->setDisabled(true);
}

void MainWindow::IndexSlot()
{
    this->Indx->StartIndex();
    this->ui->PrevButton->setDisabled(true);
    this->ui->NextButton->setDisabled(true);
}
