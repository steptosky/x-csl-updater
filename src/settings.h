#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>

namespace Ui {
    class Settings;
}

class Settings : public QDialog {
    Q_OBJECT
public:
    Settings(QWidget *parent = 0);
    ~Settings();
    void LoadSettings();
    void SaveSettings();

protected:
    void changeEvent(QEvent *e);

private slots:
    void OkSlot();
    void CancelSlot();

private:
    Ui::Settings *m_ui;
};

#endif // SETTINGS_H
