// SPDX-License-Identifier: MPL-2.0

#ifndef INFO_H
#define INFO_H

#include <QtGui>
#include "ui_PackageAdditionalInfo.h"
#include "ui_mainwindow.h"
#include "Definitions.h"
#include <QtNetwork>

namespace Ui {
	class PackageAdditionalInfo;
}

class PackageAdditionalInfo : public QDialog {
	Q_OBJECT
public:
	enum eAttributes {
		PackID = QNetworkRequest::UserMax - 1,
		PackName = QNetworkRequest::UserMax - 2,
		PackRow = QNetworkRequest::UserMax - 3,
	};

	PackageAdditionalInfo(QWidget *parent, Ui::MainWindow *_MWUI);
	~PackageAdditionalInfo();
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
	Ui::PackageAdditionalInfo *mUi;
	Ui::MainWindow *mMainUi;
	void getPackageInfo(int inPackID, int inRow) const;
	QNetworkAccessManager *mNetMng;

	QString mServer;

};

#endif // INFO_H
