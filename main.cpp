//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Main
// last change: 20251128
//-----------------------------------------------------------------------------------------------------------
#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    bool load = false;
    QApplication a(argc, argv);
#if defined (Q_OS_WIN)
    a.setStyle(QStyleFactory::create("fusion"));
#elif defined (Q_OS_MAC)
    a.setStyle(QStyleFactory::create("macos"));
#endif
    QString i18nPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    QTranslator translator;
    if (translator.load(i18nPath + "qt_en.qm")) a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
