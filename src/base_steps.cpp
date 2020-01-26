#include "base_steps.h"

BaseSteps::BaseSteps(QWidget *_MW, Ui::MainWindow *_MWUI): QWidget(0)
{
    this->MW = _MW;
    this->MWUI = _MWUI;
    this->mSeparator = "/"/*(QString)QDir::separator()*/;
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    this->mCslFolderName = settings.value("FolderName").toString();
}

BaseSteps::~BaseSteps()
{

}

void BaseSteps::SetMessage(QString Msg)
{
    this->MWUI->listWidget->addItem(Msg);
    this->MWUI->listWidget->scrollToBottom();
}

void BaseSteps::InitProgBar(int inStart, int inEnd, int inCurrent, int inStep)
{
    //this->MWUI->progressBar->setMaximum();
    //this->MWUI->progressBar->setMinimum();
    this->step = inStep;
    this->current = inCurrent;
    this->MWUI->progressBar->setRange(inStart, inEnd);
    this->MWUI->progressBar->setValue(inCurrent);
}

void BaseSteps::StepProgBar()
{
    int result;
    result = this->current + this->step;
    this->MWUI->progressBar->setValue(result);
    this->current = result;
}

void BaseSteps::SetValProgBar(int value)
{
    this->MWUI->progressBar->setValue(value);
}



