#include "UpdateStep.h"
#include <filesystem>

UpdateStep::UpdateStep(QWidget * _MW, Ui::MainWindow * _MWUI)
    : BaseSteps(_MW, _MWUI) {
    //-------------------------------------------------------------------------
    mAltDefs = mAltDefs->instance();
    mNetMng = new QNetworkAccessManager(this);
    connect(mNetMng, &QNetworkAccessManager::finished, this, &UpdateStep::httpRequestFinished);
}

UpdateStep::~UpdateStep() {
    delete mNetMng;
    delete mDownloadingFile;
}

void UpdateStep::CancelSlot() {
    setMessage(tr("The operation has been canceled by user!"));
    emit cancelDownloading();

    MWUI->cancelButton->setEnabled(false);
    MWUI->indexButton->setEnabled(false);
    MWUI->updateButton->setEnabled(false);
    mIndexStep->startIndex();
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
                qDebug() << "This file is about to be removed: <" << info.absoluteFilePath() << ">";
                result = QFile::remove(info.absoluteFilePath());
                if (result) {
                    ++mDeletedFiles;
                    qDebug() << "The file has been removed.";
                }
            }
        }
        qDebug() << "This directory is about to be removed: <" << dirName << ">";
        result = dir.rmdir(dirName);
        if (result){
            qDebug() << "The directory has been removed.";
        }
    }
    return result;
}

bool UpdateStep::removePath(PackageEntry inPackageEntry) {
    const QString path = mAltDefs->fullLocalPath(inPackageEntry.type, inPackageEntry.data[1]);
    QFileInfo fileInfo(path);
    QDir dir(path);
    if (fileInfo.isFile()) {
        qDebug() << "This file is about to be removed: <" << path << ">";
        bool res = QFile::remove(path);
        if (res) {
            ++mDeletedFiles;
            qDebug() << "The file has been removed.";
        }
        return res;
    }
    else {
        return removeDir(path);
    }
}

bool UpdateStep::createDownloadingFile(PackageEntry inPackageEntry) {
    const QString fileName = mAltDefs->fullLocalPath(inPackageEntry.type, inPackageEntry.data[1]);
    if (QFile::exists(fileName)) {
        const bool res = QFile::copy(fileName, fileName + ".backup");
        if (res){
            qDebug() << "A backup file has been created: <" << fileName + ".backup" << ">";
        }
    }
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }
    delete mDownloadingFile;
    mDownloadingFile = new QFile(fileName);
    mDownloadingFile->open(QIODevice::WriteOnly);
    if (!mDownloadingFile->isOpen()) {
        // lets suppose that path to file is not exist
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
        qWarning() << QString("Cannot write to file : <%1>; Reason: %2").arg(fileName).arg(mDownloadingFile->errorString());
        delete mDownloadingFile;
        mDownloadingFile = nullptr;
        return false;
    }
    mDownloadingFileName = fileName;
    return true;
}

void UpdateStep::StartUpdate(QVector<PackageEntry> inFileList, IndexStep * inIndexStep) {
    setMessage(tr("Updating, please wait..."));
    // auto selection for client additional files package
    if (!mAltDefs->isCustomSimDirSelected()){
        MWUI->tableWidget->item(0, 0)->setSelected(true);
    }

    connect(MWUI->cancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));
    MWUI->cancelButton->setEnabled(true);
    //
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    mEntryList.clear();
    mSelectedListForDelete.clear();
    mIndexStep = inIndexStep;
    int size = inFileList.size();
    int rowCount = MWUI->tableWidget->rowCount();
    // determine files to delete, we just will delete all files server wants.
    mSelectedListForDelete = mIndexStep->mFileListForDel;
    mDeletedFiles = 0;
    // determine files to update
    qDebug() << "Checking selected packages to determine which files are needed to be updated.";
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
    initProgBar(0, 1, 0, 1);
    if (!mEntryList.empty()) {
        qInfo() << "Staring updating files, number of files to update: " << mEntryList.size();
        mFileCounter = 0;
        mFailedFileCounter = 0;
        CopyRemoteFile(mEntryList[mFileCounter]);
    }
    else {
        qInfo() << "Updating files is skipped, no packages are selected.";
        EndUpdate();
    }
}

void UpdateStep::EndUpdate() {
    qInfo() << "Updating final stage has been entered.";
    qInfo() << "Cleaning up files and dirs...";
    // remove files was planed to delete
    for (int i = 0; i < mSelectedListForDelete.size(); i++) {
        removePath(mSelectedListForDelete[i]);
    }
    qInfo() << "Cleanup procedure is done. Number of removed files: " << mDeletedFiles;

    MWUI->cancelButton->setEnabled(false);
    disconnect(MWUI->cancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));

    if (mFailedFileCounter > 0) {
        setMessage(tr("Error: Cannot get updating successfully done! The X-CSL library can be broken! See log file for details."));
        setMessage(tr("Reindexing is required to continue. Please click \"Index\" button."));
        qWarning() << "Updating is failed. Number of failed files: " << mFailedFileCounter;
        MWUI->indexButton->setEnabled(true);
        MWUI->updateButton->setEnabled(false);
    }
    else {
        setMessage(tr("Updating process is successfully done!"));
        qInfo() << "Updating is done.";
        MWUI->indexButton->setEnabled(false);
        MWUI->updateButton->setEnabled(false);
        mIndexStep->startIndex();
    }
}

void UpdateStep::CopyRemoteFile(PackageEntry inPackageEntry) {
    const QUrl url(mAltDefs->fullUrl(inPackageEntry.type, inPackageEntry.data[1]));
    if (!createDownloadingFile(inPackageEntry)) {
        // cannot create file
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
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &UpdateStep::cancelDownloading, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &UpdateStep::updateDataReadProgress);
    setMessage(tr("Updating: %1...").arg(mDownloadingFileName));
}

void UpdateStep::httpRequestFinished(QNetworkReply * inReply) {
    inReply->deleteLater();
    if (inReply->error() != QNetworkReply::OperationCanceledError) {
        if (inReply->error() == QNetworkReply::NoError) {
            mDownloadingFile->write(inReply->readAll());
            mDownloadingFile->close();
            qDebug() << "The downloaded file has been put here: " << mDownloadingFile->fileName();
            // remove backup file
            const QString fileName = mDownloadingFile->fileName() + ".backup";
            if (QFile::exists(fileName)) {
                const bool res = QFile::remove(fileName);
                if (res) {
                    qDebug() << "The backup file has been removed: <" << fileName << ">";
                }
            }
        }
        else {
            // error details
            const QString errorUrl = inReply->request().url().toString();
            const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
            ++mFailedFileCounter;
            mDownloadingFile->close();
            mDownloadingFile->remove();
            if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
                QString msg = QString("Cannot download a file <%1> due to: %2").arg(errorUrl).arg(inReply->errorString());
                setMessage(tr(msg.toStdString().c_str()));
                qWarning() << msg;
            }
            else {
                QString msg = QString("Cannot download a file <%1> due to: %2 - %3").arg(errorUrl).arg(httpStatus).arg(httpStatusMessage);
                setMessage(tr(msg.toStdString().c_str()));
                qWarning() << msg;
            }
            // revert file from a backup
            const bool res = QFile::copy(mDownloadingFile->fileName() + ".backup", mDownloadingFile->fileName());
            if (res){
                qDebug() << "The file has been restored from the backup file : <" << mDownloadingFile->fileName() + ".backup" << ">";
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
