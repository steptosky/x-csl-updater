#include "IndexStep.h"
#include <QDebug>
#include "AltitudeDefs.h"

IndexStep::IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf)
    : BaseSteps(_MW, _MWUI) {
    //-------------------------------------------------------------------------
    mAltDefs = AltitudeDefs::instance();
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
        qWarning() << "Cannot write to file: <" << fileName << ">";
        qWarning() << "Reason: " << file.errorString();
        return false;
    }
    file.write(bytesToWrite);
    file.close();
    qDebug() << "The downloaded file has been put here: " << fileName;
    return true;
}

void IndexStep::scheduleDownloadingFile(const QString & url, const QString & localPath) const {
    qDebug() << "Will download a file from: <" << url << ">";
    QNetworkRequest request;
    request.setUrl(url);
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(localPath));
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &IndexStep::abortAllReplaysSig, reply, &QNetworkReply::abort);
}

/**************************************************************************************************/
//////////////////////////////////////////* CSL Indexing */////////////////////////////////////////////
/**************************************************************************************************/

void IndexStep::startIndex() {
    setMessage(tr("Indexing, please wait..."));
    resetIndex();

    MWUI->indexButton->setEnabled(false);
    MWUI->updateButton->setEnabled(false);

    const QSettings settings(gSettingsFileName, QSettings::IniFormat);
    mAltDefs->setServerUrl(settings.value("curServer").toString());

    connect(MWUI->cancelButton, &QPushButton::pressed, this, &IndexStep::cancelSlot);
    MWUI->cancelButton->setEnabled(true);

    // stage 1
    emit abortAllReplaysSig();
    scheduleDownloadingFile(mAltDefs->configFileUrl(), AltitudeDefs::configFileLocalPath());
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
    mFilesToDownload = 4;
    scheduleDownloadingFile(mAltDefs->indexFileUrl(), AltitudeDefs::indexFileLocalPath());
    scheduleDownloadingFile(mAltDefs->indexForDelFileUrl(), AltitudeDefs::indexForDelFileLocalPath());
    scheduleDownloadingFile(mAltDefs->cslIndexFileUrl(), AltitudeDefs::cslIndexFileLocalPath());
    scheduleDownloadingFile(mAltDefs->cslIndexForDelFileUrl(), AltitudeDefs::cslIndexForDelFileLocalPath());
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot, Qt::UniqueConnection);
}

void IndexStep::stage3() {
    qInfo() << "Indexing stage 3 has been entered.";

    disconnect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::stage3Slot);
    emit abortAllReplaysSig();
    // stage 3
    parseIndexFiles();
}

void IndexStep::endIndex(int Next) {
    qInfo() << "Indexing final stage has been entered.";
    if (Next) {
        setMessage(tr("Indexing local files is successfully done."));

        mSizeOfNeedUpdate = mSizeOfServer - mSizeOfClient;
        //TODO: разобраццо с лишними символами в мегабайтах
        double fsizeOfNeedUpdate, fsizeOfServer;
        fsizeOfNeedUpdate = mSizeOfNeedUpdate / 1048576;
        fsizeOfServer = mSizeOfServer / 1048576;
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
        mPackInfo->GetInfoToTable();
        MWUI->updateButton->setEnabled(true);
        MWUI->indexButton->setEnabled(true);
        qInfo() << "Indexing is done.";
    }
    else {
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

void IndexStep::addPackageToTable(int count, const QStringList & list) const {
    MWUI->tableWidget->setRowCount(count + 1);
    MWUI->tableWidget->setItem(count, 0, new QTableWidgetItem(QString::number(count)));
    MWUI->tableWidget->setItem(count, 1, new QTableWidgetItem(list[1]));
    MWUI->tableWidget->setItem(count, 2, new QTableWidgetItem(tr("Please wait...")));
    MWUI->tableWidget->setItem(count, 3, new QTableWidgetItem(QString("%3 (%4)").arg(list[4], list[5])));
    const double sizeMB = list[2].toDouble() / 1048576;
    MWUI->tableWidget->setItem(count, 4, new QTableWidgetItem(QString::number(sizeMB, 'f', 2).rightJustified(10)));
    qDebug() << QString("A package <%1> has been added at %2th row.").arg(list[1]).arg(count);
}

void IndexStep::addPackageStatusToTable(int count, ePackageState status) const {
    auto * statusTextItem = new QTableWidgetItem();
    auto * statusItem = new QTableWidgetItem(QString::number(status));
    statusTextItem->setText(tr(packageState2Text(status)));
    MWUI->tableWidget->setItem(count, 5, statusTextItem);
    MWUI->tableWidget->setItem(count, 6, statusItem);
    switch (status) {
        case CLIENT_PACKAGE_STATUS_OK:
            statusTextItem->setTextColor(Qt::darkGreen);
            break;
        case CLIENT_PACKAGE_STATUS_CHANGE:
            statusTextItem->setTextColor(Qt::red);
            break;
        case CLIENT_PACKAGE_STATUS_LOST:
            statusTextItem->setTextColor(Qt::darkRed);
            break;
        default:
            statusTextItem->setTextColor(Qt::darkGray);
            break;
    }
    qDebug() << QString("The status of the package at %1th row has ben set to: <%2>").arg(count).arg(packageState2Text(status));
}

bool IndexStep::parseIndexFile(int & count, const QString & indexFileName, bool isCslIndex) {
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
    int isFirstLine = true;
    while (!file.atEnd()) {
        QString line = file.readLine();
        //qDebug() << line;
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
        QStringList list = line.split("%", QString::SkipEmptyParts);
        if (list.size() >= 6 && list[0] == "11") {
            addPackageToTable(count, list);
            const ePackageState status = checkCslPack(file.pos(), count, indexFileName, isCslIndex);
            addPackageStatusToTable(count, status);
            count++;
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
    qDebug() << "The index for deleting file has been parsed.";
    return true;
}

void IndexStep::parseIndexFiles() {
    qDebug() << "Parsing index for delete files...";
    QString fileForDelPath = AltitudeDefs::indexForDelFileLocalPath();
    if (!mAltDefs->isCustomSimDirSelected() && !parseIndexForDelFile(fileForDelPath, false)) {
        endIndex(false);
        return;
    }
    stepProgBar();
    fileForDelPath = AltitudeDefs::cslIndexForDelFileLocalPath();
    if (!parseIndexForDelFile(fileForDelPath, true)) {
        endIndex(false);
        return;
    }
    stepProgBar();

    // packs
    qDebug() << "Parsing index files...";
    int count = 0;
    MWUI->tableWidget->clearContents();
    MWUI->tableWidget->setRowCount(0);
    // altitude pack
    QString indexFilePath = AltitudeDefs::indexFileLocalPath();
    if (!mAltDefs->isCustomSimDirSelected() && !parseIndexFile(count, indexFilePath, false)) {
        endIndex(false);
        return;
    }
    stepProgBar();
    // csl pack
    indexFilePath = AltitudeDefs::cslIndexFileLocalPath();
    if (!parseIndexFile(count, indexFilePath, true)) {
        endIndex(false);
        return;
    }
    stepProgBar();
    MWUI->tableWidget->sortItems(1);
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
    if (isCslIndex) {
        fileEntry.type = CSL_PACK_FILE;
    }
    else {
        fileEntry.type = ADDITIONAL_FILE;
    }
    const QString filePath = mAltDefs->fullLocalPath(fileEntry.type, List[1]);
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
        stepProgBar();
        stage2();
    }
    else {
        // error details
        const QString errorUrl = inReply->request().url().toString();
        const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            qWarning() << QString("Cannot download a file <%1> due to: %2").arg(errorUrl).arg(inReply->errorString());
        }
        else {
            qWarning() << QString("Cannot download a file <%1> due to: %2 - %3").arg(errorUrl).arg(httpStatus).arg(httpStatusMessage);
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
        stepProgBar();
        --mFilesToDownload;
    }
    else {
        // error details
        const QString errorUrl = inReply->request().url().toString();
        const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
        if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
            qWarning() << QString("Cannot download a file <%1> due to: %2").arg(errorUrl).arg(inReply->errorString());
        }
        else {
            qWarning() << QString("Cannot download a file <%1> due to: %2 - %3").arg(errorUrl).arg(httpStatus).arg(httpStatusMessage);
        }
        endIndex(false);
    }
    if (mFilesToDownload <= 0) {
        stepProgBar();
        stage3();
    }
}
