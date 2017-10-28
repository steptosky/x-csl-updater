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
	enum eAttributes {
		PackID = QNetworkRequest::UserMax - 1,
		PackName = QNetworkRequest::UserMax - 2,
		PackRow = QNetworkRequest::UserMax - 3,
	};

	info(QWidget *parent, Ui::MainWindow *_MWUI);
	~info();
	void GetInfoToTable();
	void OpenInfoWin();
	void requestStopAction();

protected:
	void changeEvent(QEvent *e);

signals:
	void cancelAllSig();

private slots:
	void httpRequestFinished(QNetworkReply *inReply);

private:
	struct PackInfo {
		int ID;
		QString Info;
		QString ShortInfo;
	};
	QVector<PackInfo> mPackInfo;
	Ui::info *mUi;
	Ui::MainWindow *mMainUi;
	void getPackageInfo(int inPackID, int inRow);
	QNetworkAccessManager *mNetMng;

	QString mServer;
	QString mCslFolder;

};

#endif // INFO_H
