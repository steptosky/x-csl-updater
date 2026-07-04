// SPDX-License-Identifier: MPL-2.0

#include "settings.h"
#include "ui_settings.h"
#include "Definitions.h"

// http://csl-updater.loc/CSL-Package/
static const QString sDefaultServer = "http://x-csl.ru/package/";

Settings::Settings(QWidget * parent)
    :
    QDialog(parent),
    m_ui(new Ui::Settings) {
    m_ui->setupUi(this);

    //m_ui->tab_2->setEnabled(false);

    this->LoadSettings();

    connect(this->m_ui->cancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));
    connect(this->m_ui->OkButton, SIGNAL(pressed()), this, SLOT(OkSlot()));

    connect(m_ui->checkRus, &QCheckBox::stateChanged, this, &Settings::LangChangedSlot);
    connect(m_ui->checkEng, &QCheckBox::stateChanged, this, &Settings::LangChangedSlot);

    setupTranslator();
}

Settings::~Settings() {
    delete m_ui;
}

void Settings::OkSlot() {
    this->SaveSettings();
    this->close();
}

void Settings::CancelSlot() {
    this->close();
}

void Settings::LangChangedSlot(int newState) {
    if (newState){
        QSettings settings(settingsFileName(), QSettings::IniFormat);
        if (this->m_ui->checkRus->isChecked()) {
            settings.setValue("lang", "ru");
        }
        else {
            settings.setValue("lang", "en");
        }
        setupTranslator();
    }
}

void Settings::setupTranslator() {
    const QSettings settings(settingsFileName(), QSettings::IniFormat);
    QApplication::removeTranslator(mTranslator);
    delete mTranslator;
    mTranslator = new QTranslator();
    mTranslator->load(translationFileName(settings.value("lang", "en").toString()));
    QApplication::installTranslator(mTranslator);
}

void Settings::LoadSettings() {
    QSettings settings(settingsFileName(), QSettings::IniFormat);
    this->m_ui->server1->setText(settings.value("DefaultServer", sDefaultServer).toString());
    this->m_ui->server2->setText(settings.value("Server2").toString());
    this->m_ui->server3->setText(settings.value("Server3").toString());
    this->m_ui->server4->setText(settings.value("Server4").toString());
    int serverActive = settings.value("serverActive", 1).toInt();
    switch (serverActive) {
        case 1:
            this->m_ui->checkBox_1->setChecked(true);
            settings.setValue("curServer", settings.value("DefaultServer", sDefaultServer).toString());
            break;
        case 2:
            this->m_ui->checkBox_2->setChecked(true);
            settings.setValue("curServer", settings.value("Server2", sDefaultServer).toString());
            break;
        case 3:
            this->m_ui->checkBox_3->setChecked(true);
            settings.setValue("curServer", settings.value("Server3", sDefaultServer).toString());
            break;
        case 4:
            this->m_ui->checkBox_4->setChecked(true);
            settings.setValue("curServer", settings.value("Server4", sDefaultServer).toString());
            break;
        default:
            this->m_ui->checkBox_1->setChecked(true);
            settings.setValue("curServer", settings.value("DefaultServer", sDefaultServer).toString());
            break;
    }
    //
    if (settings.value("lang", "en").toString() == "ru") {
        this->m_ui->checkRus->setChecked(true);
    }
    if (settings.value("lang", "en").toString() == "en") {
        this->m_ui->checkEng->setChecked(true);
    }
}

void Settings::SaveSettings() {
    QSettings settings(settingsFileName(), QSettings::IniFormat);
    if (this->m_ui->checkRus->isChecked()) {
        settings.setValue("lang", "ru");
    }
    else {
        settings.setValue("lang", "en");
    }

    //servers
    QString _server = this->m_ui->server2->text();
    if (!_server.endsWith("/"))
        _server += "/";
    settings.setValue("Server2", _server);
    _server = this->m_ui->server3->text();
    if (!_server.endsWith("/"))
        _server += "/";
    settings.setValue("Server3", _server);
    _server = this->m_ui->server4->text();
    if (!_server.endsWith("/"))
        _server += "/";
    settings.setValue("Server4", _server);
    if (this->m_ui->checkBox_1->isChecked()) {
        settings.setValue("serverActive", 1);
        settings.setValue("curServer", settings.value("DefaultServer", sDefaultServer).toString());
    }
    else if (this->m_ui->checkBox_2->isChecked()) {
        settings.setValue("serverActive", 2);
        settings.setValue("curServer", this->m_ui->server2->text());
    }
    else if (this->m_ui->checkBox_3->isChecked()) {
        settings.setValue("serverActive", 3);
        settings.setValue("curServer", this->m_ui->server3->text());
    }
    else if (this->m_ui->checkBox_4->isChecked()) {
        settings.setValue("serverActive", 4);
        settings.setValue("curServer", this->m_ui->server4->text());
    }
}

void Settings::changeEvent(QEvent * e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}