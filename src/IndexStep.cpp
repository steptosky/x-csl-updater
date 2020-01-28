#include "IndexStep.h"
#include <QDebug>

IndexStep::IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf,
                     const QString & targetDir, const QString & targetCslDir)
    : BaseSteps(_MW, _MWUI, targetDir, targetCslDir) {
    //-------------------------------------------------------------------------
    mPackInfo = _Inf;
    mNetMng = new QNetworkAccessManager(this);
    connect(mNetMng, &QNetworkAccessManager::finished, this, &IndexStep::httpRequestFinished);
}

IndexStep::~IndexStep() {
    delete mNetMng;
    delete mIndexFile;
    delete mDelIndexFile;
}

void IndexStep::StartIndex() {
    mPackInfo->requestStopAction();
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    mSizeOfClient = 0;
    mSizeOfNeedUpdate = 0;
    mSizeOfServer = 0;
    mEntryList.clear();
    mFileListForDel.clear();
    InitProgBar(0, 1, 0, 1);
    mIndexFileUrl = settings.value("curServer").toString() + getIndexFileName();
    mDelIndexFileUrl = settings.value("curServer").toString() + getIndexForDelFileName();
    SetMessage(tr("Downloading the index files from the server \"%1\" ...").arg(QUrl(mIndexFileUrl).host()));

    mIndexBytesDownloaded = 0;
    mTotalIndexBytes = 0;
    mDelIndexBytesDownloaded = 0;
    mTotalDelIndexBytes = 0;

    if (!createIndexFile(getIndexFileName(), &mIndexFile)) {
        return;
    }
    if (!createIndexFile(getIndexForDelFileName(), &mDelIndexFile)) {
        return;
    }

    mFilesToDownload = 2;
    QNetworkRequest request;
    request.setUrl(mIndexFileUrl);
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(mIndexFile));
    QNetworkReply * reply = mNetMng->get(request);
    connect(this, &IndexStep::cancelAll, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &IndexStep::indexDownloadProgress);

    request.setUrl(mDelIndexFileUrl);
    request.setAttribute(static_cast<QNetworkRequest::Attribute>(QNetworkRequest::UserMax - 1), QVariant::fromValue(mDelIndexFile));
    reply = mNetMng->get(request);
    connect(this, &IndexStep::cancelAll, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &IndexStep::delIndexDownloadProgress);
    //
}

void IndexStep::EndIndex(int Next) {
    if (Next) {
        mSizeOfNeedUpdate = mSizeOfServer - mSizeOfClient;
        //TODO: разобраццо с лишними символами в мегабайтах
        float fsizeOfNeedUpdate, fsizeOfServer;
        fsizeOfNeedUpdate = (float)mSizeOfNeedUpdate / 1048576;
        fsizeOfServer = (float)mSizeOfServer / 1048576;
        char cstrSizeNeed[13];
        sprintf(cstrSizeNeed, "%10.2f", fsizeOfNeedUpdate);
        char cstrSizeAll[13];
        sprintf(cstrSizeAll, "%10.2f", fsizeOfServer);
        QString strSizeNeed(cstrSizeNeed), strSizeAll(cstrSizeAll);
        //1048576
        if (mSizeOfNeedUpdate != 0) {
            SetMessage(tr("To make all the packages up-to-date you have to download %1 MB of %2 MB").arg(strSizeNeed, strSizeAll));
            SetMessage(tr("Select the packages you want to update and click \"Update\"."));
        }
        else {
            SetMessage(tr("Congratulations! All the packages are fully up-to-date."));
        }
        mPackInfo->GetInfoToTable();
        MWUI->updateButton->setEnabled(true);
        MWUI->indexButton->setEnabled(true);
    }
    else {
        SetMessage(tr("Cannot get indexing successfully done!"));
        MWUI->indexButton->setEnabled(true);
    }
    emit cancelAll();
}

void IndexStep::ParseIndexFiles() {
    QString FileForDelPath = getIndexForDelFileName();
    QFile fileForDel(FileForDelPath);
    if (!fileForDel.open(QIODevice::ReadOnly)) {
        SetMessage(tr("Error: %1").arg(fileForDel.errorString()));
        EndIndex(false);
        return;
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
            mFileListForDel.push_back(fileInfo);
        }
    }
    fileForDel.close();

    QString FilePath = getIndexFileName();
    QFile file(FilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        SetMessage(tr("Error: %1").arg(file.errorString()));
        EndIndex(false);
        return;
    }
    if (file.size() < 1) {
        SetMessage(tr("Error: The index file has zero size!"));
        EndIndex(false);
        return;
    }
    //SetMessage(tr("Indexing..."));
    // QTextStream in(&file);
    int count = 0;
    int ver_file_stat = true;
    MWUI->tableWidget->clearContents();
    MWUI->tableWidget->setRowCount(0);
    while (!file.atEnd()) {
        QString line = file.readLine();
        //qDebug() << line;
        QString type = line.left(1);
        if (ver_file_stat) {
            if (type != "0") {
                SetMessage(tr("Error: The index file has wrong format!"));
                file.close();
                EndIndex(false);
                return;
            }
            ver_file_stat = false;
        }
        if (type == "#" || type == "0" || type.isEmpty())
            continue;
        QStringList list = line.split("%", QString::SkipEmptyParts);
        if (list.size() >= 6 && list[0] == "11") {
            MWUI->tableWidget->setRowCount(count + 1);
            char str[MY_MAX_PATH];
            sprintf(str, "%i", count);
            MWUI->tableWidget->setItem(count, 0, new QTableWidgetItem(str));
            MWUI->tableWidget->setItem(count, 1, new QTableWidgetItem(list[1]));
            MWUI->tableWidget->setItem(count, 2, new QTableWidgetItem(tr("Please wait...")));
            QString msg = tr("%3 (%4)").arg(list[4], list[5]);
            MWUI->tableWidget->setItem(count, 3, new QTableWidgetItem(msg));
            //sizeOfServer += list[2].toInt();
            float sizeMB = list[2].toFloat() / 1048576;
            char sizeMBstr[MY_MAX_PATH];
            sprintf(sizeMBstr, "%10.2f", sizeMB);
            MWUI->tableWidget->setItem(count, 4, new QTableWidgetItem(sizeMBstr));
            ePackageState status = CheckCslPack(file.pos(), count);
            char StrStatus[MY_MAX_PATH];
            sprintf(StrStatus, "%i", status);
            QTableWidgetItem * Item = new QTableWidgetItem();
            switch (status) {
                case CLIENT_PACKAGE_STATUS_OK:
                    Item->setText(tr("Up-to-date"));
                    Item->setTextColor(Qt::darkGreen);
                    MWUI->tableWidget->setItem(count, 5, Item);
                    MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
                    break;
                case CLIENT_PACKAGE_STATUS_CHANGE:
                    Item->setText(tr("Out-of-date"));
                    Item->setTextColor(Qt::red);
                    MWUI->tableWidget->setItem(count, 5, Item);
                    MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
                    break;
                case CLIENT_PACKAGE_STATUS_LOST:
                    Item->setText(tr("Not installed"));
                    Item->setTextColor(Qt::darkRed);
                    MWUI->tableWidget->setItem(count, 5, Item);
                    MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
                    break;
                default:
                    Item->setText(tr("Unknown state"));
                    Item->setTextColor(Qt::darkGray);
                    MWUI->tableWidget->setItem(count, 5, Item);
                    MWUI->tableWidget->setItem(count, 6, new QTableWidgetItem(StrStatus));
                    break;
            }
            count++;
        }
    }
    file.close();
    //MWUI->tableWidget->sortByColumn(1);
    MWUI->tableWidget->sortItems(1);
    EndIndex();
}

ePackageState IndexStep::CheckCslPack(int pos, int ID) {
    QString FilePath = getIndexFileName();
    QFile file(FilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        SetMessage(tr("Error: %1").arg(file.errorString()));
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
            int st = CheckFile(list, ID);
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

eFileState IndexStep::CheckFile(QStringList List, int ID) {
    PackageEntry FilesInfo;
    FilesInfo.ID = ID;
    FilesInfo.data = List;
    QString separator = (QString)QDir::separator();
    QString FilePath = mTargetCslDir + separator + List[1];
    QFileInfo fileInfo(FilePath);
    mSizeOfServer += List[2].toInt();
    // does file exist?
    if (!fileInfo.isFile()) {
        FilesInfo.state = CLIENT_FILE_STATUS_LOST;
        mEntryList.push_back(FilesInfo);
        return CLIENT_FILE_STATUS_LOST;
    }
    // is the size is the same?
    int size;
    size = List[2].toInt();
    if (size != (int)fileInfo.size()) {
        FilesInfo.state = CLIENT_FILE_STATUS_CHANGE;
        mEntryList.push_back(FilesInfo);
        return CLIENT_FILE_STATUS_CHANGE;
    }
    // check hash if it is available
    if (List[3] != "Reserve") {
        static QCryptographicHash hash(QCryptographicHash::Md5);
        static QFile file;
        hash.reset();
        file.close();
        file.setFileName(FilePath);
        if (file.open(QIODevice::ReadOnly)) {
            hash.addData(file.readAll());
            // 			qDebug() << List[1] << " - " << FilePath;
            // 			qDebug() << List[3] << " - " << QString(hash.result().toHex());
            if (List[3] != QString(hash.result().toHex())) {
                FilesInfo.state = CLIENT_FILE_STATUS_CHANGE;
                mEntryList.push_back(FilesInfo);
                return CLIENT_FILE_STATUS_CHANGE;
            }
            file.close();
        }
        else {
            FilesInfo.state = CLIENT_FILE_STATUS_LOST;
            mEntryList.push_back(FilesInfo);
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
    // типа все ОК
    mSizeOfClient += (int)fileInfo.size();
    FilesInfo.state = CLIENT_FILE_STATUS_OK;
    mEntryList.push_back(FilesInfo);
    return CLIENT_FILE_STATUS_OK;
}

QString IndexStep::getIndexFileName() {
    return "x-csl-indexes.idx";
}

QString IndexStep::getIndexForDelFileName() {
    return "x-csl-indexes-for-delete.idx";
}

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

        SetMessage(tr("Error : %1.").arg(httpStatus + " - " + httpStatusMessage));
        EndIndex(false);
        return;
    }
    if (mFilesToDownload <= 0) {
        ParseIndexFiles();
    }
}

void IndexStep::indexDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    mTotalIndexBytes = totalBytes;
    mIndexBytesDownloaded = bytesRead;
    MWUI->progressBar->setMaximum(mTotalIndexBytes + mTotalDelIndexBytes);
    MWUI->progressBar->setValue(mIndexBytesDownloaded + mDelIndexBytesDownloaded);
}

void IndexStep::delIndexDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    mTotalDelIndexBytes = totalBytes;
    mDelIndexBytesDownloaded = bytesRead;
    MWUI->progressBar->setMaximum(mTotalIndexBytes + mTotalDelIndexBytes);
    MWUI->progressBar->setValue(mIndexBytesDownloaded + mDelIndexBytesDownloaded);
}

bool IndexStep::createIndexFile(QString inFileName, QFile ** inIndexFile) {
    if (QFile::exists(inFileName)) {
        QFile::remove(inFileName);
    }
    delete *inIndexFile;
    *inIndexFile = new QFile(inFileName);
    QFile * file = *inIndexFile;
    if (!file->open(QIODevice::WriteOnly)) {
        SetMessage(tr("Error: Cannot write file: <%1>; Reason: %2").arg(inFileName).arg(file->errorString()));
        delete *inIndexFile;
        *inIndexFile = nullptr;
        return false;
    }
    return true;
}
