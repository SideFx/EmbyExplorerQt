//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Main window
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QTableView>
#include <QCloseEvent>
#include "preferencesdialog.h"
#include "detailsdialog.h"
#include "dispatcher.h"
#include "splashdialog.h"
#include "export.h"
#include "messagedialog.h"
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void saveSettings();
    void loadSettings();
    bool checkSettings();
    void adjustTableView();
    void initTableView(QString collectionType);
    void displayDetails();
    void showSplashScreen();
    PreferencesDialog preferencesDialog;
    SplashDialog splashDialog;
    DetailsDialog detailsDialog;
    embySettings embySettings;
    QComboBox *cbxCollection;
    QLabel *lblStatistics;
    Dispatcher *dis;
    Export *exp;
    QVector<UserCollectionType> collections;
    QVector<DetailsDataType> detailsBuffer;
    bool detailsVisible;
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent *e);
    void centerDialog(QDialog& dlg);
    QString const FILE_FILTER = tr("Excel Arbeitsmappe (*.xlsx)");
    MessageDialog messageDialog;
    AboutDialog aboutDialog;

    QString const CAP_MOVIES_STATS = tr("Filme: %1");
    QString const CAP_SERIES_STATS = tr("Serien: %1, Episoden: %2");
    QString const CAP_VIDEOS_STATS = tr("Videos: %1");

private slots:
    void onActionQuit();
    void onActionSettings();
    void onActionAuthenticate();
    void onActionFetch();
    void onActionDetails();
    void onActionExport();
    void onActionAboutEmbyExplorer();
    void onCollectionChanged(int);
    void onItemSelectionChanged();  

};

