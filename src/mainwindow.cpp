#include "mainwindow.h"
#include <QAction>
// #include <QMessageBox>
// #include "QFileDialog"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), mUi(new Ui::MainWindow)
{
	// setup UI
	mUi->setupUi(this);

	// установка нужного кодека
//     QTextCodec *codec = QTextCodec::codecForName("CP1251");
// 	QTextCodec::setCodecForTr(codec);

	// сепаратор
	this->separator = "/"/*(QString)QDir::separator()*/;

	// цепляем настройки из сохраненых
	QSettings settings(ORGANISATION, PROGRAM_NAME);
	move(settings.value("pos", QPoint(200, 200)).toPoint());

#ifdef Q_OS_WIN32
	QString default_path =
		tr("C:") + this->separator +
		tr("X-Plane") + this->separator +
		tr("Resources") + this->separator +
		tr("plugins") + this->separator +
		tr("X-IvAp Resources") + this->separator +
		tr("CSL");
#elif defined Q_OS_LINUX
	QString default_path =
		QDir::homePath()+this->separator+
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
	this->ListClearAct = new QAction(tr("Очистить"), this);
	connect(this->ListClearAct, SIGNAL(activated()), this, SLOT(ListClear()));
	this->ListSelAllAct = new QAction(tr("Выделить Все"), this);
	this->ListSelAllAct->setShortcut(tr("Ctrl+A"));
	connect(this->ListSelAllAct, SIGNAL(activated()), this, SLOT(ListSelAll()));

	// контекст меню Таблицы
	this->TableSelAllAct = new QAction(tr("Выделить Все"), this);
	this->TableSelAllAct->setShortcut(tr("Ctrl+A"));
	connect(this->TableSelAllAct, SIGNAL(activated()), this, SLOT(TableSelAll()));
	this->TableInfoAct = new QAction(tr("Информация"), this);
	connect(this->TableInfoAct, SIGNAL(activated()), this, SLOT(TableInfo()));

	// ширина колонок таблицы
	this->mUi->tableWidget->setColumnWidth(0, 30);//ID
	this->mUi->tableWidget->setColumnWidth(1, 150);//Title
	this->mUi->tableWidget->setColumnWidth(2, 230);//Info
	this->mUi->tableWidget->setColumnWidth(3, 130);//Ver
	this->mUi->tableWidget->setColumnWidth(4, 80);//sizeMB
	this->mUi->tableWidget->setColumnWidth(5, 120);//status
	this->mUi->tableWidget->setColumnWidth(6, 20);//code
	this->mUi->tableWidget->setColumnHidden(6, true);

	// всяко разно пишем в окне и другое
	this->mUi->curPathLabel->setText(removeCslSpecifiedPath(this->FolderName));
	this->mUi->progressBar->setValue(0);
	this->mUi->listWidget->addItem(tr("X-CSL-Updater, Ver.:") + VerProg);

	// Начало
	/*QLocale Loc;
	QString local(Loc.name());
	this->ui->listWidget->addItem(local);*/
	this->mUi->listWidget->addItem(tr("Укажите путь к файлу X-Plane.exe и нажмите \"Индексировать\""));
	this->mUi->listWidget->addItem(tr("для индексирования и определения файлов, нуждающихся в обновлении."));
	this->mUi->listWidget->scrollToBottom();

	// инициализация переменных

	// инициализация обьектов
	this->AboutWin = new About(this);
	this->SettingsWin = new Settings(this);
	this->Inf = new Info(this, this->mUi);
	this->Indx = new Index(this, this->mUi, this->Inf);
	this->Updt = new Update(this, this->mUi);

	// коннекты
	connect(this->mUi->actionAbout, SIGNAL(activated()), this, SLOT(AboutSlot()));
	connect(this->mUi->actionAbout_Qt, SIGNAL(activated()), qApp, SLOT(aboutQt()));
	connect(this->mUi->actionSetFolder, SIGNAL(activated()), this, SLOT(SetFolder()));
	connect(this->mUi->actionSettings, SIGNAL(activated()), this, SLOT(SettingSlot()));
	connect(this->mUi->listWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ListContextMenu(const QPoint &)));
	connect(this->mUi->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TableContextMenu(const QPoint &)));
	connect(this->mUi->SelAllButton, SIGNAL(pressed()), this, SLOT(TableSelAll()));
	connect(this->mUi->NextButton, SIGNAL(pressed()), this, SLOT(UpdateSlot()));
	connect(this->mUi->PrevButton, SIGNAL(pressed()), this, SLOT(IndexSlot()));
	connect(this->mUi->ButtonSetFolder, SIGNAL(clicked()), this, SLOT(SetFolder()));
	connect(this->mUi->actionSet_Custom_Path, SIGNAL(activated()), this, SLOT(SetCustomFolder()));
}

MainWindow::~MainWindow()
{
	QSettings settings(ORGANISATION, PROGRAM_NAME);
	settings.setValue("pos", pos());
	delete Indx;
	delete Updt;
	delete Inf;
	delete mUi;
}

void MainWindow::ListClear()
{
	this->mUi->listWidget->clear();
}

void MainWindow::ListSelAll()
{
	this->mUi->listWidget->selectAll();
}

void MainWindow::TableSelAll()
{
	this->mUi->tableWidget->selectAll();
	this->mUi->tableWidget->setFocus();
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
	menu.exec(this->mUi->listWidget->mapToGlobal(pos));
}

void MainWindow::TableContextMenu(const QPoint & pos)
{
	QMenu menu(this);
	if (this->mUi->tableWidget->rowCount() < 1)
	{
		this->TableInfoAct->setDisabled(true);
	}
	else
	{
		this->TableInfoAct->setEnabled(true);
	}
	menu.addAction(this->TableInfoAct);
	menu.addAction(this->TableSelAllAct);
	menu.exec(this->mUi->tableWidget->mapToGlobal(pos));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *)
{
	/*QMenu menu(this);
	 menu.addAction(this->ui->actionAbout);
	 menu.addAction(this->ui->actionAbout_Qt);
	 menu.exec(event->globalPos());*/
}

QString MainWindow::removeCslSpecifiedPath(const QString &inPath)
{
	return inPath.left(inPath.length() - 38);
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

#ifdef Q_OS_WIN32
	FName = QFileDialog::getOpenFileName(this,
		tr("Укажите путь к исполняемому файлу X-Plane"),
		_FName,
		"X-Plane*.exe (X-Plane*.exe)");
#elif defined Q_OS_LINUX
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
		FName = FName.left(pos + 1);
		FName = FName + tr("Resources/plugins/X-IvAp Resources/CSL");
		QDir dir(FName);
		if (dir.exists())
		{
			this->FolderName = FName;
			QSettings settings(ORGANISATION, PROGRAM_NAME);
			settings.setValue("FolderName", this->FolderName);
			this->mUi->curPathLabel->setText(removeCslSpecifiedPath(this->FolderName));
		}
		else
		{
			this->mUi->listWidget->addItem(tr("Ошибка: В выбранной версии X-Plane не установлен плагин X-IvAp!"));
			this->mUi->listWidget->scrollToBottom();
		}
	}
	return;
}

void MainWindow::SetCustomFolder()
{
	QMessageBox::warning(this, PROGRAM_NAME, tr("Внимание! Данная функция предназначена для профессионального использования. Возможно программа станет неработоспособной!"), QMessageBox::Ok);
	this->FolderName = QFileDialog::getExistingDirectory(this,
		tr("X-CSL-Updater :: Выберите папку X-Plane"), this->FolderName, QFileDialog::ShowDirsOnly);

	if (!this->FolderName.isEmpty())
	{
		QSettings settings(ORGANISATION, PROGRAM_NAME);
		settings.setValue("FolderName", this->FolderName);
		this->mUi->curPathLabel->setText(this->FolderName);
		//this->ui->cur_ver_client->setText(this->ChVer->GetCurVerClient(this->FolderName));
	}
	return;
}

void MainWindow::UpdateSlot()
{
	this->mUi->PrevButton->setDisabled(true);
	this->mUi->NextButton->setDisabled(true);
	this->Updt->StartUpdate(this->Indx->mEntryList, this->Indx);	
}

void MainWindow::IndexSlot()
{
	this->mUi->PrevButton->setDisabled(true);
	this->mUi->NextButton->setDisabled(true);
	this->Indx->StartIndex();	
}
