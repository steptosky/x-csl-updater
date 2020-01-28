#ifndef BASE_STEPS_H
#define BASE_STEPS_H

#include <QtGui>
#include "ui_mainwindow.h"
#include "Definitions.h"
#include <QtNetwork>

class BaseSteps : public QWidget
{
	Q_OBJECT
public:
	QWidget *MW;
	Ui::MainWindow *MWUI;

	QString mCslFolderName;
    QString mTargetDir;

    struct PackageEntry
	{
		int ID;
		QStringList data;
		eFileState state;
	};
	QVector<PackageEntry> mEntryList;

	BaseSteps(QWidget *_MW, Ui::MainWindow *_MWUI, const QString & targetDir, const QString & targetCslDir);
	~BaseSteps();
	void SetMessage(QString Msg);
	void InitProgBar(int start, int end, int current = 0, int step = 1);
	void StepProgBar();
	void SetValProgBar(int value);

private:
	int step;
	int current;
};

#endif // BASE_STEPS_H
