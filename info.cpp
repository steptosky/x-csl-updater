#include "info.h"

info::info(QWidget *parent, Ui::MainWindow *_MWUI) :
    QDialog(parent),
    m_ui(new Ui::info)
{
    m_ui->setupUi(this);
    this->MW = parent;
    this->MWUI = _MWUI;
    this->http = new QHttp();

    connect(this->http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
    connect(this->http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
    connect(this->http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
}

info::~info()
{
    this->httpRequestAborted = true;
    this->http->abort();
    this->http->close();
    delete m_ui;
}

void info::OpenInfoWin()
{
    for (int i = 0; i < this->Rows; ++i)
    {
	if(this->MWUI->tableWidget->item(i, 0)->isSelected())
	{
	    int all_count = this->FileInfo.size();
	    int ID = this->MWUI->tableWidget->item(i, 0)->text().toInt();
	    for (int it = 0; it < all_count; ++it)
	    {
		if (ID == this->FileInfo[it].ID)
		{
		    this->m_ui->textBrowser->setHtml(this->FileInfo[it].Info);
		    break;
		}
	    }
	}
    }
    this->show();
}

void info::GetInfoToTable()
{
    this->httpRequestAborted = true;
    this->http->abort();
    QSettings settings("VA X-Air Team && StepToSky Team", "X-CSL-Updater");
    this->FolderName = settings.value("FolderName").toString();
    this->server = settings.value("curServer").toString();
    this->FileInfo.clear();
    this->Rows = this->MWUI->tableWidget->rowCount();
    this->countMain = 0;
    this->CopyRemoteFile(this->MWUI->tableWidget->item(this->countMain, 1)->text());
}

void info::CopyRemoteFile(QString From)
{
    QUrl url(this->server+From+tr("/x-csl-info.info"));
    QString fileName = this->FolderName+tr("/")+From+tr("/x-csl-info.info");
    this->buffer = new QBuffer();
    this->buffer->open(QIODevice::WriteOnly);

    if (!this->buffer->isOpen())
    {
	this->MWUI->listWidget->addItem(this->buffer->errorString());
	this->buffer = 0;
	InfoType Info;
	Info.ID = this->MWUI->tableWidget->item(this->countMain, 0)->text().toInt();
	Info.Info = tr("Информация отсутствует...");
	Info.ShortInfo = tr("Информация отсутствует...");
	this->FileInfo.push_back(Info);
	this->MWUI->tableWidget->setItem(this->countMain, 2, new QTableWidgetItem(Info.ShortInfo));
	if (this->countMain < this->Rows-1)
	{
	    this->countMain++;
	    this->CopyRemoteFile(this->MWUI->tableWidget->item(this->countMain, 1)->text());
	}
	return;
    }
    this->http->setHost(url.host());
    this->httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty()) path = "/";
    this->httpGetId = this->http->get(path, this->buffer);
}

void info::httpRequestFinished(int reqId, bool error)
{
    if (reqId != this->httpGetId) return;
    if (this->httpRequestAborted)
    {
	if (this->buffer->isOpen())
	{
	    this->buffer->close();
	    delete this->buffer;
	    this->buffer = 0;
	}
	this->buffer = 0;
	return;
    }
    QByteArray Array(this->buffer->data());
    this->buffer->close();
    delete this->buffer;
    this->buffer = 0;
    QString str(tr(Array));
    InfoType Info;
    Info.ID = this->MWUI->tableWidget->item(this->countMain, 0)->text().toInt();
    Info.Info = str;
    QStringList list = str.split("\n", QString::SkipEmptyParts);
    QString s_str(list[0]);
    int size = s_str.length();
    /*int home = str.indexOf(tr("<x-title>"));
    int end = str.indexOf(tr("</x-title>"));*/
    //Info.ShortInfo = str.section("<x-title>", 1, 1);
    Info.ShortInfo = s_str.left(size-1);
    this->FileInfo.push_back(Info);
    this->MWUI->tableWidget->setItem(this->countMain, 2, new QTableWidgetItem(Info.ShortInfo));
    if (this->countMain < this->Rows-1)
    {
	this->countMain++;
	this->CopyRemoteFile(this->MWUI->tableWidget->item(this->countMain, 1)->text());
    }
}

void info::readResponseHeader(const QHttpResponseHeader &responseHeader)
{

    switch (responseHeader.statusCode())
    {
	case 200:                   // Ok
	case 301:                   // Moved Permanently
	case 302:                   // Found
	case 303:                   // See Other
	case 307:                   // Temporary Redirect
	// these are not error conditions
	break;
	default:
	this->httpRequestAborted = true;
	this->http->abort();
	InfoType Info;
	Info.ID = this->MWUI->tableWidget->item(this->countMain, 0)->text().toInt();
	Info.Info = tr("Информация отсутствует...");
	Info.ShortInfo = tr("Информация отсутствует...");
	this->FileInfo.push_back(Info);
	QTableWidgetItem *Item = new QTableWidgetItem(Info.ShortInfo);
	Item->setTextColor(Qt::lightGray);
	this->MWUI->tableWidget->setItem(this->countMain, 2, Item);
	//this->MWUI->tableWidget->setItem(this->countMain, 2, new QTableWidgetItem(Info.ShortInfo));
	if (this->countMain < this->Rows-1)
	{
	    this->countMain++;
	    this->CopyRemoteFile(this->MWUI->tableWidget->item(this->countMain, 1)->text());
	}
	break;
    }
}

void info::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (this->httpRequestAborted) return;
    /*this->MWUI->progressBar->setMaximum(totalBytes);
    this->MWUI->progressBar->setValue(bytesRead);*/
}

void info::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
