/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *server1;
    QCheckBox *checkBox_1;
    QLabel *label_2;
    QLineEdit *server2;
    QCheckBox *checkBox_2;
    QLabel *label_3;
    QLineEdit *server3;
    QCheckBox *checkBox_3;
    QLabel *label_4;
    QLineEdit *server4;
    QCheckBox *checkBox_4;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_3;
    QWidget *tab_2;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_4;
    QCheckBox *checkRus;
    QCheckBox *checkEng;
    QSpacerItem *horizontalSpacer;
    QPushButton *CancelButton;
    QPushButton *OkButton;
    QButtonGroup *CheckGroup;
    QButtonGroup *LangGroup;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->setWindowModality(Qt::WindowModal);
        Settings->resize(540, 240);
        Settings->setMinimumSize(QSize(540, 240));
        Settings->setMaximumSize(QSize(540, 240));
        gridLayout_5 = new QGridLayout(Settings);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(Settings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        layoutWidget = new QWidget(tab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 9, 491, 111));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        server1 = new QLineEdit(layoutWidget);
        server1->setObjectName(QString::fromUtf8("server1"));
        server1->setEnabled(false);
        server1->setReadOnly(true);

        gridLayout_2->addWidget(server1, 0, 1, 1, 1);

        checkBox_1 = new QCheckBox(layoutWidget);
        CheckGroup = new QButtonGroup(Settings);
        CheckGroup->setObjectName(QString::fromUtf8("CheckGroup"));
        CheckGroup->addButton(checkBox_1);
        checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));

        gridLayout_2->addWidget(checkBox_1, 0, 2, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        server2 = new QLineEdit(layoutWidget);
        server2->setObjectName(QString::fromUtf8("server2"));

        gridLayout_2->addWidget(server2, 1, 1, 1, 1);

        checkBox_2 = new QCheckBox(layoutWidget);
        CheckGroup->addButton(checkBox_2);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout_2->addWidget(checkBox_2, 1, 2, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        server3 = new QLineEdit(layoutWidget);
        server3->setObjectName(QString::fromUtf8("server3"));

        gridLayout_2->addWidget(server3, 2, 1, 1, 1);

        checkBox_3 = new QCheckBox(layoutWidget);
        CheckGroup->addButton(checkBox_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        gridLayout_2->addWidget(checkBox_3, 2, 2, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        server4 = new QLineEdit(layoutWidget);
        server4->setObjectName(QString::fromUtf8("server4"));

        gridLayout_2->addWidget(server4, 3, 1, 1, 1);

        checkBox_4 = new QCheckBox(layoutWidget);
        CheckGroup->addButton(checkBox_4);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

        gridLayout_2->addWidget(checkBox_4, 3, 2, 1, 1);

        layoutWidget1 = new QWidget(tab);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 130, 491, 22));
        gridLayout_3 = new QGridLayout(layoutWidget1);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        layoutWidget2 = new QWidget(tab_2);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(20, 10, 471, 51));
        gridLayout_4 = new QGridLayout(layoutWidget2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        checkRus = new QCheckBox(layoutWidget2);
        LangGroup = new QButtonGroup(Settings);
        LangGroup->setObjectName(QString::fromUtf8("LangGroup"));
        LangGroup->addButton(checkRus);
        checkRus->setObjectName(QString::fromUtf8("checkRus"));

        gridLayout_4->addWidget(checkRus, 0, 0, 1, 1);

        checkEng = new QCheckBox(layoutWidget2);
        LangGroup->addButton(checkEng);
        checkEng->setObjectName(QString::fromUtf8("checkEng"));

        gridLayout_4->addWidget(checkEng, 1, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        CancelButton = new QPushButton(Settings);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));

        gridLayout->addWidget(CancelButton, 1, 1, 1, 1);

        OkButton = new QPushButton(Settings);
        OkButton->setObjectName(QString::fromUtf8("OkButton"));

        gridLayout->addWidget(OkButton, 1, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Settings);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "X-CSL-Updater :: Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabWidget->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("Settings", "\320\241\320\265\321\200\320\262\320\265\321\200 1:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_1->setToolTip(QApplication::translate("Settings", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\320\271 \320\263\320\260\320\273\320\276\321\207\320\272\320\276\320\271 \321\201\320\265\321\200\320\262\320\265\321\200, \320\261\321\203\320\264\320\265\321\202 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\201\321\217 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\276\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_1->setText(QApplication::translate("Settings", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202\321\201\321\217", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Settings", "\320\241\320\265\321\200\320\262\320\265\321\200 2:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        server2->setToolTip(QApplication::translate("Settings", "\320\220\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 #2", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        server2->setWhatsThis(QApplication::translate("Settings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\260\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 /\320\236\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217 X-CSL-Package...</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        checkBox_2->setToolTip(QApplication::translate("Settings", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\320\271 \320\263\320\260\320\273\320\276\321\207\320\272\320\276\320\271 \321\201\320\265\321\200\320\262\320\265\321\200, \320\261\321\203\320\264\320\265\321\202 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\201\321\217 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\276\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_2->setText(QApplication::translate("Settings", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202\321\201\321\217", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Settings", "\320\241\320\265\321\200\320\262\320\265\321\200 3:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        server3->setToolTip(QApplication::translate("Settings", "\320\220\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 #3", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        server3->setWhatsThis(QApplication::translate("Settings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\260\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 /\320\236\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217 X-CSL-Package...</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        checkBox_3->setToolTip(QApplication::translate("Settings", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\320\271 \320\263\320\260\320\273\320\276\321\207\320\272\320\276\320\271 \321\201\320\265\321\200\320\262\320\265\321\200, \320\261\321\203\320\264\320\265\321\202 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\201\321\217 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\276\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_3->setText(QApplication::translate("Settings", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202\321\201\321\217", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Settings", "\320\241\320\265\321\200\320\262\320\265\321\200 4:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        server4->setToolTip(QApplication::translate("Settings", "\320\220\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 #4", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        server4->setWhatsThis(QApplication::translate("Settings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\260\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 /\320\236\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217 X-CSL-Package...</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        checkBox_4->setToolTip(QApplication::translate("Settings", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\320\271 \320\263\320\260\320\273\320\276\321\207\320\272\320\276\320\271 \321\201\320\265\321\200\320\262\320\265\321\200, \320\261\321\203\320\264\320\265\321\202 \320\270\321\201\320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\321\201\321\217 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\276\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_4->setText(QApplication::translate("Settings", "\320\230\321\201\320\277\320\276\320\273\321\214\320\267\321\203\320\265\321\202\321\201\321\217", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Settings", "\320\241\320\265\321\200\320\262\320\265\321\200", 0, QApplication::UnicodeUTF8));
        checkRus->setText(QApplication::translate("Settings", "\320\240\321\203\321\201\321\201\320\272\320\270\320\271", 0, QApplication::UnicodeUTF8));
        checkEng->setText(QApplication::translate("Settings", "\320\220\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\270\320\271", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Settings", "\320\257\320\267\321\213\320\272\320\270", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        CancelButton->setToolTip(QApplication::translate("Settings", "\320\235\320\260\320\266\320\274\320\270\321\202\320\265 \320\264\320\273\321\217 \320\276\321\202\320\274\320\265\320\275\321\213 \320\262\320\262\320\265\320\264\320\265\320\275\321\213\321\205 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        CancelButton->setText(QApplication::translate("Settings", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        OkButton->setToolTip(QApplication::translate("Settings", "\320\235\320\260\320\266\320\274\320\270\321\202\320\265 \320\264\320\273\321\217 \320\277\320\276\320\264\321\202\320\262\320\265\321\200\320\266\320\264\320\265\320\275\320\270\321\217 \320\262\320\262\320\265\320\264\320\265\320\275\321\213\321\205 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\320\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        OkButton->setText(QApplication::translate("Settings", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
