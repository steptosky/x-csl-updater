#include "UpdateStep.h"

UpdateStep::UpdateStep(QWidget *_MW, Ui::MainWindow *_MWUI, 
	const QString & targetDir, const QString & targetCslDir)
: BaseSteps(_MW, _MWUI,targetDir, targetCslDir) {

	mNetMng = new QNetworkAccessManager(this);
	connect(mNetMng, &QNetworkAccessManager::finished, this, &UpdateStep::httpRequestFinished);
	connect(MWUI->cancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));

}

UpdateStep::~UpdateStep() {
	delete mNetMng;
	delete mDownloadingFile;
}

void UpdateStep::CancelSlot() {
	SetMessage(tr("The operation has been canceled by user!"));
	emit cancelDownloading();

	MWUI->cancelButton->setEnabled(false);
	MWUI->indexButton->setEnabled(false);
	MWUI->updateButton->setEnabled(false);
	mIndexStep->StartIndex();
}

bool UpdateStep::removeDir(const QString & dirName) {
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

bool UpdateStep::removePath(QString path) {
	QString correctedPath = QDir::toNativeSeparators(mTargetCslDir + "/" + path).trimmed();
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

bool UpdateStep::createDownloadingFile(PackageEntry inPackageEntry) {
	QString fileName = mTargetCslDir + "/" + inPackageEntry.data[1];
	if (inPackageEntry.ID == 100) {// files for root recourses folder
		QDir dir(mTargetCslDir);
		dir.cdUp();
		const QString corrFolderName = dir.path();
		fileName = corrFolderName + "/" + inPackageEntry.data[1];
	}
    if (QFile::exists(fileName) && inPackageEntry.ID == 100) {
        QFile::copy(fileName, fileName + ".backup");
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
        SetMessage(tr("Error: Cannot write file: <%1>; Reason: %2").arg(fileName).arg(mDownloadingFile->errorString()));
		delete mDownloadingFile;
		mDownloadingFile = nullptr;
		return false;
	}
	mDownloadingFileName = fileName;
	return true;
}

void UpdateStep::StartUpdate(QVector<PackageEntry> inFileList, IndexStep *inIndexStep) {
	MWUI->cancelButton->setEnabled(true);
	QSettings settings(gSettingsFileName, QSettings::IniFormat);
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
                const ePackageState packageState = static_cast<ePackageState>(MWUI->tableWidget->item(i, 6)->text().toInt());
				if (packageState == CLIENT_PACKAGE_STATUS_CHANGE || packageState == CLIENT_PACKAGE_STATUS_LOST) {
					if (inFileList[it].ID == MWUI->tableWidget->item(i, 0)->text().toInt()) {
						if (inFileList[it].state == CLIENT_FILE_STATUS_CHANGE || inFileList[it].state == CLIENT_FILE_STATUS_LOST) {
							mEntryList.push_back(inFileList[it]);
						}
					}
				}
			}
		}
	}
	InitProgBar(0, 1, 0, 1);
	// added task for download several resource files
	if (mTargetCslDir.contains("X-IvAp Resources")) {
		PackageEntry entry;
        // mtl.dat
		entry.ID = 100;// files for root recourses folder
        entry.data.clear();
		entry.data.append("100");
		entry.data.append("mtl.dat");
		entry.state = CLIENT_FILE_STATUS_CHANGE;
		mEntryList.push_front(entry);
        // Doc8643.txt
        entry.ID = 100;// files for root recourses folder
        entry.data.clear();
        entry.data.append("100");
        entry.data.append("Doc8643.txt");
        entry.state = CLIENT_FILE_STATUS_CHANGE;
        mEntryList.push_front(entry);
        // related.txt
        entry.ID = 100;// files for root recourses folder
        entry.data.clear();
        entry.data.append("100");
        entry.data.append("related.txt");
        entry.state = CLIENT_FILE_STATUS_CHANGE;
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

void UpdateStep::EndUpdate() {
	// remove files was planed to delete	
	for (int i = 0; i < mSelectedListForDelete.size(); i++) {
		removePath(mSelectedListForDelete[i].data[1]);
	}
	if (mDeletedFiles > 0) {
		SetMessage(tr("Cleanup procedure done. Removed %1 files.").arg(mDeletedFiles));
	}

	if (mFailedFileCounter > 0) {
        SetMessage(tr("Warning: Updating process is NOT done! %1 files have NOT been updated for some reasons.").arg(mFailedFileCounter));
	}
	else {
		SetMessage(tr("Updating process is successfully done!"));
	}	
	MWUI->cancelButton->setEnabled(false);
	MWUI->indexButton->setEnabled(false);
	MWUI->updateButton->setEnabled(false);
	mIndexStep->StartIndex();
}

void UpdateStep::CopyRemoteFile(PackageEntry inPackageEntry) {
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
	connect(this, &UpdateStep::cancelDownloading, reply, &QNetworkReply::abort);
	connect(reply, &QNetworkReply::downloadProgress, this, &UpdateStep::updateDataReadProgress);
	SetMessage(tr("Updating: %1...").arg(mDownloadingFileName));
}

void UpdateStep::httpRequestFinished(QNetworkReply *inReply) {
	inReply->deleteLater();
	if (inReply->error() != QNetworkReply::OperationCanceledError) {
		if (inReply->error() == QNetworkReply::NoError) {
			mDownloadingFile->write(inReply->readAll());
			mDownloadingFile->close();
            // remove backup file if it's a root resource file ID==100
            if(mEntryList[mFileCounter].ID == 100) {
                QDir dir(mTargetCslDir);
                dir.cdUp();
                const QString corrFolderName = dir.path();
                const QString fileName = corrFolderName + "/" + mEntryList[mFileCounter].data[1] + ".backup";
                if (QFile::exists(fileName)) {
                    QFile::remove(fileName);
                }
            }
		}
		else {
			// error details
			QString errorUrl = inReply->request().url().toString();
			QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
			QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
			++mFailedFileCounter;
			mDownloadingFile->close();
			mDownloadingFile->remove();
			SetMessage(tr("Error : %1.").arg(httpStatus + " - " + httpStatusMessage));
            // revert file if it's a root resource file ID==100
            if(mEntryList[mFileCounter].ID == 100) {
                QDir dir(mTargetCslDir);
                dir.cdUp();
                const QString corrFolderName = dir.path();
                const QString fileName = corrFolderName + "/" + mEntryList[mFileCounter].data[1];
                QFile::copy(fileName + ".backup", fileName);
            }
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

void UpdateStep::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes) {
	mTotalBytes = totalBytes;
	mDownloadedBytes = bytesRead;
	MWUI->progressBar->setMaximum(mTotalBytes);
	MWUI->progressBar->setValue(mDownloadedBytes);
}

