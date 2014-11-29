/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSetFolder;
    QAction *actionSettings;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionSet_Custom_Path;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBoxLogo;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLabel *curPathLabel;
    QPushButton *ButtonSetFolder;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QProgressBar *progressBar;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *SelAllButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *CancelButton;
    QPushButton *PrevButton;
    QPushButton *NextButton;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(810, 605);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(810, 605));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        MainWindow->setBaseSize(QSize(810, 605));
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/xcsl_logo_64.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        MainWindow->setDocumentMode(false);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        actionSetFolder = new QAction(MainWindow);
        actionSetFolder->setObjectName(QString::fromUtf8("actionSetFolder"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionSettings->setEnabled(true);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionSet_Custom_Path = new QAction(MainWindow);
        actionSet_Custom_Path->setObjectName(QString::fromUtf8("actionSet_Custom_Path"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setContextMenuPolicy(Qt::NoContextMenu);
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBoxLogo = new QGroupBox(centralWidget);
        groupBoxLogo->setObjectName(QString::fromUtf8("groupBoxLogo"));
        groupBoxLogo->setMinimumSize(QSize(160, 160));
        groupBoxLogo->setMaximumSize(QSize(160, 160));
        groupBoxLogo->setStyleSheet(QString::fromUtf8("background-image: url(:/xcsl_logo.png);"));

        horizontalLayout_2->addWidget(groupBoxLogo);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 160));
        groupBox_2->setMaximumSize(QSize(16777215, 160));
        groupBox_2->setContextMenuPolicy(Qt::NoContextMenu);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(100, 15));
        label_5->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout->addWidget(label_5);

        curPathLabel = new QLabel(groupBox_2);
        curPathLabel->setObjectName(QString::fromUtf8("curPathLabel"));
        curPathLabel->setMaximumSize(QSize(16777215, 15));
        curPathLabel->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout->addWidget(curPathLabel);

        ButtonSetFolder = new QPushButton(groupBox_2);
        ButtonSetFolder->setObjectName(QString::fromUtf8("ButtonSetFolder"));
        ButtonSetFolder->setMaximumSize(QSize(50, 20));

        horizontalLayout->addWidget(ButtonSetFolder);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listWidget = new QListWidget(groupBox_2);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(0, 81));
        listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setProperty("showDropIndicator", QVariant(false));
        listWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);

        verticalLayout->addWidget(listWidget);

        progressBar = new QProgressBar(groupBox_2);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setContextMenuPolicy(Qt::NoContextMenu);
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox_2);


        gridLayout_3->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 351));
        groupBox_3->setContextMenuPolicy(Qt::NoContextMenu);
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tableWidget = new QTableWidget(groupBox_3);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setMinimumSize(QSize(0, 315));
        tableWidget->setMaximumSize(QSize(10000, 10000));
        tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        tableWidget->setEditTriggers(QAbstractItemView::EditKeyPressed);
        tableWidget->setTabKeyNavigation(true);
        tableWidget->setProperty("showDropIndicator", QVariant(false));
        tableWidget->setDragDropOverwriteMode(false);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setShowGrid(false);
        tableWidget->setSortingEnabled(false);
        tableWidget->setWordWrap(true);
        tableWidget->setCornerButtonEnabled(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setDefaultSectionSize(16);

        gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_3, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        SelAllButton = new QPushButton(centralWidget);
        SelAllButton->setObjectName(QString::fromUtf8("SelAllButton"));
        SelAllButton->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout_3->addWidget(SelAllButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        CancelButton = new QPushButton(centralWidget);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));
        CancelButton->setEnabled(false);

        horizontalLayout_3->addWidget(CancelButton);

        PrevButton = new QPushButton(centralWidget);
        PrevButton->setObjectName(QString::fromUtf8("PrevButton"));
        PrevButton->setEnabled(true);
        PrevButton->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout_3->addWidget(PrevButton);

        NextButton = new QPushButton(centralWidget);
        NextButton->setObjectName(QString::fromUtf8("NextButton"));
        NextButton->setEnabled(false);
        NextButton->setContextMenuPolicy(Qt::NoContextMenu);
        NextButton->setAutoRepeat(false);

        horizontalLayout_3->addWidget(NextButton);


        gridLayout_3->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 810, 18));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionSetFolder);
        menu->addAction(actionSettings);
        menu->addAction(actionSet_Custom_Path);
        menu->addSeparator();
        menu->addAction(actionExit);
        menu_2->addAction(actionAbout);
        menu_2->addAction(actionAbout_Qt);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "X-CSL-Updater", 0, QApplication::UnicodeUTF8));
        actionSetFolder->setText(QApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270", 0, QApplication::UnicodeUTF8));
        actionSetFolder->setIconText(QApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSetFolder->setToolTip(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\277\321\203\321\202\321\214 \320\264\320\273\321\217 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 X-CSL-Package", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        actionSetFolder->setWhatsThis(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \320\277\321\203\321\202\321\214 \320\264\320\273\321\217 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 X-CSL-Package", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionSetFolder->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSettings->setToolTip(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        actionSettings->setWhatsThis(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAbout->setToolTip(QApplication::translate("MainWindow", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276\320\261 \321\215\321\202\320\276\320\271 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        actionAbout->setWhatsThis(QApplication::translate("MainWindow", "\320\230\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217 \320\276\320\261 \321\215\321\202\320\276\320\271 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionAbout->setShortcut(QApplication::translate("MainWindow", "Ctrl+/", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionSet_Custom_Path->setText(QApplication::translate("MainWindow", "Set Custom Path", 0, QApplication::UnicodeUTF8));
        actionSet_Custom_Path->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+O", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        groupBoxLogo->setToolTip(QApplication::translate("MainWindow", "logo", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBoxLogo->setTitle(QString());
        groupBox_2->setTitle(QString());
        label_5->setText(QApplication::translate("MainWindow", "\320\237\321\203\321\202\321\214 \320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270:", 0, QApplication::UnicodeUTF8));
        curPathLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ButtonSetFolder->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\277\321\203\321\202\321\214 \320\264\320\273\321\217 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 \320\277\320\260\320\272\320\265\321\202\320\276\320\262 <span style=\" font-weight:600;\">X-CSL</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ButtonSetFolder->setText(QApplication::translate("MainWindow", "\320\236\320\261\320\267\320\276\321\200", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        listWidget->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\233\320\276\320\263 \321\200\320\260\320\261\320\276\321\202\321\213 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        listWidget->setWhatsThis(QApplication::translate("MainWindow", "\320\222\321\213 \320\274\320\276\320\266\320\265\321\202\320\265 \320\276\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \320\273\320\276\320\263, \320\262\321\213\320\261\321\200\320\260\320\262 \320\262 \320\272\320\276\320\275\321\202\320\265\320\272\321\201\321\202\320\275\320\276\320\274 \320\274\320\265\320\275\321\216 \320\277\321\203\320\275\320\272\321\202 \"\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        progressBar->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\277\320\260\320\272\320\265\321\202\320\276\320\262 X-CSL, \320\264\320\276\321\201\321\202\321\203\320\277\320\275\321\213\321\205 \320\275\320\260 \321\201\320\265\321\200\320\262\320\265\321\200\320\265:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "#ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "\320\230\320\275\321\204\320\276", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\320\222\320\265\321\200\321\201\320\270\321\217", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 (MB)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\321\203\321\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\264", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tableWidget->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\222\321\213\320\264\320\265\320\273\320\270\321\202\320\265 \320\275\320\265\320\276\320\261\321\205\320\276\320\264\320\270\320\274\321\213\320\265 \320\277\320\260\320\272\320\265\321\202\321\213 </span><span style=\" font-size:8pt; font-weight:600;\">X-CSL</span><span style=\" font-size:8pt;\"> \320\270 \320\275\320\260\320\266\320\274\320\270\321\202\320\265 \320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SelAllButton->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">\320\222\321\213\320\264\320\265\320\273\320\270\321\202\321\214 \320\277\320\260\320\272\320\265\321\202\321\213 </span><span style=\" font-size:8pt; font-weight:600;\">X-CSL</span><span style=\" font-size:8pt;\"> \320\262 \321\201\320\277\320\270\321\201\320\272\320\265 (Ctrl+A)</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SelAllButton->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\264\320\265\320\273\320\270\321\202\321\214 \320\222\321\201\320\265", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        CancelButton->setToolTip(QApplication::translate("MainWindow", "\320\237\321\200\320\265\321\200\320\262\320\260\321\202\321\214 \321\202\320\265\320\272\321\203\321\211\320\265\320\265 \320\264\320\265\320\271\321\201\321\202\320\262\320\270\320\265", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        CancelButton->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        PrevButton->setToolTip(QApplication::translate("MainWindow", "\320\230\320\275\320\264\320\265\320\272\321\201\320\260\321\206\320\270\321\217 \320\262\320\260\321\210\320\270\321\205 \321\204\320\260\320\271\320\273\320\276\320\262 (Ctrl+I)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        PrevButton->setText(QApplication::translate("MainWindow", "\320\230\320\275\320\264\320\265\320\272\321\201\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        PrevButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        NextButton->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\276\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\320\265 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\275\321\213\321\205 \320\277\320\260\320\272\320\265\321\202\320\276\320\262 <span style=\" font-weight:600;\">X-CSL</span> (Ctrl+U)</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        NextButton->setText(QApplication::translate("MainWindow", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", 0, QApplication::UnicodeUTF8));
        NextButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        menu_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\237\320\276\320\274\320\276\321\211\321\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
