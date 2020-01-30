#ifndef INDEX_H
#define INDEX_H

#include <QtGui>
#include "base_steps.h"
#include "PackageAdditionalInfo.h"

class IndexStep /*: public QThread*/ : public BaseSteps {
Q_OBJECT
public:
    QVector<PackageEntry> mFileListForDel;

    IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf, const QString & targetDir, const QString & targetCslDir);
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
    void test();

    //-------------------------------------------------------------------------
private:
    static QString getIndexFileName();
    static QString getIndexForDelFileName();
    static QString getLocalIndexFilePath();
    static QString getLocalIndexForDelFilePath();
    bool createIndexFile(QString inFileName, QFile ** inIndexFile) const;
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
