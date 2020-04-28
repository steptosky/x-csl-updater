#include "about.h"

About::About(QWidget * parent)
    :
    QDialog(parent),
    m_ui(new Ui::About) {

    m_ui->setupUi(this);
    setVersionInfo();
}

About::~About() {
    delete m_ui;
}

void About::setVersionInfo() {
    m_ui->versionLabel->setText(QString(PROGRAM_NAME) + " Ver.:");
    m_ui->version->setText(gProgramVersion);
}

void About::changeEvent(QEvent * e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            setVersionInfo();
            break;
        default:
            break;
    }
}
