#include "base_steps.h"

BaseSteps::BaseSteps(QWidget * _MW, Ui::MainWindow * _MWUI, const QString & targetDir, const QString & targetCslDir)
    : QWidget(nullptr),
      mTargetDir(targetDir),
      mTargetCslDir(targetCslDir) {
    //-------------------------------------------------------------------------
    this->MW = _MW;
    this->MWUI = _MWUI;
}

BaseSteps::~BaseSteps() { }

void BaseSteps::SetMessage(QString msg) const {
    this->MWUI->listWidget->addItem(msg);
    this->MWUI->listWidget->scrollToBottom();
}

void BaseSteps::InitProgBar(int inStart, int inEnd, int inCurrent, int inStep) {
    this->step = inStep;
    this->current = inCurrent;
    this->MWUI->progressBar->setRange(inStart, inEnd);
    this->MWUI->progressBar->setValue(inCurrent);
}

void BaseSteps::StepProgBar() {
    int const result = this->current + this->step;
    this->MWUI->progressBar->setValue(result);
    this->current = result;
}

void BaseSteps::SetValProgBar(int value) const {
    this->MWUI->progressBar->setValue(value);
}
