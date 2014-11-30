#include "update.h"

Update::Update(QWidget *_MW, Ui::MainWindow *_MWUI) : BaseSteps(_MW, _MWUI)
{
	this->http = new QHttp();

	connect(this->http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
	connect(this->http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
	connect(this->http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	connect(this->MWUI->CancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));

}

Update::~Update()
{
	this->http->abort();
	this->http->close();
}

void Update::CancelSlot()
{
	this->SetMessage(tr("Операция прервана пользователем! Подождите..."));
	this->httpRequestAborted = true;
	this->MWUI->CancelButton->setEnabled(false);
}

bool Update::removeDir(const QString & dirName)
{
	bool result = false;
	QDir dir(dirName);

	if (dir.exists(dirName)) {
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
			if (info.isDir()) {
				result = removeDir(info.absoluteFilePath());
			}
			else {
				result = QFile::remove(info.absoluteFilePath());
			}
		}
		result = dir.rmdir(dirName);
	}
	return result;
}

bool Update::removePath(QString path) {
	QString correctedPath = QDir::toNativeSeparators(this->FolderName + separator + path).trimmed();
	//qDebug() << correctedPath;
	QFileInfo fileInfo(correctedPath);
	QDir dir(correctedPath);
	if (fileInfo.isFile()){
		return QFile::remove(correctedPath);
	}
	else{
		return removeDir(correctedPath);
	}
}

void Update::StartUpdate(QVector<FilesTypes> _FilesList, Index *_Indx)
{
	this->MWUI->CancelButton->setEnabled(true);
	QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
	this->FolderName = settings.value("FolderName").toString();
	this->FilesList.clear();
	mSelectedListForDelete.clear();
	this->Indx = _Indx;
	this->server = settings.value("curServer").toString();
	int size = _FilesList.size();
	int RowCount = this->MWUI->tableWidget->rowCount();
	int sizeForDel = Indx->mFileListForDel.size();
	// determine files to delete
	// I think we must delete files in any case during update. so it code was commented...
// 	for (int i = 0; i < sizeForDel; i++){
// 
// 		if (Indx->mFileListForDel[i].ID == 11){
// 			mSelectedListForDelete.push_back(Indx->mFileListForDel[i]);
// 			continue;
// 		}
// 
// 		// I think we must delete files in any case during update. so it code was commented...
// 		for (int ii = 0; ii < RowCount; ii++)
// 		{
// 			if (this->MWUI->tableWidget->item(ii, 0)->isSelected())
// 			{
// 				QStringList list = Indx->mFileListForDel[i].List[1].split("/", QString::SkipEmptyParts);
// 				if (this->MWUI->tableWidget->item(ii, 1)->text() == list[0] && Indx->mFileListForDel[i].ID != 11){
// 					mSelectedListForDelete.push_back(Indx->mFileListForDel[i]);
// 					break;
// 				}
// 			}
// 		}
// 	}
	// determine files to update
	for (int it = 0; it < size; it++)
	{
		for (int i = 0; i < RowCount; i++)
		{
			if (this->MWUI->tableWidget->item(i, 0)->isSelected())
			{
				if (this->MWUI->tableWidget->item(i, 6)->text() == "1")
				{					
					if (_FilesList[it].ID == this->MWUI->tableWidget->item(i, 0)->text().toInt())
					{
						if (_FilesList[it].State != 0)
						{
							this->FilesList.push_back(_FilesList[it]);
						}
					}
				}
			}
		}
	}
	this->InitProgBar(0, 1, 0, 1);
	// added task for download mtl.dat
	FilesTypes fileInfo;
	fileInfo.ID = -999;
	fileInfo.List.append("0");
	fileInfo.List.append("mtl.dat");
	fileInfo.State = -999;
	this->FilesList.push_front(fileInfo);
	if (!this->FilesList.empty())
	{
		this->countMain = 0;
		// first task - mtl.dat, so make correct path
		QString corrFolderName = this->FolderName.left(this->FolderName.length() - 4);
		this->CopyRemoteFile(this->server + this->FilesList[this->countMain].List[1], corrFolderName + separator + this->FilesList[this->countMain].List[1]);
	}
	else
	{
		this->EndUpdate();
	}
}

void Update::EndUpdate()
{
	// remove files was planed to delete
	int size = Indx->mFileListForDel.size();
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (removePath(Indx->mFileListForDel[i].List[1])){
			count++;
			//this->SetMessage(tr("Removed file or directory: %1").arg(Indx->mFileListForDel[i].List[1]));
		}
	}
	if (count > 0){
		this->SetMessage(tr("Cleanup procedure done. Removed %1 files.").arg(count));
	}	

// 	int size = mSelectedListForDelete.size();
// 	for (int i = 0; i < size; i++)
// 	{
// 		removePath(mSelectedListForDelete[i].List[1]);	
// 	}

	this->MWUI->CancelButton->setEnabled(false);
	this->SetMessage(tr("Обновление завершено!"));
	this->Indx->StartIndex();
	this->MWUI->PrevButton->setEnabled(true);
	this->MWUI->NextButton->setEnabled(true);
}

void Update::CopyRemoteFile(QString From, QString To)
{
	QUrl url(From);
	QString fileName = To;

	if (QFile::exists(fileName)) QFile::remove(fileName);

	this->file = new QFile(fileName);
	this->file->open(QIODevice::WriteOnly);

	if (!this->file->isOpen())
	{
		int index = fileName.lastIndexOf(tr("/"));
		const QString dir_path = fileName.left(index);
		QDir dir("");
		dir.mkpath(dir_path);
		delete this->file;
		this->file = 0;
		this->file = new QFile(fileName);
		this->file->open(QIODevice::WriteOnly);
	}

	if (!this->file->isOpen())
	{
		this->SetMessage(tr("Ошибка: Не могу записать файл на ваш компьютер - %1 : %2.").arg(fileName).arg(this->file->errorString()));
		delete this->file;
		this->file = 0;
		if (this->countMain < this->FilesList.size() - 1)
		{
			this->countMain++;
			this->CopyRemoteFile(this->server + this->FilesList[this->countMain].List[1], this->FolderName + tr("/") + this->FilesList[this->countMain].List[1]);
		}
		else
		{
			this->EndUpdate();
		}
		return;
	}
	this->http->setHost(url.host());
	this->httpRequestAborted = false;
	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
	if (path.isEmpty()) path = "/";
	this->DownSize = 0;
	this->TotalDownSize = 0;
	this->httpGetId = this->http->get(path, this->file);
	this->SetMessage(tr("Обновляем: %1...").arg(To));
}

void Update::httpRequestFinished(int reqId, bool error)
{
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
		this->EndUpdate();
		return;
	}
	if (error)
	{
		this->SetMessage(tr("Ошибка: %1").arg(this->http->errorString()));
	}
	this->file->close();
	delete this->file;
	this->file = 0;
	if (this->countMain < this->FilesList.size() - 1)
	{
		this->countMain++;
		this->CopyRemoteFile(this->server + this->FilesList[this->countMain].List[1], this->FolderName + tr("/") + this->FilesList[this->countMain].List[1]);
	}
	else
	{
		this->EndUpdate();
	}
}

void Update::readResponseHeader(const QHttpResponseHeader &responseHeader)
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
// 		if (this->countMain < this->FilesList.size() - 1)
// 		{
// 			this->countMain++;
// 			this->CopyRemoteFile(this->server + this->FilesList[this->countMain].List[1], this->FolderName + tr("/") + this->FilesList[this->countMain].List[1]);
// 		}
// 		else
// 		{
// 			this->EndUpdate();
// 		}
		break;
	}

}

void Update::updateDataReadProgress(int bytesRead, int totalBytes)
{
	if (this->httpRequestAborted) return;
	this->MWUI->progressBar->setMaximum(totalBytes);
	this->MWUI->progressBar->setValue(bytesRead);
	this->TotalDownSize = totalBytes;
	this->DownSize = bytesRead;
	//this->InitProgBar(0, totalBytes, bytesRead, 1);
}

