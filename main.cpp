//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Main
//-----------------------------------------------------------------------------------------------------------
#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #if defined(Q_OS_WIN)
        a.setStyle(QStyleFactory::create("windowsvista"));
    #elif defined(Q_OS_MAC)
        a.setStyle(QStyleFactory::create("fusion"));
    #endif
    if (QLocale::system().name().startsWith("en_", Qt::CaseSensitive)) {
        QTranslator translator;
        if (translator.load("EmbyExplorerQt_en_US.qm")) a.installTranslator(&translator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
