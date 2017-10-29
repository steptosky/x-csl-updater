#ifndef INDEX_H
#define INDEX_H

#include <QtGui>
#include "base_steps.h"
#include "PackageAdditionalInfo.h"

class IndexStep /*: public QThread*/ : public BaseSteps {
	Q_OBJECT
public:
	QVector<PackageEntry> mFileListForDel;

	IndexStep(QWidget *_MW, Ui::MainWindow *_MWUI, PackageAdditionalInfo *_Inf);
	~IndexStep();
	void StartIndex();

signals:
	void cancelAll();

private slots:
	void httpRequestFinished(QNetworkReply *inReply);
	void indexDonwloadProgress(qint64 bytesRead, qint64 totalBytes);
	void delIndexDonwloadProgress(qint64 bytesRead, qint64 totalBytes);

private:
	void ParseIndexFiles();
	int CheckCslPack(int pos, int ID);
	int CheckFile(QStringList list, int ID);
	void EndIndex(int Next = true);
	QString getIndexFilePath();
	QString getIndexForDelFilePath();
	bool createIndexFile(QString inFileName, QFile **inIndexFile);

	PackageAdditionalInfo *mPackInfo;
	QNetworkAccessManager *mNetMng;
	QFile *mIndexFile = nullptr;
	QFile *mDelIndexFile = nullptr;

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
