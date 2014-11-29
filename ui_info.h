/********************************************************************************
** Form generated from reading UI file 'info.ui'
**
** Created: Mon 19. Apr 15:08:16 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFO_H
#define UI_INFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_info
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QTextBrowser *textBrowser;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *info)
    {
        if (info->objectName().isEmpty())
            info->setObjectName(QString::fromUtf8("info"));
        info->setWindowModality(Qt::WindowModal);
        info->resize(570, 472);
        info->setMinimumSize(QSize(570, 472));
        info->setMaximumSize(QSize(10000, 10000));
        gridLayout_2 = new QGridLayout(info);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(info);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        textBrowser = new QTextBrowser(info);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setMinimumSize(QSize(550, 400));
        textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        textBrowser->setOpenExternalLinks(true);
        textBrowser->setOpenLinks(true);

        gridLayout->addWidget(textBrowser, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        pushButton = new QPushButton(info);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(info);
        QObject::connect(pushButton, SIGNAL(pressed()), info, SLOT(close()));

        QMetaObject::connectSlotsByName(info);
    } // setupUi

    void retranslateUi(QDialog *info)
    {
        info->setWindowTitle(QApplication::translate("info", "X-CSL-Updater :: Info", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("info", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\274 \320\277\320\260\320\272\320\265\321\202\320\265 <strong>X-CSL</strong>:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        textBrowser->setToolTip(QApplication::translate("info", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\274 \320\277\320\260\320\272\320\265\321\202\320\265 X-CSL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        textBrowser->setWhatsThis(QApplication::translate("info", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\230\320\267 \321\215\321\202\320\276\320\263\320\276 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\276\320\275\320\275\320\276\320\263\320\276 \320\276\320\272\320\275\320\260 \320\262\321\213 \320\274\320\276\320\266\320\265\321\202\320\265 \321\203\320\267\320\275\320\260\321\202\321\214 \320\261\320\276\320\273\321\214\321\210\320\265 \320\276 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\274 \320\277\320\260\320\272\320\265\321\202\320\265 <span style=\" font-weight:600;\">X-CSL</span>.</p>\n"
""
                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\221\320\276\320\273\320\265\320\265 \320\277\320\276\320\264\321\200\320\276\320\261\320\275\320\276 \321\203\320\267\320\275\320\260\321\202\321\214 \320\276 \320\277\320\260\320\272\320\265\321\202\320\260\321\205 X-CSL \320\264\320\273\321\217 X-Plane \320\274\320\276\320\266\320\275\320\276 \321\203\320\267\320\275\320\260\321\202\321\214 \320\275\320\260 \321\201\320\260\320\271\321\202\320\265 <span style=\" font-weight:600;\">X-CSL Package</span>.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        pushButton->setToolTip(QApplication::translate("info", "\320\235\320\260\320\266\320\274\320\270\321\202\320\265 \320\264\320\273\321\217 \320\267\320\260\320\272\321\200\321\213\321\202\320\270\321\217 \320\276\320\272\320\275\320\260", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        pushButton->setWhatsThis(QApplication::translate("info", "\320\235\320\260\320\266\320\274\320\270\321\202\320\265 \320\264\320\273\321\217 \320\267\320\260\320\272\321\200\321\213\321\202\320\270\321\217 \320\276\320\272\320\275\320\260", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        pushButton->setText(QApplication::translate("info", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class info: public Ui_info {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFO_H
