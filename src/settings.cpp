#include "settings.h"
#include "ui_settings.h"
#include "types.h"

Settings::Settings(QWidget * parent)
    :
    QDialog(parent),
    m_ui(new Ui::Settings) {
    m_ui->setupUi(this);

    m_ui->tab_2->setEnabled(false);

    this->LoadSettings();

    connect(this->m_ui->CancelButton, SIGNAL(pressed()), this, SLOT(CancelSlot()));
    connect(this->m_ui->OkButton, SIGNAL(pressed()), this, SLOT(OkSlot()));
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

void Settings::LoadSettings() {
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    // http://csl-updater.loc/CSL-Package/
    this->m_ui->server1->setText(settings.value("DefaultServer", tr("http://csl.x-air.ru/package/")).toString());
    this->m_ui->server2->setText(settings.value("Server2").toString());
    this->m_ui->server3->setText(settings.value("Server3").toString());
    this->m_ui->server4->setText(settings.value("Server4").toString());
    int serverActive = settings.value("serverActive", 1).toInt();
    switch (serverActive) {
        case 1:
            this->m_ui->checkBox_1->setChecked(true);
            settings.setValue("curServer", settings.value("DefaultServer", tr("http://csl.x-air.ru/package/")).toString());
            break;
        case 2:
            this->m_ui->checkBox_2->setChecked(true);
            settings.setValue("curServer", settings.value("Server2", tr("http://csl.x-air.ru/package/")).toString());
            break;
        case 3:
            this->m_ui->checkBox_3->setChecked(true);
            settings.setValue("curServer", settings.value("Server3", tr("http://csl.x-air.ru/package/")).toString());
            break;
        case 4:
            this->m_ui->checkBox_4->setChecked(true);
            settings.setValue("curServer", settings.value("Server4", tr("http://csl.x-air.ru/package/")).toString());
            break;
        default:
            this->m_ui->checkBox_1->setChecked(true);
            settings.setValue("curServer", settings.value("DefaultServer", tr("http://csl.x-air.ru/package/")).toString());
            break;
    }
    settings.setValue("IndexFile", "x-csl-indexes.idx");
    settings.setValue("IndexForDelFile", "x-csl-indexes-for-delete.idx");
    /*QString PathLang = QDir::currentPath()+tr("/english.qm");
    QFileInfo FInfo(PathLang);
    if (!FInfo.isFile())
    {
	this->m_ui->checkEng->setDisabled(true);
	this->m_ui->checkRus->setChecked(true);
	settings.setValue("lang", "default");
    }*/

    // if (settings.value("lang", "default").toString() == ":/lang/english.qm")
    //     this->m_ui->checkEng->setChecked(true);
    // if (settings.value("lang", "default").toString() == "default")
    //     this->m_ui->checkRus->setChecked(true);

    this->m_ui->checkEng->setChecked(true);
    this->m_ui->checkRus->setChecked(false);
    settings.setValue("lang", "default");
}

void Settings::SaveSettings() {
    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    // if (this->m_ui->checkEng->isChecked()) {
    //     settings.setValue("lang", ":/lang/english.qm");
    // }
    // else {
    //     settings.setValue("lang", "default");
    // }
    settings.setValue("lang", "default");

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
    settings.setValue("IndexFile", tr("x-csl-indexes.idx"));
    settings.setValue("IndexForDelFile", "x-csl-indexes-for-delete.idx");
    if (this->m_ui->checkBox_1->isChecked()) {
        settings.setValue("serverActive", 1);
        settings.setValue("curServer", settings.value("DefaultServer", tr("http://csl.x-air.ru/package/")).toString());
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