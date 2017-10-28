#include "base_steps.h"

BaseSteps::BaseSteps(QWidget *_MW, Ui::MainWindow *_MWUI): QWidget(0)
{
    this->MW = _MW;
    this->MWUI = _MWUI;
    this->separator = "/"/*(QString)QDir::separator()*/;
    QSettings settings(ORGANISATION, PROGRAM_NAME);
    this->FolderName = settings.value("FolderName").toString();
}

BaseSteps::~BaseSteps()
{

}

void BaseSteps::SetMessage(QString Msg)
{
    this->MWUI->listWidget->addItem(Msg);
    this->MWUI->listWidget->scrollToBottom();
}

void BaseSteps::InitProgBar(int start, int end, int current, int step)
{
    //this->MWUI->progressBar->setMaximum();
    //this->MWUI->progressBar->setMinimum();
    this->step = step;
    this->current = current;
    this->MWUI->progressBar->setRange(start, end);
    this->MWUI->progressBar->setValue(current);
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



