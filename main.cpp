#include "widget_main.h"
#include <QApplication>

#include "version.h"

int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("SCON");
    QCoreApplication::setOrganizationDomain("scon.pro");
    QCoreApplication::setApplicationName("Hydroponic system configurator");
    QCoreApplication::setApplicationVersion(VERSION);

    QTranslator qtTranslator;
    qtTranslator.load("qtbase_ru" /* + QLocale::system().name() */, qApp->applicationDirPath());
    a.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load("hponic_ru" /* + QLocale::system().name() */, qApp->applicationDirPath());
    a.installTranslator(&myappTranslator);

    WidgetMain w;
    w.show();

    return a.exec();
}
