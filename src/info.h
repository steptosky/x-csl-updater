#ifndef INFO_H
#define INFO_H

#include <QtGui>
#include "ui_info.h"
#include "ui_mainwindow.h"
#include "types.h"
#include <QtNetwork>

namespace Ui {
    class info;
}

class info : public QDialog {
    Q_OBJECT
public:
    info(QWidget *parent, Ui::MainWindow *_MWUI);
    ~info();
    void GetInfoToTable();
    void OpenInfoWin();

protected:
    void changeEvent(QEvent *e);

private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

private:
    struct InfoType
    {
	int ID;
	QString Info;
	QString ShortInfo;
    };
    QVector<InfoType> FileInfo;
    Ui::info *m_ui;
    QWidget *MW;
    Ui::MainWindow *MWUI;
    void CopyRemoteFile(QString From);
    QHttp *http;
    int httpGetId;
    bool httpRequestAborted;
    int countMain;
    int Rows;
    QString server;
    QString FolderName;
    QBuffer *buffer;
};

#endif // INFO_H
