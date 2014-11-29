/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Mon 19. Apr 15:08:16 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLabel *label;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *version;
    QLabel *is_beta;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->setWindowModality(Qt::WindowModal);
        About->resize(580, 210);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(About->sizePolicy().hasHeightForWidth());
        About->setSizePolicy(sizePolicy);
        About->setMinimumSize(QSize(580, 210));
        About->setMaximumSize(QSize(580, 210));
        layoutWidget = new QWidget(About);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 162, 119));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(160, 50));
        label_3->setMaximumSize(QSize(160, 50));
        label_3->setStyleSheet(QString::fromUtf8("background-image: url(:/sts_logo.png);"));

        verticalLayout->addWidget(label_3);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(160, 61));
        label->setMaximumSize(QSize(16777215, 61));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/x-air-team-logo.png);"));

        verticalLayout->addWidget(label);

        layoutWidget1 = new QWidget(About);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(180, 10, 391, 191));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        version = new QLabel(layoutWidget1);
        version->setObjectName(QString::fromUtf8("version"));

        gridLayout->addWidget(version, 0, 1, 1, 1);

        is_beta = new QLabel(layoutWidget1);
        is_beta->setObjectName(QString::fromUtf8("is_beta"));

        gridLayout->addWidget(is_beta, 0, 2, 1, 1);

        textBrowser = new QTextBrowser(layoutWidget1);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setEnabled(true);
        textBrowser->setOpenExternalLinks(true);
        textBrowser->setOpenLinks(true);

        gridLayout->addWidget(textBrowser, 1, 0, 1, 3);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QDialog *About)
    {
        About->setWindowTitle(QApplication::translate("About", "X-CSL-Updater :: \320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\221\320\276\320\273\321\214\321\210\320\265 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\277\320\276 \321\201\321\201\321\213\320\273\320\272\320\265 <a href=\"http://www.steptosky.com\"><span style=\" text-decoration: underline; color:#0000ff;\">www.steptosky.com</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        label_3->setWhatsThis(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\221\320\276\320\273\321\214\321\210\320\265 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\277\320\276 \321\201\321\201\321\213\320\273\320\272\320\265 </span><a href=\"http://www.steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">www.steptosky.com</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\221\320\276\320\273\321\214\321\210\320\265 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\277\320\276 \321\201\321\201\321\213\320\273\320\272\320\265 <a href=\"http://www.x-air.ru\"><span style=\" text-decoration: underline; color:#0000ff;\">www.x-air.ru</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        label->setWhatsThis(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\221\320\276\320\273\321\214\321\210\320\265 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\320\270 \320\277\320\276 \321\201\321\201\321\213\320\273\320\272\320\265 </span><a href=\"http://www.x-air.ru\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">www.x-air.ru</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label->setText(QString());
        label_2->setText(QApplication::translate("About", "X-CSL-Updater ver.: ", 0, QApplication::UnicodeUTF8));
        version->setText(QString());
        is_beta->setText(QString());
        textBrowser->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Copyright \302\251 2009 </span><a href=\"http:steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">StepToSky (</span></a><a href=\"http://steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">http://steptosky.com</span></a><a href=\"http:steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">)</span></a></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margi"
                        "n-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">&amp;</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Copyright \302\251 2009 </span><a href=\"http://team.x-air.ru\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">X-AiR Team (http://team.x-air.ru)</span></a></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">***</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Programming - den_rain (</span><a href=\"den_rain@steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">den_rain@steptosky.com</span></a><span style=\" font-size:"
                        "8pt;\">)</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">***</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">build win32/64 - den_rain  (</span><a href=\"den_rain@steptosky.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">den_rain@steptosky.com</span></a><span style=\" font-size:8pt;\">)</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">build Linux - Saturn  (</span><a href=\"mailto://frizins@gmail.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">frizins@gmail.com</span></a><span style=\" font-size:8pt;\">)</span></p>\n"
"<p align=\"center\" styl"
                        "e=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">build MacOS - TheChoix  (</span><a href=\"mailto://choixer@gmail.com\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">choixer@gmail.com</span></a><span style=\" font-size:8pt;\">)</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">***</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Official site of project X-CSL-Package (</span><a href=\"http://csl.x-air.ru\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">http://csl.x-air.ru</span></a><span style=\" font-size:8pt;\">).</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
