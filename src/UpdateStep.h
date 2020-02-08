#ifndef UPDATE_H
#define UPDATE_H

#include <QtGui>
#include "base_steps.h"
#include "IndexStep.h"

class UpdateStep : public BaseSteps {
Q_OBJECT

public:
    UpdateStep(QWidget * _MW, Ui::MainWindow * _MWUI);
    ~UpdateStep();
    void StartUpdate(QVector<PackageEntry> FilesList, IndexStep * Indx);

signals:
    void cancelDownloading();

private slots:
    void httpRequestFinished(QNetworkReply * inReply);
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void CancelSlot();

private:
    bool removeDir(const QString & dirName);
    bool removePath(PackageEntry inPackageEntry);
    bool createDownloadingFile(PackageEntry inPackageEntry);
    void CopyRemoteFile(PackageEntry inPackageEntry);
    void EndUpdate();

    //-------------------------------------------------------------------------
    AltitudeDefs * mAltDefs = nullptr;
    IndexStep * mIndexStep = nullptr;
    QNetworkAccessManager * mNetMng = nullptr;
    QFile * mDownloadingFile = nullptr;
    QString mDownloadingFileName;

    int mDownloadedBytes = 0;
    int mTotalBytes = 0;
    int mFileCounter = 0;
    int mFailedFileCounter = 0;

    int mDeletedFiles = 0;

    QVector<PackageEntry> mSelectedListForDelete;
};

#endif // UPDATE_H
