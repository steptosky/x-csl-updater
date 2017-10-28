#include "info.h"

info::info(QWidget *parent, Ui::MainWindow *_MWUI) :
	QDialog(parent),
	mUi(new Ui::info) {
	mUi->setupUi(this);
	this->mMainUi = _MWUI;
	this->mNetMng = new QNetworkAccessManager(this);
	connect(mNetMng, &QNetworkAccessManager::finished, this, &info::httpRequestFinished);
}

info::~info() {
	delete(this->mNetMng);
	delete mUi;
}

void info::OpenInfoWin() {
	for (int i = 0; i < this->mMainUi->tableWidget->rowCount(); ++i) {
		if (this->mMainUi->tableWidget->item(i, 0)->isSelected()) {
			int allCount = this->mPackInfo.size();
			int ID = this->mMainUi->tableWidget->item(i, 0)->text().toInt();
			for (int it = 0; it < allCount; ++it) {
				if (ID == this->mPackInfo[it].ID) {
					this->mUi->textBrowser->setHtml(this->mPackInfo[it].Info);
					break;
				}
			}
		}
	}
	this->show();
}

void info::GetInfoToTable() {
	QSettings settings(ORGANISATION, PROGRAM_NAME);
	this->mCslFolder = settings.value("FolderName").toString();
	this->mServer = settings.value("curServer").toString();
	this->mPackInfo.clear();
	for (size_t i = 0; i < this->mMainUi->tableWidget->rowCount(); i++) {
		this->getPackageInfo(this->mMainUi->tableWidget->item(i, 0)->text().toInt(), i);
	}	
}

void info::getPackageInfo(int inPackID, int inRow) {
	QString packPath = this->mMainUi->tableWidget->item(inRow, 1)->text();
	QUrl url(this->mServer + packPath + tr("/x-csl-info.info"));
	QString fileName = this->mCslFolder + "/" + packPath + tr("/x-csl-info.info");

	QNetworkRequest request;
	request.setUrl(url);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackID), inPackID);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackName), packPath);
	request.setAttribute(static_cast<QNetworkRequest::Attribute>(PackRow), inRow);
	QNetworkReply *reply = mNetMng->get(request);
	connect(this, &info::cancelAllSig, reply, &QNetworkReply::abort);
}

void info::httpRequestFinished(QNetworkReply *inReply) {
	inReply->deleteLater();

	int packId = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackID)).toInt();
	QString packName = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackName)).toString();
	int row = inReply->request().attribute(static_cast<QNetworkRequest::Attribute>(PackRow)).toInt();

	if (mMainUi->tableWidget->item(row, 0)->text().toInt() != packId
		|| mMainUi->tableWidget->item(row, 1)->text() != packName) {
		// something is wrong, maybe the table has been resorted.
		return;
	}
	
	if (inReply->error() == QNetworkReply::NoError) {
		PackInfo packInfo;
		packInfo.ID = packId;
		packInfo.Info = inReply->readAll();
		QStringList list = packInfo.Info.split("\n", QString::SkipEmptyParts);
		QString s_str(list[0]);
		int size = s_str.length();
		packInfo.ShortInfo = s_str.left(size - 1);
		this->mPackInfo.push_back(packInfo);
		QTableWidgetItem *Item = new QTableWidgetItem(packInfo.ShortInfo);
		this->mMainUi->tableWidget->setItem(row, 2, Item);
	}
	else {
		PackInfo packInfo;
		packInfo.ID = packId;
		packInfo.Info = tr("Информация отсутствует...");
		packInfo.ShortInfo = tr("Информация отсутствует...");
		this->mPackInfo.push_back(packInfo);
		QTableWidgetItem *Item = new QTableWidgetItem(packInfo.ShortInfo);
		Item->setTextColor(Qt::lightGray);
		this->mMainUi->tableWidget->setItem(row, 2, Item);

		// error details
		QString errorUrl = inReply->request.url();
		QString httpStatus = inReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		QString httpStatusMessage = inReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toByteArray();

	}
}

// void info::readResponseHeader(const QHttpResponseHeader &responseHeader) {
// 
// 	switch (responseHeader.statusCode()) {
// 		case 200:                   // Ok
// 		case 301:                   // Moved Permanently
// 		case 302:                   // Found
// 		case 303:                   // See Other
// 		case 307:                   // Temporary Redirect
// 			// these are not error conditions
// 			break;
// 		default:
// 			this->httpRequestAborted = true;
// 			this->mNetMng->abort();
// 			PackInfo Info;
// 			Info.ID = this->mMainUi->tableWidget->item(this->countMain, 0)->text().toInt();
// 			Info.Info = tr("Информация отсутствует...");
// 			Info.ShortInfo = tr("Информация отсутствует...");
// 			this->mPackInfo.push_back(Info);
// 			QTableWidgetItem *Item = new QTableWidgetItem(Info.ShortInfo);
// 			Item->setTextColor(Qt::lightGray);
// 			this->mMainUi->tableWidget->setItem(this->countMain, 2, Item);
// 			//this->MWUI->tableWidget->setItem(this->countMain, 2, new QTableWidgetItem(Info.ShortInfo));
// 			if (this->countMain < this->Rows - 1) {
// 				this->countMain++;
// 				this->getPackageInfo(this->mMainUi->tableWidget->item(this->countMain, 1)->text());
// 			}
// 			break;
// 	}
// }


void info::changeEvent(QEvent *e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			mUi->retranslateUi(this);
			break;
		default:
			break;
	}
}

void info::requestStopAction() {
	emit cancelAllSig();
}
