// SPDX-License-Identifier: MPL-2.0

#include "IndexStep.h"
#include <QDebug>
#include "AltitudeDefs.h"

IndexStep::IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf)
    : BaseSteps(_MW, _MWUI), mLocale(this->locale())
{
    //-------------------------------------------------------------------------
    mAltDefs = mAltDefs->instance();
    mPackInfo = _Inf;
    mNetMng = new QNetworkAccessManager(this);
}

IndexStep::~IndexStep() {
    delete mNetMng;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool IndexStep::createTargetFile(const QString & fileName, const QByteArray & bytesToWrite) const {
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }
    const QDir dir;
    if (!dir.exists(QFileInfo(fileName).dir().path())) {
        if (!dir.mkpath(QFileInfo(fileName).dir().path())) {
            qWarning() << "Cannot create the path: <" << fileName << ">";
            return false;
        }
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot write to file: <" << fileName << ">";
        qWarning() << "Reason: " << file.errorString();
        return false;
    }
    file.write(bytesToWrite);
    file.close();
    qDebug() << "The downloaded file has been put here: " << fileName;
    return true;
}

void IndexStep::scheduleDownloadingFile(const QString & url, const QString & localPath) {
    qDebug() << "Will download a file from: <" << url << ">";
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, networkUserAgent());
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(localPath));
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &IndexStep::abortAllReplaysSig, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &IndexStep::updateDownloadProgress);
}

void IndexStep::resetDownloadProgress() {
    mDownloadBytesReceived.clear();
    mDownloadBytesTotal.clear();
    initProgBar(0, 1, 0, 1);
}

void IndexStep::updateDownloadProgressBar() const {
    qint64 totalBytesReceived = 0;
    qint64 totalBytes = 0;
    for (auto it = mDownloadBytesReceived.constBegin(); it != mDownloadBytesReceived.constEnd(); ++it) {
        totalBytesReceived += it.value();
        totalBytes += qMax(mDownloadBytesTotal.value(it.key(), 0), it.value());
    }

    if (totalBytes <= 0) {
        totalBytes = 1;
    }
    MWUI->progressBar->setMaximum(static_cast<int>(totalBytes));
    MWUI->progressBar->setValue(static_cast<int>(totalBytesReceived));
}

/**************************************************************************************************/
//////////////////////////////////////////* CSL Indexing */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::startIndex() {
    setMessage(tr("Indexing, please wait..."));
    resetIndex();
    mIndexFinished = false;
    mCancelRequested = false;

    MWUI->indexButton->setEnabled(false);
    MWUI->updateButton->setEnabled(false);

    const QSettings settings(settingsFileName(), QSettings::IniFormat);
    mAltDefs->setServerUrl(settings.value("curServer").toString());

    connect(MWUI->cancelButton, &QPushButton::pressed, this, &IndexStep::cancelSlot);
    MWUI->cancelButton->setEnabled(true);

    // stage 1
    emit abortAllReplaysSig();
    resetDownloadProgress();
    setMessage(tr("Downloading configuration file..."));
    scheduleDownloadingFile(mAltDefs->configFileUrl(), mAltDefs->configFileLocalPath());
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage2Slot, Qt::UniqueConnection);
}

void IndexStep::resetIndex() {
    MWUI->indexButton->setEnabled(false);
    MWUI->updateButton->setEnabled(false);
    mPackInfo->requestStopAction();
    mSizeOfClient = 0;
    mSizeOfNeedUpdate = 0;
    mSizeOfServer = 0;
    mEntryList.clear();
    mFileListForDel.clear();
    initProgBar(0, 10, 0, 1);
    emit abortAllReplaysSig();
    MWUI->tableWidget->clearContents();
    MWUI->tableWidget->setRowCount(0);
    MWUI->indexButton->setEnabled(true);
    qDebug() << "Index data has been reset.";
}

void IndexStep::stage2() {
    qInfo() << "Indexing stage 2 has been entered.";
    if (!mAltDefs->getAllPathsAndUrlsReady()) {
        endIndex(false);
        return;
    }
    //
    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage2Slot);
    emit abortAllReplaysSig();
    // stage 2
    resetDownloadProgress();
    setMessage(tr("Downloading index files..."));
    mFilesToDownload = 4;
    scheduleDownloadingFile(mAltDefs->indexFileUrl(), mAltDefs->indexFileLocalPath());
    scheduleDownloadingFile(mAltDefs->indexForDelFileUrl(), mAltDefs->indexForDelFileLocalPath());
    scheduleDownloadingFile(mAltDefs->cslIndexFileUrl(), mAltDefs->cslIndexFileLocalPath());
    scheduleDownloadingFile(mAltDefs->cslIndexForDelFileUrl(), mAltDefs->cslIndexForDelFileLocalPath());
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot, Qt::UniqueConnection);
}

void IndexStep::stage3() {
    qInfo() << "Indexing stage 3 has been entered.";

    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot);
    emit abortAllReplaysSig();
    // stage 3
    mDownloadBytesReceived.clear();
    mDownloadBytesTotal.clear();
    initProgBar(0, 4, 0, 1);
    setMessage(tr("Reading index files..."));
    parseIndexFiles();
}

void IndexStep::endIndex(int Next) {
    if (mIndexFinished) {
        return;
    }
    mIndexFinished = true;
    qInfo() << "Indexing final stage has been entered.";
    if (Next) {
        setMessage(tr("Indexing local files is successfully done."));
        mSizeOfNeedUpdate = mSizeOfServer - mSizeOfClient;
        const QString strSizeNeed = mLocale.formattedDataSize(mSizeOfNeedUpdate);
        const QString strSizeAll = mLocale.formattedDataSize(mSizeOfServer);
        //1048576
        if (mSizeOfNeedUpdate != 0) {
            setMessage(tr("To make all the packages up-to-date you have to download %1 of %2").arg(strSizeNeed, strSizeAll));
            setMessage(tr("Select the packages you want to update and click \"Update\"."));
        }
        else {
            setMessage(tr("Congratulations! All the packages are fully up-to-date."));
        }
        mPackInfo->GetInfoToTable();
        MWUI->tableWidget->scrollToTop();
        MWUI->updateButton->setEnabled(true);
        MWUI->indexButton->setEnabled(true);
        qInfo() << "Indexing is done.";
    }
    else {
        if (mCancelRequested) {
            setMessage(tr("Indexing process has been canceled by user!"));
        }
        setMessage(tr("Error: Cannot get indexing successfully done! See log file for details."));
        MWUI->indexButton->setEnabled(true);
        qWarning() << "Indexing is failed. See details above.";
    }
    //
    emit abortAllReplaysSig();
    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage2Slot);
    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot);
    disconnect(MWUI->cancelButton, &QPushButton::pressed, this, &IndexStep::cancelSlot);
    MWUI->cancelButton->setDisabled(true);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::addPackageToTable(const QStringList & list, int row) const {
    Q_ASSERT(list.size() > 7);
    const int rowCount = MWUI->tableWidget->rowCount();
    if (row < 0 || row > rowCount) {
        row = rowCount;
    }
    MWUI->tableWidget->insertRow(row);
    MWUI->tableWidget->setItem(row, 0, new QTableWidgetItem(list[7]));
    MWUI->tableWidget->setItem(row, 1, new QTableWidgetItem(list[1]));
    MWUI->tableWidget->setItem(row, 2, new QTableWidgetItem(tr("Please wait...")));
    MWUI->tableWidget->setItem(row, 3, new QTableWidgetItem(QString("%3 (%4)").arg(list[4], list[5])));
    const QString sizeStr = mLocale.formattedDataSize(list[2].toDouble());
    auto * sizeItem = new QTableWidgetItem(sizeStr);
    sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    MWUI->tableWidget->setItem(row, 4, sizeItem);
    addPackageStatusToTable(row, static_cast<ePackageState>(list[6].toInt()));

    qDebug() << QString("A package <%1> has been added at %2th row.").arg(list[1]).arg(row);
}

void IndexStep::addPackageStatusToTable(int count, ePackageState status) const {
    auto * statusTextItem = new QTableWidgetItem();
    auto * statusItem = new QTableWidgetItem(QString::number(status));
    statusTextItem->setText(tr(packageState2Text(status)));
    statusTextItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    MWUI->tableWidget->setItem(count, 5, statusTextItem);
    MWUI->tableWidget->setItem(count, 6, statusItem);
    switch (status) {
        case CLIENT_PACKAGE_STATUS_OK:
            statusTextItem->setForeground(QBrush(Qt::darkGreen));
            break;
        case CLIENT_PACKAGE_STATUS_CHANGE:
            statusTextItem->setForeground(QBrush(Qt::red));
            break;
        case CLIENT_PACKAGE_STATUS_LOST:
            statusTextItem->setForeground(QBrush(Qt::darkRed));
            break;
        default:
            statusTextItem->setForeground(QBrush(Qt::darkGray));
            break;
    }
    qDebug() << QString("The status of the package at %1th row has ben set to: <%2>").arg(count).arg(packageState2Text(status));
}

int IndexStep::findPackageInsertRow(int firstRow, const QString & packageName) const {
    for (int row = firstRow; row < MWUI->tableWidget->rowCount(); ++row) {
        const auto * item = MWUI->tableWidget->item(row, 1);
        if (item != nullptr && QString::localeAwareCompare(packageName, item->text()) < 0) {
            return row;
        }
    }
    return MWUI->tableWidget->rowCount();
}

bool IndexStep::countFilesInIndexFile(int & filesTotal, const QString & indexFileName) const {
    qDebug() << "Counting files in an index file: <" << indexFileName << ">";
    QFile file(indexFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open the index file, reason: " << file.errorString();
        return false;
    }
    if (file.size() < 1) {
        qWarning() << "Cannot parse the index file, reason: The index file has zero size!";
        return false;
    }
    int isFirstLine = true;
    while (!file.atEnd()) {
        QString line = QString(file.readLine()).trimmed();
        QString type = line.left(1);
        if (isFirstLine) {
            if (type != "0") {
                qWarning() << "Cannot parse the index file, reason: The index file has wrong format!";
                file.close();
                return false;
            }
            isFirstLine = false;
        }
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", Qt::SkipEmptyParts);
        if (list.size() >= 4 && list[0] == "10") {
            filesTotal++;
        }
    }
    file.close();
    qDebug() << "Files have been counted in the index file.";
    return true;
}

bool IndexStep::parseIndexFile(int & packagesCount, const QString & indexFileName, bool isCslIndex) {
    qDebug() << "Parsing an index file: <" << indexFileName << ">";
    QFile file(indexFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open the index file, reason: " << file.errorString();
        return false;
    }
    if (file.size() < 1) {
        qWarning() << "Cannot parse the index file, reason: The index file has zero size!";
        return false;
    }
    const int firstRow = MWUI->tableWidget->rowCount();
    int isFirstLine = true;
    while (!file.atEnd()) {
        QString line = QString(file.readLine()).trimmed();
        QString type = line.left(1);
        if (isFirstLine) {
            if (type != "0") {
                qWarning() << "Cannot parse the index file, reason: The index file has wrong format!";

                file.close();
                return false;
            }
            isFirstLine = false;
        }
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", Qt::SkipEmptyParts);
        if (list.size() >= 6 && list[0] == "11") {
            const ePackageState status = checkCslPack(file.pos(), packagesCount, indexFileName, isCslIndex);
            if (mCancelRequested) {
                file.close();
                return false;
            }
            const int packageId = packagesCount;
            const QString packageLine = line + "%" + QString::number(status) + "%" + QString::number(packageId);
            QStringList packageList = packageLine.split("%", Qt::SkipEmptyParts);
            const int sortStartRow = !isCslIndex && packageId == 0 ? firstRow : firstRow + (!isCslIndex && firstRow == 0 ? 1 : 0);
            const int insertRow = !isCslIndex && packageId == 0 ? firstRow : findPackageInsertRow(sortStartRow, packageList[1]);
            addPackageToTable(packageList, insertRow);
            packagesCount++;
            QApplication::processEvents();
            if (mCancelRequested) {
                file.close();
                return false;
            }
        }
    }
    file.close();
    qDebug() << "The index file has been parsed.";
    return true;
}

bool IndexStep::parseIndexForDelFile(const QString & indexFileName, bool isCslIndex) {
    qDebug() << "Parsing an index for deleting file: <" << indexFileName << ">";
    QFile fileForDel(indexFileName);
    if (!fileForDel.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open the index for deleting file, reason: " << fileForDel.errorString();
        return false;
    }
    while (!fileForDel.atEnd()) {
        QString line = fileForDel.readLine();
        QString type = line.left(1);
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", Qt::SkipEmptyParts);
        if (list.size() >= 2) {
            PackageEntry fileInfo;
            fileInfo.ID = list[0].toInt();
            fileInfo.data = list;
            fileInfo.state = CLIENT_FILE_STATUS_NONE;
            if (isCslIndex) {
                fileInfo.type = CSL_PACK_FILE;
            }
            else {
                fileInfo.type = ADDITIONAL_FILE;
            }
            mFileListForDel.push_back(fileInfo);
        }
    }
    fileForDel.close();
    qDebug() << "The index for deleting file has been parsed.";
    return true;
}

void IndexStep::parseIndexFiles() {
    qDebug() << "Parsing index for delete files...";
    QString fileForDelPath = mAltDefs->indexForDelFileLocalPath();
    if (!mAltDefs->isCustomSimDirSelected() && !parseIndexForDelFile(fileForDelPath, false)) {
        endIndex(false);
        return;
    }
    stepProgBar();
    fileForDelPath = mAltDefs->cslIndexForDelFileLocalPath();
    if (!parseIndexForDelFile(fileForDelPath, true)) {
        endIndex(false);
        return;
    }
    stepProgBar();

    // packs
    qDebug() << "Parsing index files...";
    int packagesCount = 0;
    MWUI->tableWidget->clearContents();
    MWUI->tableWidget->setRowCount(0);

    int filesTotal = 0;
    QString indexFilePath = mAltDefs->indexFileLocalPath();
    if (!mAltDefs->isCustomSimDirSelected() && !countFilesInIndexFile(filesTotal, indexFilePath)) {
        endIndex(false);
        return;
    }
    indexFilePath = mAltDefs->cslIndexFileLocalPath();
    if (!countFilesInIndexFile(filesTotal, indexFilePath)) {
        endIndex(false);
        return;
    }

    setMessage(tr("Checking local packages..."));
    initProgBar(0, qMax(1, filesTotal), 0, 1);

    // altitude pack
    indexFilePath = mAltDefs->indexFileLocalPath();
    if (!mAltDefs->isCustomSimDirSelected() && !parseIndexFile(packagesCount, indexFilePath, false)) {
        endIndex(false);
        return;
    }
    // csl pack
    indexFilePath = mAltDefs->cslIndexFileLocalPath();
    if (!parseIndexFile(packagesCount, indexFilePath, true)) {
        endIndex(false);
        return;
    }
    //MWUI->tableWidget->sortItems(0);
    endIndex();
}

ePackageState IndexStep::checkCslPack(int pos, int ID, const QString & indexFileName, bool isCslIndex) {
    QFile file(indexFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open the index file, reason: " << file.errorString();
        return CLIENT_PACKAGE_STATUS_LOST;
    }
    QTextStream in(&file);
    in.seek(pos);
    bool wereOkFiles = false;
    bool wereLostFiles = false;
    bool wereChangedFiles = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QString type = line.left(1);
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", Qt::SkipEmptyParts);
        if (list[0] == "11")
            break;
        if (list[0] == "10") {
            int st = checkFile(list, ID, isCslIndex);
            stepProgBar();
            QApplication::processEvents();
            if (mCancelRequested) {
                file.close();
                return CLIENT_PACKAGE_STATUS_NONE;
            }
            if (st == CLIENT_FILE_STATUS_OK) {
                wereOkFiles = true;
            }
            else if (st == CLIENT_FILE_STATUS_CHANGE) {
                wereChangedFiles = true;
            }
            else if (st == CLIENT_FILE_STATUS_LOST) {
                wereLostFiles = true;
            }
        }
    }
    file.close();
    ePackageState status = CLIENT_PACKAGE_STATUS_LOST;
    if (wereOkFiles && !wereLostFiles && !wereChangedFiles) {
        status = CLIENT_PACKAGE_STATUS_OK;
    }
    else if (wereLostFiles && !wereOkFiles && !wereChangedFiles) {
        status = CLIENT_PACKAGE_STATUS_LOST;
    }
    else if (wereChangedFiles || wereOkFiles || wereLostFiles) {
        status = CLIENT_PACKAGE_STATUS_CHANGE;
    }
    else {
        status = CLIENT_PACKAGE_STATUS_NONE;
    }
    return status;
}

eFileState IndexStep::checkFile(QStringList List, int ID, bool isCslIndex) {
    PackageEntry fileEntry;
    fileEntry.ID = ID;
    fileEntry.data = List;
    if (isCslIndex) {
        fileEntry.type = CSL_PACK_FILE;
    }
    else {
        fileEntry.type = ADDITIONAL_FILE;
    }
    const QString filePath = mAltDefs->fullLocalPath(fileEntry.type, List[1]);
    if (filePath.isEmpty()) {
        setMessage(tr("Error: Unsafe or invalid path in package index for URI <%1>, package ID <%2>.").arg(List.value(1)).arg(ID));
        fileEntry.state = CLIENT_FILE_STATUS_CHANGE;
        mEntryList.push_back(fileEntry);
        qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_CHANGE);
        return CLIENT_FILE_STATUS_CHANGE;
    }
    qDebug() << "Checking local file: <" << filePath << ">";
    const QFileInfo fileInfo(filePath);
    mSizeOfServer += List[2].toInt();
    // does file exist?
    if (!fileInfo.isFile()) {
        fileEntry.state = CLIENT_FILE_STATUS_LOST;
        mEntryList.push_back(fileEntry);
        qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_LOST);
        return CLIENT_FILE_STATUS_LOST;
    }
    const int size = List[2].toInt();
    if (size != static_cast<int>(fileInfo.size())) {
        fileEntry.state = CLIENT_FILE_STATUS_CHANGE;
        mEntryList.push_back(fileEntry);
        qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_CHANGE);
        return CLIENT_FILE_STATUS_CHANGE;
    }
    // check hash if it is available
    if (List[3] != "Reserve") {
        static QCryptographicHash hash(QCryptographicHash::Md5);
        static QFile file;
        hash.reset();
        file.close();
        file.setFileName(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            hash.addData(file.readAll());
            // 			qDebug() << List[1] << " - " << FilePath;
            // 			qDebug() << List[3] << " - " << QString(hash.result().toHex());
            if (List[3] != QString(hash.result().toHex())) {
                fileEntry.state = CLIENT_FILE_STATUS_CHANGE;
                mEntryList.push_back(fileEntry);
                qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_CHANGE);
                return CLIENT_FILE_STATUS_CHANGE;
            }
            file.close();
        }
        else {
            fileEntry.state = CLIENT_FILE_STATUS_LOST;
            mEntryList.push_back(fileEntry);
            qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_LOST);
            return CLIENT_FILE_STATUS_LOST;
        }
        QApplication::processEvents();
    }

    // dtae & time checking
    /*QString date_end_time;
    date_end_time = file.lastModified().toString("dd.MM.yyyy hh:mm:ss");
    if (date_end_time != List[4]+" "+List[5])
    {
    FilesInfo.State = _CLIENT_FILE_STATUS_CHANGE;
    FilesList.push_back(FilesInfo);
    return _CLIENT_FILE_STATUS_CHANGE
    }*/

    // file is ОК
    mSizeOfClient += static_cast<int>(fileInfo.size());
    fileEntry.state = CLIENT_FILE_STATUS_OK;
    mEntryList.push_back(fileEntry);
    qDebug() << "The file state has been set to: " << localFileState2Text(CLIENT_FILE_STATUS_OK);
    return CLIENT_FILE_STATUS_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Slots */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    auto * reply = qobject_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }
    mDownloadBytesReceived[reply] = bytesReceived;
    if (bytesTotal > 0) {
        mDownloadBytesTotal[reply] = bytesTotal;
    }
    else {
        mDownloadBytesTotal[reply] = qMax(mDownloadBytesTotal.value(reply, 0), bytesReceived);
    }
    updateDownloadProgressBar();
}

void IndexStep::cancelSlot() {
    mCancelRequested = true;
    MWUI->cancelButton->setDisabled(true);
    emit abortAllReplaysSig();
}

void IndexStep::stage2Slot(QNetworkReply * inReply) {
    if (inReply->error() == QNetworkReply::OperationCanceledError) {
        inReply->deleteLater();
        if (mCancelRequested) {
            endIndex(false);
        }
        return;
    }
    if (inReply->error() == QNetworkReply::NoError) {
        const QString targetFileName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1)).value<QString>();
        const QByteArray downloadedData = inReply->readAll();
        mDownloadBytesReceived[inReply] = downloadedData.size();
        mDownloadBytesTotal[inReply] = qMax(mDownloadBytesTotal.value(inReply, 0), static_cast<qint64>(downloadedData.size()));
        updateDownloadProgressBar();
        if (!createTargetFile(targetFileName, downloadedData)) {
            inReply->deleteLater();
            endIndex(false);
            return;
        }
        inReply->deleteLater();
        stage2();
    }
    else {
        // error details
        const QString errorUrl = inReply->request().url().toString();
        const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            qWarning() << QString("Cannot download the file <%1> due to: %2").arg(errorUrl, inReply->errorString());
        }
        else {
            qWarning() << QString("Cannot download the file <%1> due to: %2 - %3").arg(errorUrl, httpStatus, httpStatusMessage);
        }
        inReply->deleteLater();
        endIndex(false);
    }
}

void IndexStep::stage3Slot(QNetworkReply * inReply) {
    if (inReply->error() == QNetworkReply::OperationCanceledError) {
        inReply->deleteLater();
        if (mCancelRequested) {
            endIndex(false);
        }
        return;
    }
    if (inReply->error() == QNetworkReply::NoError) {
        const QString targetFileName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1)).value<QString>();
        const QByteArray downloadedData = inReply->readAll();
        mDownloadBytesReceived[inReply] = downloadedData.size();
        mDownloadBytesTotal[inReply] = qMax(mDownloadBytesTotal.value(inReply, 0), static_cast<qint64>(downloadedData.size()));
        updateDownloadProgressBar();
        if (!createTargetFile(targetFileName, downloadedData)) {
            inReply->deleteLater();
            endIndex(false);
            return;
        }
        inReply->deleteLater();
        --mFilesToDownload;
    }
    else {
        // error details
        const QString errorUrl = inReply->request().url().toString();
        const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            qWarning() << QString("Cannot download the file <%1> due to: %2").arg(errorUrl).arg(inReply->errorString());
        }
        else {
            qWarning() << QString("Cannot download the file <%1> due to: %2 - %3").arg(errorUrl).arg(httpStatus).arg(httpStatusMessage);
        }
        inReply->deleteLater();
        endIndex(false);
    }
    if (mFilesToDownload <= 0) {
        stage3();
    }
}
