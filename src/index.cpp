#include "index.h"
#include <QDebug>

Index::Index(QWidget *_MW, Ui::MainWindow *_MWUI, info *_Inf) : BaseSteps(_MW, _MWUI)
{
	this->Inf = _Inf;
	this->http = new QHttp();

	connect(this->http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
	connect(this->http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
	connect(this->http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
}

Index::~Index()
{
	this->httpRequestAborted = true;
	this->http->abort();
	this->http->close();
}

void Index::StartIndex()
{
	this->Inf->requestStopAction();
	this->httpRequestAborted = true;
	this->http->abort();
	QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
	this->FolderName = settings.value("FolderName").toString();
	this->SetMessage(tr("Индексирование:"));
	this->sizeOfClient = 0;
	this->sizeOfNeedUpdate = 0;
	this->sizeOfServer = 0;
	this->FilesList.clear();
	this->mFileListForDel.clear();
	this->InitProgBar(0, 1, 0, 1);
	IndexFile = settings.value("curServer").toString() + settings.value("IndexFile").toString();
	IndexForDelFile = settings.value("curServer").toString() + settings.value("IndexForDelFile").toString();
	this->SetMessage(tr("Downloading the indexes files from server \"%1\" ...").arg(QUrl(IndexFile).host()));
	this->CopyRemoteFile(IndexFile, getIndexFilePath());	
	//this->CopyRemoteFile(IndexFile, "sss");
}

void Index::EndIndex(int Next)
{
	if (Next)
	{
		this->sizeOfNeedUpdate = this->sizeOfServer - this->sizeOfClient;
		//TODO: разобраццо с лишними символами в мегабайтах
		float fsizeOfNeedUpdate, fsizeOfServer;
		fsizeOfNeedUpdate = (float)this->sizeOfNeedUpdate / 1048576;
		fsizeOfServer = (float)this->sizeOfServer / 1048576;
		char cstrSizeNeed[13];
		sprintf(cstrSizeNeed, "%10.2f", fsizeOfNeedUpdate);
		char cstrSizeAll[13];
		sprintf(cstrSizeAll, "%10.2f", fsizeOfServer);
		QString strSizeNeed(cstrSizeNeed), strSizeAll(cstrSizeAll);
		this->SetMessage(tr("Индексация завершена!"));
		//1048576
		if (this->sizeOfNeedUpdate != 0)
		{
			this->SetMessage(tr("Для полного обновления вам необходимо загрузить %1 MB из %2 MB").arg(strSizeNeed, strSizeAll));
			this->SetMessage(tr("Выделите необходимые пакеты X-CSL моделей в списке и нажмите \"Обновить\"."));
		}
		else
		{
			this->SetMessage(tr("Поздравляем!, У вас имеется полная последняя версия пакетов X-CSL моделей."));
		}
		this->Inf->GetInfoToTable();
		this->MWUI->NextButton->setEnabled(true);
		this->MWUI->PrevButton->setEnabled(true);
	}
	else
	{
		this->SetMessage(tr("Невозможно выполнить индексацию!"));
		this->MWUI->PrevButton->setEnabled(true);
	}
	this->httpRequestAborted = true;
	this->http->abort();
}

void Index::ParserIndexFile()
{
	QString FileForDelPath = getIndexForDelFilePath();
	QFile fileForDel(FileForDelPath);
	if (!fileForDel.open(QIODevice::ReadOnly))
	{
		this->SetMessage(tr("Ошибка: %1").arg(fileForDel.errorString()));
		this->EndIndex(false);
		return;
	}
	while(!fileForDel.atEnd()){
		
		QString line = fileForDel.readLine();
		QString type = line.left(1);
		if (type == "#" || type == "0" || type == false) continue;
		QStringList list = line.split("%", QString::SkipEmptyParts);
		if (list.size() >= 2){
			FilesTypes fileInfo;
			fileInfo.ID = list[0].toInt();
			fileInfo.List = list;
			fileInfo.State = -999;
			this->mFileListForDel.push_back(fileInfo);
		}
	}
	fileForDel.close();

	QString FilePath = getIndexFilePath();
	QFile file(FilePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		this->SetMessage(tr("Ошибка: %1").arg(file.errorString()));
		this->EndIndex(false);
		return;
	}
	if (file.size() < 1)
	{
		this->SetMessage(tr("Ошибка: Индексный файл имеет нулевой размер! %1").arg(file.size()));
		this->EndIndex(false);
		return;
	}
	//this->SetMessage(tr("Индексируем CSL модели..."));
	// QTextStream in(&file);
	int count = 0;
	int ver_file_stat = true;
	this->MWUI->tableWidget->clearContents();
	this->MWUI->tableWidget->setRowCount(0);
	while (!file.atEnd())
	{
		QString line = file.readLine();
		//qDebug() << line;
		QString type = line.left(1);
		if (ver_file_stat)
		{
			if (type != "0")
			{
				this->SetMessage(tr("Ошибка: Индексный файл имеет не верный формат!"));
				file.close();
				this->EndIndex(false);
				return;
			}
			ver_file_stat = false;
		}
		if (type == "#" || type == "0" || type == false) continue;
		QStringList list = line.split("%", QString::SkipEmptyParts);
		if (list.size() >= 6 && list[0] == "11")
		{
			this->MWUI->tableWidget->setRowCount(count + 1);
			char str[MAX_PATH];
			sprintf(str, "%i", count);
			this->MWUI->tableWidget->setItem(count, 0, new QTableWidgetItem(str));
			this->MWUI->tableWidget->setItem(count, 1, new QTableWidgetItem(list[1]));
			this->MWUI->tableWidget->setItem(count, 2, new QTableWidgetItem(tr("Подождите...")));
			QString msg = tr("%3 (%4)").arg(list[4], list[5]);
			this->MWUI->tableWidget->setItem(count, 3, new QTableWidgetItem(msg));
			//this->sizeOfServer += list[2].toInt();
			float sizeMB = list[2].toFloat() / 1048576;
			char sizeMBstr[MAX_PATH];
			sprintf(sizeMBstr, "%10.2f", sizeMB);
			this->MWUI->tableWidget->setItem(count, 4, new QTableWidgetItem(sizeMBstr));
			int status = this->CheckCslPack(file.pos(), count);
			char StrStatus[MAX_PATH];
			sprintf(StrStatus, "%i", status);
			QTableWidgetItem *Item = new QTableWidgetItem();
			switch (status)
			{
			case 0:
				Item->setText(tr("Установлено"));
				Item->setTextColor(Qt::darkGreen);
				this->MWUI->tableWidget->setItem(count, 5, Item);
				//this->MWUI->tableWidget->setItem(count, 5, new QTableWidgetItem(tr("Установлено")));
				this->MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
				break;
			case 1:
				Item->setText(tr("Требует обновления"));
				Item->setTextColor(Qt::red);
				this->MWUI->tableWidget->setItem(count, 5, Item);
				//this->MWUI->tableWidget->setItem(count, 5, new QTableWidgetItem(tr("Требует обновления")));
				this->MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
				break;
			case -1:
				this->MWUI->tableWidget->setItem(count, 5, new QTableWidgetItem(tr("Не установлено")));
				this->MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
				break;
			default:
				this->MWUI->tableWidget->setItem(count, 5, new QTableWidgetItem(tr("Не выяснено")));
				this->MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
				break;
			}
			count++;
		}
	}
	file.close();
	//this->MWUI->tableWidget->sortByColumn(1);
	this->MWUI->tableWidget->sortItems(1);
	this->EndIndex();
}

int Index::CheckCslPack(int pos, int ID)
{
	QString FilePath = getIndexFilePath();
	QFile file(FilePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		this->SetMessage(tr("Ошибка: %1").arg(file.errorString()));
		return _CLIENT_FILE_STATUS_LOST;
	}
	QTextStream in(&file);
	in.seek(pos);
	int status = 0;
	while (!in.atEnd())
	{
		QString line = in.readLine();
		QString type = line.left(1);
		if (type == "#" || type == "0" || type == false) continue;
		QStringList list = line.split("%", QString::SkipEmptyParts);
		if (list[0] == "11") break;
		if (list[0] == "10")
		{
			int st = this->CheckFile(list, ID);
			if (st != 0) status = 1;
		}
	}

	file.close();
	return status;
}

int Index::CheckFile(QStringList List, int ID)
{
	FilesTypes FilesInfo;
	FilesInfo.ID = ID;
	FilesInfo.List = List;
	QString separator = (QString)QDir::separator();
	QString FilePath = this->FolderName + this->separator + List[1];
	QFileInfo fileInfo(FilePath);
	this->sizeOfServer += List[2].toInt();
	// файл существует?
	if (!fileInfo.isFile())
	{
		FilesInfo.State = _CLIENT_FILE_STATUS_LOST;
		this->FilesList.push_back(FilesInfo);
		return _CLIENT_FILE_STATUS_LOST;
	}
	// размер файла совпадает?
	long int size;
	size = List[2].toInt();
	if (size != (int)fileInfo.size())
	{
		FilesInfo.State = _CLIENT_FILE_STATUS_CHANGE;
		this->FilesList.push_back(FilesInfo);
		return _CLIENT_FILE_STATUS_CHANGE;
	}
	// check hash if it is available
	if (List[3] != "Reserve"){
		static QCryptographicHash hash(QCryptographicHash::Md5);
		hash.reset();
		static QFile file;
		file.setFileName(FilePath);
		if (file.open(QIODevice::ReadOnly)){
			hash.addData(file.readAll(), QCryptographicHash::Md5);
			if (List[3] != QString(hash.result())){
				FilesInfo.State = _CLIENT_FILE_STATUS_CHANGE;
				this->FilesList.push_back(FilesInfo);
				return _CLIENT_FILE_STATUS_CHANGE;
			}
			file.close();
		}
		else{
			FilesInfo.State = _CLIENT_FILE_STATUS_LOST;
			this->FilesList.push_back(FilesInfo);
			return _CLIENT_FILE_STATUS_LOST;
		}
	}

	// Дата и время файла совпадает?
	/*QString date_end_time;
	date_end_time = file.lastModified().toString("dd.MM.yyyy hh:mm:ss");
	if (date_end_time != List[4]+" "+List[5])
	{
	FilesInfo.State = _CLIENT_FILE_STATUS_CHANGE;
	this->FilesList.push_back(FilesInfo);
	return _CLIENT_FILE_STATUS_CHANGE
	}*/
	// типа все ОК
	this->sizeOfClient += (int)fileInfo.size();
	FilesInfo.State = _CLIENT_FILE_STATUS_OK;
	this->FilesList.push_back(FilesInfo);
	return _CLIENT_FILE_STATUS_OK;
}

void Index::CopyRemoteFile(QString From, QString To)
{
	QUrl url(From);
	QString fileName = To;

	if (QFile::exists(fileName)) QFile::remove(fileName);

	this->file = new QFile(fileName);

	if (!file->open(QIODevice::WriteOnly))
	{
		this->SetMessage(tr("Ошибка: Не могу записать файл на ваш компьютер - %1 : %2.").arg(fileName).arg(this->file->errorString()));
		delete this->file;
		this->file = 0;
		return;
	}
	this->http->setHost(url.host());
	this->httpRequestAborted = false;
	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
	if (path.isEmpty()) path = "/";
	this->DownSize = 0;
	this->TotalDownSize = 0;
	this->httpGetId = this->http->get(path, this->file);
	//this->SetMessage(tr("Скачиваем индексный файл с сервера \"%1\" ...").arg(url.host()));
}

QString Index::getIndexFilePath()
{
#ifdef Q_OS_WIN32
	return "x-csl-indexes.idx";
#else
	QString path(
		QDir::homePath()
		+this->separator+
		".config"
		+this->separator+
		"X-CSL-Updater"
		+this->separator
		);

	QDir pathDir(path);
	if (!pathDir.exists()) pathDir.mkpath(path);

	return path += "x-csl-indexes.idx";

#endif
}

QString Index::getIndexForDelFilePath()
{
#ifdef Q_OS_WIN32
	return "x-csl-indexes-for-delete.idx";
#else
	QString path(
		QDir::homePath()
		+this->separator+
		".config"
		+this->separator+
		"X-CSL-Updater"
		+this->separator
		);

	QDir pathDir(path);
	if (!pathDir.exists()) pathDir.mkpath(path);

	return path += "x-csl-indexes-for-delete.idx";

#endif
}

void Index::httpRequestFinished(int reqId, bool error)
{
	static int count = 0;
	if (reqId != this->httpGetId) return;
	if (this->httpRequestAborted)
	{
		if (this->file)
		{
			this->file->close();
			this->file->remove();
			delete this->file;
			this->file = 0;
		}
		return;
	}
	if (error)
	{
		this->SetMessage(tr("Ошибка: %1").arg(this->http->errorString()));
	}
	this->file->close();
	delete this->file;
	this->file = 0;

	if (!error && count == 0){
		this->CopyRemoteFile(IndexForDelFile, getIndexForDelFilePath());
		count++;
	}
	else if (!error && count == 1)
	{
		count = 0;
		this->ParserIndexFile();
	}
	else
	{
		this->EndIndex(false);
	}
}

void Index::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
	switch (responseHeader.statusCode())
	{
	case 200:                   // Ok
	case 301:                   // Moved Permanently
	case 302:                   // Found
	case 303:                   // See Other
	case 307:                   // Temporary Redirect
		// these are not error conditions
		break;

	default:
		this->SetMessage(tr("Ошибка : %1.").arg(responseHeader.reasonPhrase()));
		this->httpRequestAborted = true;
		this->http->abort();
		this->EndIndex(false);
		break;
	}
}

void Index::updateDataReadProgress(int bytesRead, int totalBytes)
{
	if (this->httpRequestAborted) return;
	this->MWUI->progressBar->setMaximum(totalBytes);
	this->MWUI->progressBar->setValue(bytesRead);
	this->TotalDownSize = totalBytes;
	this->DownSize = bytesRead;
}

