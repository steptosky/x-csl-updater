#include "base_steps.h"

BaseSteps::BaseSteps(QWidget * _MW, Ui::MainWindow * _MWUI)
    : QWidget(nullptr) {
    //-------------------------------------------------------------------------
    this->MW = _MW;
    this->MWUI = _MWUI;
}

BaseSteps::~BaseSteps() { }

void BaseSteps::setMessage(QString msg) const {
    this->MWUI->listWidget->addItem(msg);
    this->MWUI->listWidget->scrollToBottom();
    qInfo() << msg;
}

void BaseSteps::initProgBar(int inStart, int inEnd, int inCurrent, int inStep) {
    this->mStep = inStep;
    this->mCurrent = inCurrent;
    this->MWUI->progressBar->setRange(inStart, inEnd);
    this->MWUI->progressBar->setValue(inCurrent);
}

void BaseSteps::stepProgBar() {
    int const result = this->mCurrent + this->mStep;
    this->MWUI->progressBar->setValue(result);
    this->mCurrent = result;
}

void BaseSteps::setValProgBar(int value) const {
    this->MWUI->progressBar->setValue(value);
}
