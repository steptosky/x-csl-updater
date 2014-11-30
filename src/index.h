#ifndef INDEX_H
#define INDEX_H

#include <QtGui>
#include "base_steps.h"
#include "info.h"

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

class Index /*: public QThread*/: public BaseSteps
{
    Q_OBJECT
public:
	QVector<FilesTypes> mFileListForDel;

    Index(QWidget *_MW, Ui::MainWindow *_MWUI, info *_Inf);
    ~Index();
    void StartIndex();

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

private:
    void ParserIndexFile();
    int CheckCslPack(int pos, int ID);
    int CheckFile(QStringList list, int ID);
    void EndIndex(int Next = true);
    void CopyRemoteFile(QString From, QString To);
    QString getIndexFilePath();
	QString getIndexForDelFilePath();

    info *Inf;
    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
    int DownSize;
    int TotalDownSize;
    int sizeOfServer;
    int sizeOfNeedUpdate;
    int sizeOfClient;
};

#endif // INDEX_H
