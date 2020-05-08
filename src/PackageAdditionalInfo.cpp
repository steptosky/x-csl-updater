#include "PackageAdditionalInfo.h"
#include "AltitudeDefs.h"

PackageAdditionalInfo::PackageAdditionalInfo(QWidget *parent, Ui::MainWindow *_MWUI) :
	QDialog(parent),
	mUi(new Ui::PackageAdditionalInfo) {
	mUi->setupUi(this);
	mMainUi = _MWUI;
	mNetMng = new QNetworkAccessManager(this);
	connect(mNetMng, &QNetworkAccessManager::finished, this, &PackageAdditionalInfo::httpRequestFinished);
}

PackageAdditionalInfo::~PackageAdditionalInfo() {
	delete(mNetMng);
	delete mUi;
}

void PackageAdditionalInfo::OpenInfoWin() {
	for (int i = 0; i < mMainUi->tableWidget->rowCount(); ++i) {
		if (mMainUi->tableWidget->item(i, 0)->isSelected()) {
			int allCount = mPackInfo.size();
			int ID = mMainUi->tableWidget->item(i, 0)->text().toInt();
			for (int it = 0; it < allCount; ++it) {
				if (ID == mPackInfo[it].ID) {
					mUi->textBrowser->setHtml(mPackInfo[it].Info);
					break;
				}
			}
		}
	}
	show();
}

void PackageAdditionalInfo::GetInfoToTable() {
    const QSettings settings(gSettingsFileName, QSettings::IniFormat);
	mServer = settings.value("curServer").toString();
	mPackInfo.clear();
	for (int i = 0; i < mMainUi->tableWidget->rowCount(); i++) {
		getPackageInfo(mMainUi->tableWidget->item(i, 0)->text().toInt(), i);
	}	
}

void PackageAdditionalInfo::getPackageInfo(int inPackID, int inRow) const {
    const QString packPath = mMainUi->tableWidget->item(inRow, 1)->text();
	AltitudeDefs* altDefs = AltitudeDefs::instance();
	QUrl url;
	if (!altDefs->isCustomSimDirSelected() && inRow == 0){
		url = altDefs->fileUrl(packPath + "/" + AltitudeDefs::infoFileName());
	}
	else{
		url = altDefs->cslFileUrl(packPath + "/" + AltitudeDefs::infoFileName());
	}

	QNetworkRequest request;
	request.setUrl(url);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackID), inPackID);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackName), packPath);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackRow), inRow);
	QNetworkReply *reply = mNetMng->get(request);
	connect(this, &PackageAdditionalInfo::cancelAllSig, reply, &QNetworkReply::abort);
}

void PackageAdditionalInfo::httpRequestFinished(QNetworkReply *inReply) {
	inReply->deleteLater();

    const int packId = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackID)).toInt();
    const QString packName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackName)).toString();
	int row = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackRow)).toInt();

	if (row >= mMainUi->tableWidget->rowCount()
		|| mMainUi->tableWidget->item(row, 0)->text().toInt() != packId
		|| mMainUi->tableWidget->item(row, 1)->text() != packName) {
		// something is wrong, maybe the table has been resorted.
		return;
	}
	
	if (inReply->error() == QNetworkReply::NoError) {
		PackInfo packInfo;
		packInfo.ID = packId;
		packInfo.Info = inReply->readAll();
		QStringList list = packInfo.Info.split("\n", QString::SkipEmptyParts);
		// QString s_str(list[0]);
		// int size = s_str.length();
		// packInfo.ShortInfo = s_str.left(size - 1);
		packInfo.ShortInfo = list[0];
		mPackInfo.push_back(packInfo);
		QTableWidgetItem *Item = new QTableWidgetItem(packInfo.ShortInfo);
		mMainUi->tableWidget->setItem(row, 2, Item);
		qDebug() << QString("An info file has been downloaded: <%1>").arg(inReply->url().toString());
	}
	else {
		PackInfo packInfo;
		packInfo.ID = packId;
		packInfo.Info = tr("There is no information...");
		packInfo.ShortInfo = tr("There is no information...");
		mPackInfo.push_back(packInfo);
		QTableWidgetItem *Item = new QTableWidgetItem(packInfo.ShortInfo);
		Item->setTextColor(Qt::lightGray);
		mMainUi->tableWidget->setItem(row, 2, Item);

		// error details
		const QString errorUrl = inReply->request().url().toString();
		const QString httpStatus = QString::number(inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
		const QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();
		if (httpStatus.toInt() == 0 || httpStatusMessage.isEmpty()) {
			qWarning() << QString("Cannot download this info file <%1> due to: %2").arg(errorUrl).arg(inReply->errorString());
		}
		else {
			qWarning() << QString("Cannot download this info file <%1> due to: %2 - %3").arg(errorUrl).arg(httpStatus).arg(httpStatusMessage);
		}
	}
}

void PackageAdditionalInfo::changeEvent(QEvent *e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			mUi->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PackageAdditionalInfo::requestStopAction() {
	emit cancelAllSig();
}
