#include <QtGui>
#include "mainwindow.h"

int main(int argc, char * argv[]) {
    QApplication a(argc, argv);

    // QSettings settings(gSettingsFileName, QSettings::IniFormat);
    // QString lang;
    // if (settings.value("first", "no").toString() == "no") {
    //     QLocale Loc;
    //     QString local(Loc.name());
    //     if (local == "ru_RU") {
    //         lang = settings.value("lang", "default").toString();
    //         settings.setValue("lang", "default");
    //     }
    //     else {
    //         lang = settings.value("lang", ":/lang/english.qm").toString();
    //         settings.setValue("lang", ":/lang/english.qm");
    //     }
    // }
    // else {
    //     lang = settings.value("lang", "default").toString();
    // }
    // QTranslator translator;
    // translator.load(lang);
    // a.installTranslator(&translator);
    //settings.setValue("first", "yes");

    MainWindow w;
    w.show();
    return a.exec();    
}
