#ifndef UPDATE_H
#define UPDATE_H

#include <QtGui>
#include "base_steps.h"
#include "IndexStep.h"

class UpdateStep : public BaseSteps {
	Q_OBJECT

public:
	UpdateStep(QWidget *_MW, Ui::MainWindow *_MWUI);
	~UpdateStep();
	void StartUpdate(QVector<PackageEntry> FilesList, IndexStep *Indx);

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

	IndexStep *mIndexStep;
	QNetworkAccessManager *mNetMng;
	QFile *mDownloadingFile = nullptr;
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
