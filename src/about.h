#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"
#include <QtGui/QDialog>
#include "types.h"

namespace Ui {
    class About;
}

class About : public QDialog {
    Q_OBJECT
public:
    About(QWidget *parent = 0);
    ~About();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::About *m_ui;
};

#endif // ABOUT_H
