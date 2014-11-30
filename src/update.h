#ifndef UPDATE_H
#define UPDATE_H

#include <QtGui>
#include "base_steps.h"
#include "index.h"

class Update : public BaseSteps
{
    Q_OBJECT

public:
    Update(QWidget *_MW, Ui::MainWindow *_MWUI);
    ~Update();
    void StartUpdate(QVector<FilesTypes> FilesList, Index *Indx);

signals:

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void CancelSlot();

private:
	bool Update::removeDir(const QString &dirName);
	void removePath(QString path);
    void CopyRemoteFile(QString From, QString To);
    void EndUpdate();
    Index *Indx;
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
    int DownSize;
    int TotalDownSize;
    int countMain;
    QString server;

	QVector<FilesTypes> mSelectedListForDelete;
};

#endif // UPDATE_H
