#ifndef BASE_STEPS_H
#define BASE_STEPS_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "types.h"
#include <QtNetwork>

class BaseSteps : public QWidget
{
	Q_OBJECT
public:
	QWidget *MW;
	Ui::MainWindow *MWUI;
	QString separator;
	QString FolderName;
	/*QHttp *http;
	QFile *file;
	int httpGetId;
	bool httpRequestAborted;*/
	struct FilesTypes
	{
		int ID;
		QStringList List;
		int State;
	};
	QVector<FilesTypes> FilesList;

	BaseSteps(QWidget *_MW, Ui::MainWindow *_MWUI);
	~BaseSteps();
	void SetMessage(QString Msg);
	void InitProgBar(int start, int end, int current = 0, int step = 1);
	void StepProgBar();
	void SetValProgBar(int value);
	//void CopyRemoteFile(QString From, QString To);

	/*private slots:
		void httpRequestFinished(int requestId, bool error);
		void readResponseHeader(const QHttpResponseHeader &responseHeader);
		void updateDataReadProgress(int bytesRead, int totalBytes);*/


private:
	int step;
	int current;
	/*int DownSize;
	int TotalDownSize;*/
};

#endif // BASE_STEPS_H
