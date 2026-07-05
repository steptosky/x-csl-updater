// SPDX-License-Identifier: MPL-2.0

#include "UpdateStep.h"

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
    const QString fileUri = inPackageEntry.data.value(1);
    const QString path = mAltDefs->fullLocalPath(inPackageEntry.type, fileUri);
    if (path.isEmpty()) {
        setMessage(tr("Error: Unsafe or invalid path in delete index for URI <%1>, entry ID <%2>. Skipped.").arg(fileUri).arg(inPackageEntry.ID));
        return false;
    }
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
    const QString fileUri = inPackageEntry.data.value(1);
    const QString fileName = mAltDefs->fullLocalPath(inPackageEntry.type, fileUri);
    if (fileName.isEmpty()) {
        setMessage(tr("Error: Unsafe or invalid path in package index for URI <%1>, package ID <%2>. Skipped.").arg(fileUri).arg(inPackageEntry.ID));
        return false;
    }

    const QFileInfo fileInfo(fileName);
    const QString dirPath = fileInfo.dir().path();
    QDir dir;
    if (!dir.mkpath(dirPath)) {
        qWarning() << QString("Cannot create the path: <%1>").arg(dirPath);
        return false;
    }

    const QString downloadingFileName = fileName + ".download";
    if (QFile::exists(downloadingFileName) && !QFile::remove(downloadingFileName)) {
        qWarning() << QString("Cannot remove stale downloading file: <%1>").arg(downloadingFileName);
        return false;
    }

    clearDownloadingFileState();
    mDownloadingFile = new QFile(downloadingFileName);
    mDownloadingFile->open(QIODevice::WriteOnly);
    if (!mDownloadingFile->isOpen()) {
        qWarning() << QString("Cannot write to file : <%1>; Reason: %2").arg(downloadingFileName).arg(mDownloadingFile->errorString());
        clearDownloadingFileState();
        return false;
    }
    mDownloadingFileName = fileName;
    mDownloadingTargetFileName = fileName;
    return true;
}

bool UpdateStep::installDownloadedFile() {
    if (mDownloadingFile == nullptr || mDownloadingTargetFileName.isEmpty()) {
        qWarning() << "Cannot install downloaded file, reason: downloading file state is not valid.";
        return false;
    }

    if (mDownloadingFile->isOpen()) {
        mDownloadingFile->close();
    }

    const QString downloadingFileName = mDownloadingFile->fileName();
    const QString backupFileName = mDownloadingTargetFileName + ".backup";

    if (!QFile::exists(downloadingFileName)) {
        qWarning() << QString("Cannot install downloaded file, temporary file is not found: <%1>").arg(downloadingFileName);
        return false;
    }

    if (QFile::exists(backupFileName) && !QFile::remove(backupFileName)) {
        qWarning() << QString("Cannot remove old backup file: <%1>").arg(backupFileName);
        return false;
    }

    const bool hasOriginalFile = QFile::exists(mDownloadingTargetFileName);
    if (hasOriginalFile && !QFile::rename(mDownloadingTargetFileName, backupFileName)) {
        qWarning() << QString("Cannot create backup file: <%1>").arg(backupFileName);
        return false;
    }
    if (hasOriginalFile) {
        qDebug() << "A backup file has been created: <" << backupFileName << ">";
    }

    if (!QFile::rename(downloadingFileName, mDownloadingTargetFileName)) {
        qWarning() << QString("Cannot move downloaded file <%1> to target path <%2>").arg(downloadingFileName).arg(mDownloadingTargetFileName);
        if (hasOriginalFile && !QFile::rename(backupFileName, mDownloadingTargetFileName)) {
            qWarning() << QString("Cannot restore file from backup: <%1>").arg(backupFileName);
        }
        return false;
    }

    qDebug() << "The downloaded file has been put here: " << mDownloadingTargetFileName;

    if (hasOriginalFile && QFile::exists(backupFileName)) {
        const bool res = QFile::remove(backupFileName);
        if (res) {
            qDebug() << "The backup file has been removed: <" << backupFileName << ">";
        }
        else {
            qWarning() << QString("Cannot remove backup file: <%1>").arg(backupFileName);
        }
    }

    return true;
}

void UpdateStep::clearDownloadingFileState() {
    if (mDownloadingFile != nullptr) {
        if (mDownloadingFile->isOpen()) {
            mDownloadingFile->close();
        }
        delete mDownloadingFile;
        mDownloadingFile = nullptr;
    }
    mDownloadingFileName.clear();
    mDownloadingTargetFileName.clear();
}

void UpdateStep::removeDownloadingFile() {
    const QString downloadingFileName = mDownloadingFile != nullptr ? mDownloadingFile->fileName() : QString();
    clearDownloadingFileState();
    if (!downloadingFileName.isEmpty() && QFile::exists(downloadingFileName) && !QFile::remove(downloadingFileName)) {
        qWarning() << QString("Cannot remove downloading file: <%1>").arg(downloadingFileName);
    }
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
    QSettings settings(settingsFileName(), QSettings::IniFormat);
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
    const QUrl url(mAltDefs->fullUrl(inPackageEntry.type, inPackageEntry.data.value(1)));
    // start downloading
    mDownloadedBytes = 0;
    mTotalBytes = 0;
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, networkUserAgent());
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &UpdateStep::cancelDownloading, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &UpdateStep::updateDataReadProgress);
    setMessage(tr("Updating: %1...").arg(mDownloadingFileName));
}

void UpdateStep::httpRequestFinished(QNetworkReply * inReply) {
    inReply->deleteLater();
    if (inReply->error() != QNetworkReply::OperationCanceledError) {
        if (inReply->error() == QNetworkReply::NoError) {
            const QByteArray downloadedData = inReply->readAll();
            if (mDownloadingFile == nullptr) {
                ++mFailedFileCounter;
                qWarning() << "Cannot save downloaded file, reason: downloading file state is not valid.";
            }
            else {
                const qint64 bytesWritten = mDownloadingFile->write(downloadedData);
                if (bytesWritten != downloadedData.size()) {
                    ++mFailedFileCounter;
                    qWarning() << QString("Cannot write downloaded file <%1>; Reason: %2").arg(mDownloadingFile->fileName()).arg(mDownloadingFile->errorString());
                    removeDownloadingFile();
                }
                else if (!mDownloadingFile->flush()) {
                    ++mFailedFileCounter;
                    qWarning() << QString("Cannot flush downloaded file <%1>; Reason: %2").arg(mDownloadingFile->fileName()).arg(mDownloadingFile->errorString());
                    removeDownloadingFile();
                }
                else if (!installDownloadedFile()) {
                    ++mFailedFileCounter;
                    removeDownloadingFile();
                }
            }
        }
        else {
            // error details
            const QString errorUrl = inReply->request().url().toString();
            const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
            const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
            ++mFailedFileCounter;
            removeDownloadingFile();
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
        }
        // start for next file
        clearDownloadingFileState();
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
        removeDownloadingFile();
    }
}

void UpdateStep::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes) {
    mTotalBytes = totalBytes;
    mDownloadedBytes = bytesRead;
    MWUI->progressBar->setMaximum(mTotalBytes);
    MWUI->progressBar->setValue(mDownloadedBytes);
}
