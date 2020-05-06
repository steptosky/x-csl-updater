#ifndef INDEX_H
#define INDEX_H

#include <QtGui>
#include "base_steps.h"
#include "PackageAdditionalInfo.h"
#include "AltitudeDefs.h"

class IndexStep /*: public QThread*/ : public BaseSteps {
Q_OBJECT
public:
    QVector<PackageEntry> mFileListForDel;

    IndexStep(QWidget * _MW, Ui::MainWindow * _MWUI, PackageAdditionalInfo * _Inf);
    ~IndexStep();
    void startIndex();
    void resetIndex();

    //-------------------------------------------------------------------------
signals:
    void abortAllReplaysSig();

    //-------------------------------------------------------------------------
private slots:
    void cancelSlot();
    //-------------------------------------------------------------------------
    //public slots:
    void stage2Slot(QNetworkReply * inReply);
    void stage3Slot(QNetworkReply * inReply);

    //-------------------------------------------------------------------------
private:
    bool createTargetFile(const QString & fileName, const QByteArray & bytesToWrite) const;
    void scheduleDownloadingFile(const QString & url, const QString & localPath) const;
    //-------------------------------------------------------------------------
    void stage2();
    void stage3();
    void endIndex(int Next = true);
    //-------------------------------------------------------------------------
    void addPackageToTable(int count, const QStringList & list) const;
    void addPackageStatusToTable(int count, ePackageState status) const;
    bool parseIndexFile(int & count, const QString & indexFileName, bool isCslIndex);
    bool parseIndexForDelFile(const QString & indexFileName, bool isCslIndex);
    void parseIndexFiles();
    ePackageState checkCslPack(int pos, int ID, const QString & indexFileName, bool isCslIndex);
    eFileState checkFile(QStringList list, int ID, bool isCslIndex);
    //-------------------------------------------------------------------------
    AltitudeDefs* mAltDefs = nullptr;
    //-------------------------------------------------------------------------
    PackageAdditionalInfo * mPackInfo;
    QNetworkAccessManager * mNetMng;

    int mFilesToDownload = 0;

    size_t mSizeOfServer;
    size_t mSizeOfNeedUpdate;
    size_t mSizeOfClient;

    mutable QLocale mLocale;
};

#endif // INDEX_H
