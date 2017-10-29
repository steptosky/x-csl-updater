#ifndef UPDATE_H
#define UPDATE_H

#include <QtGui>
#include "base_steps.h"
#include "index.h"

class Update : public BaseSteps {
	Q_OBJECT

public:
	Update(QWidget *_MW, Ui::MainWindow *_MWUI);
	~Update();
	void StartUpdate(QVector<PackageEntry> FilesList, Index *Indx);

signals:
	void cancelDownloading();

private slots:
	void httpRequestFinished(QNetworkReply *inReply);
	void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
	void CancelSlot();

private:
	bool removeDir(const QString &dirName);
	bool removePath(QString path);
	bool createDownloadingFile(PackageEntry inPackageEntry);
	void CopyRemoteFile(PackageEntry inPackageEntry);
	void EndUpdate();

	Index *mIndexStep;
	QNetworkAccessManager *mNetMng;
	QFile *mDownloadingFile;
	QString mDownloadingFileName;

	int mDownloadedBytes;
	int mTotalBytes;
	int mFileCounter;
	int mFailedFileCounter;

	int mDeletedFiles;

	QString mServer;

	QVector<PackageEntry> mSelectedListForDelete;
};

#endif // UPDATE_H
