#ifndef INDEX_H
#define INDEX_H

#include <QtGui>
#include "base_steps.h"
#include "PackageAdditionalInfo.h"

class IndexStep /*: public QThread*/ : public BaseSteps {
Q_OBJECT
public:
    QVector<PackageEntry> mFileListForDel;

    IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf);
    ~IndexStep();
    void StartIndex();

    //-------------------------------------------------------------------------
signals:
    void abortAllReplaysSig();

    //-------------------------------------------------------------------------
private slots:
    void httpRequestFinished(QNetworkReply * inReply);
    void indexDownloadProgress(qint64 bytesRead, qint64 totalBytes);
    void delIndexDownloadProgress(qint64 bytesRead, qint64 totalBytes);
    void cancelSlot();
    //-------------------------------------------------------------------------
//public slots:
    void stage2Slot(QNetworkReply * inReply);
    void stage3Slot(QNetworkReply * inReply);

    //-------------------------------------------------------------------------
private:
    void stage2();
    void stage3();
//-------------------------------------------------------------------------
    bool createTargetFile(const QString & fileName, const QByteArray & bytesToWrite) const;
    void scheduleDownloadingFile(const QString & url, const QString & localPath) const;
    //-------------------------------------------------------------------------
    void ParseIndexFiles();
    ePackageState CheckCslPack(int pos, int ID);
    eFileState CheckFile(QStringList list, int ID);
    void EndIndex(int Next = true);

    //-------------------------------------------------------------------------
    PackageAdditionalInfo * mPackInfo;
    QNetworkAccessManager * mNetMng;
    QFile * mIndexFile = nullptr;
    QFile * mDelIndexFile = nullptr;

    int mFilesToDownload = 2;
    int mIndexBytesDownloaded;
    int mTotalIndexBytes;
    int mDelIndexBytesDownloaded;
    int mTotalDelIndexBytes;

    int mSizeOfServer;
    int mSizeOfNeedUpdate;
    int mSizeOfClient;

    QString mIndexFileUrl;
    QString mDelIndexFileUrl;
};

#endif // INDEX_H
