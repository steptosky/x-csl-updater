#include "update.h"

Update::Update(QWidget *_MW, Ui::MainWindow *_MWUI) : BaseSteps(_MW, _MWUI) {
	mNetMng = new QNetworkAccessManager(this);
	connect(mNetMng, &QNetworkAccessManager::finished, this, &Update::httpRequestFinished);
	connect(MWUI->CancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));

}

Update::~Update() {
	delete mNetMng;
	delete mDownloadingFile;
}

void Update::CancelSlot() {
	SetMessage(tr("Операция прервана пользователем!"));
	emit cancelDownloading();

	MWUI->CancelButton->setEnabled(false);
	MWUI->PrevButton->setEnabled(false);
	MWUI->NextButton->setEnabled(false);
	mIndexStep->StartIndex();
}

bool Update::removeDir(const QString & dirName) {
	bool result = false;
	QDir dir(dirName);

	if (dir.exists(dirName)) {
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
			if (info.isDir()) {
				result = removeDir(info.absoluteFilePath());
			}
			else {
				result = QFile::remove(info.absoluteFilePath());
				if (result) {
					++mDeletedFiles;
				}
			}
		}
		result = dir.rmdir(dirName);
	}
	return result;
}

bool Update::removePath(QString path) {
	QString correctedPath = QDir::toNativeSeparators(mCslFolderName + mSeparator + path).trimmed();
	//qDebug() << correctedPath;
	QFileInfo fileInfo(correctedPath);
	QDir dir(correctedPath);
	if (fileInfo.isFile()) {
		bool res = QFile::remove(correctedPath);
		if (res) {
			++mDeletedFiles;
		}
		return res;
	}
	else {
		return removeDir(correctedPath);
	}
}

bool Update::createDownloadingFile(PackageEntry inPackageEntry) {
	QString fileName = mCslFolderName + mSeparator + inPackageEntry.data[1];
	if (inPackageEntry.ID == 100) {// files for root recourses folder
		QDir dir(mCslFolderName);
		dir.cdUp();
		QString corrFolderName = dir.path();
		fileName = corrFolderName + mSeparator + inPackageEntry.data[1];
	}
	if (QFile::exists(fileName)) {
		QFile::remove(fileName);
	}
	delete mDownloadingFile;
	mDownloadingFile = new QFile(fileName);
	mDownloadingFile->open(QIODevice::WriteOnly);
	if (!mDownloadingFile->isOpen()) {// lets suppose that path to file is not exist
		int index = fileName.lastIndexOf(tr("/"));
		const QString dir_path = fileName.left(index);
		QDir dir("");
		dir.mkpath(dir_path);
		delete mDownloadingFile;
		mDownloadingFile = nullptr;
		mDownloadingFile = new QFile(fileName);
		mDownloadingFile->open(QIODevice::WriteOnly);
	}
	if (!mDownloadingFile->isOpen()) {
		SetMessage(tr("Ошибка: Не могу записать файл на ваш компьютер - %1 : %2.").arg(fileName).arg(mDownloadingFile->errorString()));
		delete mDownloadingFile;
		mDownloadingFile = nullptr;
		return false;
	}
	mDownloadingFileName = fileName;
	return true;
}

void Update::StartUpdate(QVector<PackageEntry> inFileList, Index *inIndexStep) {
	MWUI->CancelButton->setEnabled(true);
	QSettings settings(ORGANISATION, PROGRAM_NAME);
	mCslFolderName = settings.value("FolderName").toString();
	mEntryList.clear();
	mSelectedListForDelete.clear();
	mIndexStep = inIndexStep;
	mServer = settings.value("curServer").toString();
	int size = inFileList.size();
	int rowCount = MWUI->tableWidget->rowCount();
	// determine files to delete, we just will delete all files server wants.
	mSelectedListForDelete = mIndexStep->mFileListForDel;
	mDeletedFiles = 0;
	// determine files to update
	for (int it = 0; it < size; it++) {
		for (int i = 0; i < rowCount; i++) {
			if (MWUI->tableWidget->item(i, 0)->isSelected()) {
				if (MWUI->tableWidget->item(i, 6)->text() == "1") {
					if (inFileList[it].ID == MWUI->tableWidget->item(i, 0)->text().toInt()) {
						if (inFileList[it].state != 0) {
							mEntryList.push_back(inFileList[it]);
						}
					}
				}
			}
		}
	}
	InitProgBar(0, 1, 0, 1);
	// added task for download mtl.dat
	if (mCslFolderName.contains("X-IvAp Resources")) {
		PackageEntry entry;
		entry.ID = 100;// files for root recourses folder
		entry.data.append("100");
		entry.data.append("mtl.dat");
		entry.state = 1;
		mEntryList.push_front(entry);
	}	
	if (!mEntryList.empty()) {
		mFileCounter = 0;
		mFailedFileCounter = 0;
		CopyRemoteFile(mEntryList[mFileCounter]);
	}
	else {
		EndUpdate();
	}
}

void Update::EndUpdate() {
	// remove files was planed to delete	
	for (int i = 0; i < mSelectedListForDelete.size(); i++) {
		removePath(mSelectedListForDelete[i].data[1]);
	}
	if (mDeletedFiles > 0) {
		SetMessage(tr("Cleanup procedure done. Removed %1 files.").arg(mDeletedFiles));
	}

	if (mFailedFileCounter > 0) {
		SetMessage(tr("Обновление НЕ завершено! Не удалось обновить %1 файлов!").arg(mFailedFileCounter));
	}
	else {
		SetMessage(tr("Обновление завершено!"));
	}	
	MWUI->CancelButton->setEnabled(false);
	MWUI->PrevButton->setEnabled(false);
	MWUI->NextButton->setEnabled(false);
	mIndexStep->StartIndex();
}

void Update::CopyRemoteFile(PackageEntry inPackageEntry) {
	QString From = mServer + inPackageEntry.data[1];	
	QUrl url(From);
	if (!createDownloadingFile(inPackageEntry)) {// cannot create file
		++mFailedFileCounter;
		++mFileCounter;
		if (mFileCounter < mEntryList.size()) {			
			CopyRemoteFile(mEntryList[mFileCounter]);
		}
		else {
			EndUpdate();
		}
		return;
	}
	// start downloading
	mDownloadedBytes = 0;
	mTotalBytes = 0;
	QNetworkRequest request;
	request.setUrl(url);
	QNetworkReply *reply = mNetMng->get(request);
	connect(this, &Update::cancelDownloading, reply, &QNetworkReply::abort);
	connect(reply, &QNetworkReply::downloadProgress, this, &Update::updateDataReadProgress);
	SetMessage(tr("Обновляем: %1...").arg(mDownloadingFileName));
}

void Update::httpRequestFinished(QNetworkReply *inReply) {
	inReply->deleteLater();
	if (inReply->error() != QNetworkReply::OperationCanceledError) {
		if (inReply->error() == QNetworkReply::NoError) {
			mDownloadingFile->write(inReply->readAll());
			mDownloadingFile->close();
		}
		else {
			// error details
			QString errorUrl = inReply->request().url().toString();
			QString httpStatus = inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
			++mFailedFileCounter;
			mDownloadingFile->close();
			mDownloadingFile->remove();
			SetMessage(tr("Ошибка : %1.").arg(httpStatus + " - " + httpStatusMessage));
		}
		// start for next file
		delete mDownloadingFile;
		mDownloadingFile = nullptr;
		++mFileCounter;
		if (mFileCounter < mEntryList.size()) {
			CopyRemoteFile(mEntryList[mFileCounter]);
		}
		else {
			EndUpdate();
		}
	}
	else {
		// we canceled
		mDownloadingFile->remove();
		mDownloadingFile->close();
		delete mDownloadingFile;
		mDownloadingFile = nullptr;
	}
}

void Update::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes) {
	mTotalBytes = totalBytes;
	mDownloadedBytes = bytesRead;
	MWUI->progressBar->setMaximum(mTotalBytes);
	MWUI->progressBar->setValue(mDownloadedBytes);
}

