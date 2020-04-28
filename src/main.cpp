#include <QtGui>
#include "mainwindow.h"

QScopedPointer<QFile> gLogFile;

void myMessageOutput(QtMsgType type, const QMessageLogContext & context, const QString & msg) {
    QTextStream logFile(gLogFile.data());
    const QByteArray localMsg = msg.toUtf8();
    const char * file = context.file ? context.file : "";
    const char * function = context.function ? context.function : "";
    const QString category = "";
    const char * categoryC = "";
    switch (type) {
        case QtDebugMsg:
            if (MainWindow::mIsLogVerbose){
                fprintf(stdout, "[DEBUG]%s %s\n", categoryC, localMsg.constData());
                logFile << QString("[DEBUG]%1 %2\n").arg(category).arg(localMsg.constData());
            }
            break;
        case QtInfoMsg:
            fprintf(stdout, "[INFO]%s %s\n", categoryC, localMsg.constData());
            logFile << QString("[INFO]%1 %2\n").arg(category).arg(localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stdout, "[WARNING]%s %s\n", categoryC, localMsg.constData());
            logFile << QString("[WARNING]%1 %2\n").arg(category).arg(localMsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stdout, "[CRITICAL]%s %s (%s:%u, %s)\n", categoryC, localMsg.constData(), file, context.line, function);
            fprintf(stderr, "[CRITICAL]%s %s (%s:%u, %s)\n", categoryC, localMsg.constData(), file, context.line, function);
            logFile << QString("[CRITICAL]%1 %2 (%3:%4)\n").arg(category).arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
        case QtFatalMsg:
            fprintf(stdout, "[FATAL]%s %s (%s:%u, %s)\n", categoryC, localMsg.constData(), file, context.line, function);
            fprintf(stderr, "[FATAL]%s %s (%s:%u, %s)\n", categoryC, localMsg.constData(), file, context.line, function);
            logFile << QString("[FATAL]%1 %2 (%3:%4)\n").arg(category).arg(localMsg.constData()).arg(file).arg(context.line).arg(function);
            break;
    }
    logFile.flush();
}

int main(int argc, char * argv[]) {
    QApplication a(argc, argv);
    gLogFile.reset(new QFile("log.txt"));
    if (!gLogFile->open(QFile::WriteOnly)) {
        QMessageBox::critical(nullptr, PROGRAM_NAME + QApplication::tr(" :: ERROR!"),
                              QApplication::tr("Cannot open log file: <log.txt>"), QMessageBox::Ok);
        return 1;
    }
    qInstallMessageHandler(myMessageOutput);
    qInfo() << PROGRAM_NAME;
    qInfo() << STS_XCSL_ORGANIZATION_NAME;
    qInfo() << STS_XCSL_PROJECT_WEBLINK;
    qInfo() << "Version: " << gProgramVersion;
    qInfo() << "-------------------------------";

    QSettings settings(gSettingsFileName, QSettings::IniFormat);
    QString lang;
    if (settings.value("first", "no").toString() == "no") {
        const QLocale locale;
        if (locale.name() == "ru_RU") {
            settings.setValue("lang", "ru");
        }
        else {
            settings.setValue("lang", "en");
        }
    }
    settings.setValue("first", "yes");

    MainWindow w;
    w.show();
    const int res = QApplication::exec();
    qInfo() << "Will exit with code: " << res;
    gLogFile->close();
    return res;
}
