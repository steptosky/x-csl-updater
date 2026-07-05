// SPDX-License-Identifier: MPL-2.0

#include "about.h"

About::About(QWidget * parent)
    :
    QDialog(parent),
    m_ui(new Ui::About) {

    m_ui->setupUi(this);
    setVersionInfo();

    m_ui->copyrights->setText(STS_XCSL_COPYRIGHT_XAR);
    m_ui->copyrights_2->setText(STS_XCSL_COPYRIGHT_STS);

    m_ui->license->setText("");
    m_ui->license_2->setText(STS_XCSL_LICENSE_TYPE);
}

About::~About() {
    delete m_ui;
}

void About::setVersionInfo() const {
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
