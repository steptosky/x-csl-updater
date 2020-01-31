#include "IndexStep.h"
#include <QDebug>
#include "AltitudeDefs.h"

IndexStep::IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf)
    : BaseSteps(_MW, _MWUI, "", "") {
    //-------------------------------------------------------------------------
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
        bool res = dir.mkpath(QFileInfo(fileName).dir().path());
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        setMessage(tr("Error: Cannot write file: <%1>").arg(fileName));
        setMessage(tr("Reason: %1").arg(file.errorString()));
        return false;
    }
    file.write(bytesToWrite);
    file.close();
    qInfo() << "Created file: " << fileName;
    return true;
}

void IndexStep::scheduleDownloadingFile(const QString & url, const QString & localPath) const {
    QNetworkRequest request;
    setMessage(QString("Downloading a file from the server <%1> ...").arg(url));
    setMessage(QString("To local path: <%1>").arg(localPath));
    request.setUrl(url);
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(localPath));
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &IndexStep::abortAllReplaysSig, reply, &QNetworkReply::abort);
}

/**************************************************************************************************/
//////////////////////////////////////////* CSL Indexing */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::startIndex() {
    const QSettings settings(gSettingsFileName, QSettings::IniFormat);

    AltitudeDefs * altDefs = AltitudeDefs::instance();
    altDefs->setServerUrl(settings.value("curServer").toString());

    connect(MWUI->cancelButton, &QPushButton::pressed, this, &IndexStep::cancelSlot);
    MWUI->cancelButton->setEnabled(true);
    //
    mPackInfo->requestStopAction();
    mSizeOfClient = 0;
    mSizeOfNeedUpdate = 0;
    mSizeOfServer = 0;
    mEntryList.clear();
    mFileListForDel.clear();

    initProgBar(0, 1, 0, 1);

    // stage 1
    emit abortAllReplaysSig();
    scheduleDownloadingFile(altDefs->configFileUrl(), AltitudeDefs::configFileLocalPath());
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage2Slot, Qt::UniqueConnection);

}

void IndexStep::stage2() {
    setMessage(tr("Hello from stage 2!"));
    AltitudeDefs * altDefs = AltitudeDefs::instance();
    if (!altDefs->getAllPathsAndUrlsReady()) {
        endIndex(false);
        return;
    }
    //
    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage2Slot);
    emit abortAllReplaysSig();
    // stage 2
    mFilesToDownload = 4;
    scheduleDownloadingFile(altDefs->indexFileUrl(), AltitudeDefs::indexFileLocalPath());
    scheduleDownloadingFile(altDefs->indexForDelFileUrl(), AltitudeDefs::indexForDelFileLocalPath());
    scheduleDownloadingFile(altDefs->cslIndexFileUrl(), AltitudeDefs::cslIndexFileLocalPath());
    scheduleDownloadingFile(altDefs->cslIndexForDelFileUrl(), AltitudeDefs::cslIndexForDelFileLocalPath());
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot, Qt::UniqueConnection);
}

void IndexStep::stage3() {
    setMessage(tr("Hello from stage 3!"));

    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot);
    emit abortAllReplaysSig();
    // stage 3
    parseIndexFiles();
}

void IndexStep::endIndex(int Next) {
    if (Next) {
        setMessage(tr("Indexing local files is successfully done."));

        mSizeOfNeedUpdate = mSizeOfServer - mSizeOfClient;
        //TODO: разобраццо с лишними символами в мегабайтах
        float fsizeOfNeedUpdate, fsizeOfServer;
        fsizeOfNeedUpdate = (float)mSizeOfNeedUpdate / 1048576;
        fsizeOfServer = (float)mSizeOfServer / 1048576;
        char cstrSizeNeed[13];
        sprintf(cstrSizeNeed, "%7.2f", fsizeOfNeedUpdate);
        char cstrSizeAll[13];
        sprintf(cstrSizeAll, "%7.2f", fsizeOfServer);
        QString strSizeNeed(cstrSizeNeed), strSizeAll(cstrSizeAll);
        //1048576
        if (mSizeOfNeedUpdate != 0) {
            setMessage(tr("To make all the packages up-to-date you have to download %1 MB of %2 MB").arg(strSizeNeed, strSizeAll));
            setMessage(tr("Select the packages you want to update and click \"Update\"."));
        }
        else {
            setMessage(tr("Congratulations! All the packages are fully up-to-date."));
        }
        //TODO: fix and return this
        //mPackInfo->GetInfoToTable();
        MWUI->updateButton->setEnabled(true);
        MWUI->indexButton->setEnabled(true);
    }
    else {
        setMessage(tr("Cannot get indexing successfully done!"));
        MWUI->indexButton->setEnabled(true);
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

void IndexStep::addPackageToTable(int count, const QStringList & list) const {
    MWUI->tableWidget->setRowCount(count + 1);
    MWUI->tableWidget->setItem(count, 0, new QTableWidgetItem(QString::number(count)));
    MWUI->tableWidget->setItem(count, 1, new QTableWidgetItem(list[1]));
    MWUI->tableWidget->setItem(count, 2, new QTableWidgetItem(tr("Please wait...")));
    MWUI->tableWidget->setItem(count, 3, new QTableWidgetItem(QString("%3 (%4)").arg(list[4], list[5])));
    const double sizeMB = list[2].toDouble() / 1048576;
    MWUI->tableWidget->setItem(count, 4, new QTableWidgetItem(QString::number(sizeMB, 'f', 2).rightJustified(10)));
}

void IndexStep::addPackageStatusToTable(int count, ePackageState status) const {
    auto * statusTextItem = new QTableWidgetItem();
    auto * statusItem = new QTableWidgetItem(QString::number(status));
    switch (status) {
        case CLIENT_PACKAGE_STATUS_OK:
            statusTextItem->setText(tr("Up-to-date"));
            statusTextItem->setTextColor(Qt::darkGreen);
            MWUI->tableWidget->setItem(count, 5, statusTextItem);
            MWUI->tableWidget->setItem(count, 6, statusItem);
            break;
        case CLIENT_PACKAGE_STATUS_CHANGE:
            statusTextItem->setText(tr("Out-of-date"));
            statusTextItem->setTextColor(Qt::red);
            MWUI->tableWidget->setItem(count, 5, statusTextItem);
            MWUI->tableWidget->setItem(count, 6, statusItem);
            break;
        case CLIENT_PACKAGE_STATUS_LOST:
            statusTextItem->setText(tr("Not installed"));
            statusTextItem->setTextColor(Qt::darkRed);
            MWUI->tableWidget->setItem(count, 5, statusTextItem);
            MWUI->tableWidget->setItem(count, 6, statusItem);
            break;
        default:
            statusTextItem->setText(tr("Unknown state"));
            statusTextItem->setTextColor(Qt::darkGray);
            MWUI->tableWidget->setItem(count, 5, statusTextItem);
            MWUI->tableWidget->setItem(count, 6, statusItem);
            break;
    }
}

bool IndexStep::parseIndexFile(int & count, const QString & indexFileName, bool isCslIndex) {
    QFile file(indexFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        setMessage(tr("Error: %1").arg(file.errorString()));
        return false;
    }
    if (file.size() < 1) {
        setMessage(tr("Error: The index file has zero size!"));
        return false;
    }
    int isFirstLine = true;
    while (!file.atEnd()) {
        QString line = file.readLine();
        //qDebug() << line;
        QString type = line.left(1);
        if (isFirstLine) {
            if (type != "0") {
                setMessage(tr("Error: The index file has wrong format! File: <%1>").arg(indexFileName));
                file.close();
                return false;
            }
            isFirstLine = false;
        }
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", QString::SkipEmptyParts);
        if (list.size() >= 6 && list[0] == "11") {
            addPackageToTable(count, list);
            const ePackageState status = checkCslPack(file.pos(), count, indexFileName, isCslIndex);
            addPackageStatusToTable(count, status);
            count++;
        }
    }
    file.close();
    return true;
}

bool IndexStep::parseIndexForDelFile(const QString & indexFileName, bool isCslIndex) {
    QFile fileForDel(indexFileName);
    if (!fileForDel.open(QIODevice::ReadOnly)) {
        setMessage(tr("Error: %1").arg(fileForDel.errorString()));
        return false;
    }
    while (!fileForDel.atEnd()) {
        QString line = fileForDel.readLine();
        QString type = line.left(1);
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", QString::SkipEmptyParts);
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
    return true;
}

void IndexStep::parseIndexFiles() {
    setMessage(tr("Indexing local files ..."));
    QString fileForDelPath = AltitudeDefs::indexForDelFileLocalPath();
    if (!parseIndexForDelFile(fileForDelPath, false)) {
        endIndex(false);
        return;
    }
    fileForDelPath = AltitudeDefs::cslIndexForDelFileLocalPath();
    if (!parseIndexForDelFile(fileForDelPath, true)) {
        endIndex(false);
        return;
    }

    // packs
    int count = 0;
    MWUI->tableWidget->clearContents();
    MWUI->tableWidget->setRowCount(0);
    // altitude pack
    QString indexFilePath = AltitudeDefs::indexFileLocalPath();
    if (!parseIndexFile(count, indexFilePath, false)) {
        endIndex(false);
        return;
    }
    // csl pack
    indexFilePath = AltitudeDefs::cslIndexFileLocalPath();
    if (!parseIndexFile(count, indexFilePath, true)) {
        endIndex(false);
        return;
    }
    MWUI->tableWidget->sortItems(1);
    endIndex();
}

ePackageState IndexStep::checkCslPack(int pos, int ID, const QString & indexFileName, bool isCslIndex) {
    QFile file(indexFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        setMessage(tr("Error: %1").arg(file.errorString()));
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
        QStringList list = line.split("%", QString::SkipEmptyParts);
        if (list[0] == "11")
            break;
        if (list[0] == "10") {
            int st = checkFile(list, ID, isCslIndex);
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
    QString filePath;
    if (isCslIndex) {
        fileEntry.type = CSL_PACK_FILE;
        filePath = AltitudeDefs::instance()->cslFileLocalPath(List[1]);
    }
    else {
        fileEntry.type = ADDITIONAL_FILE;
        filePath = AltitudeDefs::instance()->fileLocalPath(List[1]);
    }
    const QFileInfo fileInfo(filePath);
    mSizeOfServer += List[2].toInt();
    // does file exist?
    if (!fileInfo.isFile()) {
        fileEntry.state = CLIENT_FILE_STATUS_LOST;
        mEntryList.push_back(fileEntry);
        return CLIENT_FILE_STATUS_LOST;
    }
    const int size = List[2].toInt();
    if (size != static_cast<int>(fileInfo.size())) {
        fileEntry.state = CLIENT_FILE_STATUS_CHANGE;
        mEntryList.push_back(fileEntry);
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
                return CLIENT_FILE_STATUS_CHANGE;
            }
            file.close();
        }
        else {
            fileEntry.state = CLIENT_FILE_STATUS_LOST;
            mEntryList.push_back(fileEntry);
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
    return CLIENT_FILE_STATUS_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::httpRequestFinished(QNetworkReply * inReply) {
    inReply->deleteLater();
    QFile * indexFile = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1)).value<QFile*>();

    if (inReply->error() == QNetworkReply::NoError) {
        indexFile->write(inReply->readAll());
        indexFile->close();
        --mFilesToDownload;
    }
    else {
        // error details
        QString errorUrl = inReply->request().url().toString();
        QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();

        setMessage(tr("Error : %1.").arg(httpStatus + " - " + httpStatusMessage));
        endIndex(false);
        return;
    }
    if (mFilesToDownload <= 0) {
        parseIndexFiles();
    }
}

void IndexStep::indexDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    // mTotalIndexBytes = totalBytes;
    // mIndexBytesDownloaded = bytesRead;
    // MWUI->progressBar->setMaximum(mTotalIndexBytes + mTotalDelIndexBytes);
    // MWUI->progressBar->setValue(mIndexBytesDownloaded + mDelIndexBytesDownloaded);
}

void IndexStep::delIndexDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    // mTotalDelIndexBytes = totalBytes;
    // mDelIndexBytesDownloaded = bytesRead;
    // MWUI->progressBar->setMaximum(mTotalIndexBytes + mTotalDelIndexBytes);
    // MWUI->progressBar->setValue(mIndexBytesDownloaded + mDelIndexBytesDownloaded);
}

void IndexStep::cancelSlot() {
    setMessage(tr("Indexing process has been canceled by user!"));
    endIndex(false);
}

void IndexStep::stage2Slot(QNetworkReply * inReply) {
    inReply->deleteLater();
    if (inReply->error() == QNetworkReply::OperationCanceledError) {
        return;
    }
    if (inReply->error() == QNetworkReply::NoError) {
        const QString targetFileName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1)).value<QString>();
        if (!createTargetFile(targetFileName, inReply->readAll())) {
            endIndex(false);
            return;
        }
        stage2();
    }
    else {
        // error details
        QString errorUrl = inReply->request().url().toString();
        QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            setMessage(tr("Cannot download a file due to: %1").arg(inReply->errorString()));
            setMessage(tr("The file url: <%1>").arg(errorUrl));
        }
        else {
            setMessage(tr("Cannot download a file due to: %1 - %2").arg(httpStatus).arg(httpStatusMessage));
            setMessage(tr("The file url: <%1>").arg(errorUrl));
        }
        endIndex(false);
    }
}

void IndexStep::stage3Slot(QNetworkReply * inReply) {
    inReply->deleteLater();
    if (inReply->error() == QNetworkReply::OperationCanceledError) {
        return;
    }
    if (inReply->error() == QNetworkReply::NoError) {
        const QString targetFileName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1)).value<QString>();
        if (!createTargetFile(targetFileName, inReply->readAll())) {
            endIndex(false);
            return;
        }
        --mFilesToDownload;
    }
    else {
        // error details
        QString errorUrl = inReply->request().url().toString();
        QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            setMessage(tr("Cannot download a file due to: %1").arg(inReply->errorString()));
            setMessage(tr("The file url: <%1>").arg(errorUrl));
        }
        else {
            setMessage(tr("Cannot download a file due to: %1 - %2").arg(httpStatus).arg(httpStatusMessage));
            setMessage(tr("The file url: <%1>").arg(errorUrl));
        }
        endIndex(false);
    }
    if (mFilesToDownload <= 0) {
        stage3();
    }
}
