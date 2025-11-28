//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Main window
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardPaths>
#include "jbpreferences.h"
#include <QHeaderView>
#include <QFileDialog>
#include "stylesheets.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
#if defined(Q_OS_WIN)
    ui->toolBar->setStyleSheet(styleToolBar);
#elif defined(Q_OS_MAC)
    ui->toolBar->setStyleSheet(styleToolButton);
    ui->statusBar->setVisible(false);
#endif
    setWindowTitle(QString(APP_NAME) + " " + QString(APP_VERSION));
    ui->toolBar->addAction(ui->actionSettings);
    ui->toolBar->addAction(ui->actionAuthenticate);
    ui->toolBar->addSeparator();
    cbxCollection = new QComboBox;
    cbxCollection->setMinimumWidth(150);
    ui->toolBar->addWidget(cbxCollection);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionFetch);
    ui->toolBar->addAction(ui->actionDetails);
    ui->toolBar->addAction(ui->actionExport);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionAboutEmbyExplorer);
    ui->toolBar->addAction(ui->actionQuit);
    lblStatistics = new QLabel;
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(lblStatistics);
    setCentralWidget(ui->tableView);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onActionQuit);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);
    connect(ui->actionAuthenticate, &QAction::triggered, this, &MainWindow::onActionAuthenticate);
    connect(ui->actionFetch, &QAction::triggered, this, &MainWindow::onActionFetch);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::onActionDetails);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onActionExport);
    connect(ui->actionAboutEmbyExplorer, &QAction::triggered, this, &MainWindow::onActionAboutEmbyExplorer);
    connect(cbxCollection, &QComboBox::currentIndexChanged, this, &MainWindow::onCollectionChanged);
    connect(ui->tableView, &QTableWidget::itemSelectionChanged, this, &MainWindow::onItemSelectionChanged);
    detailsVisible = false;
    ui->actionDetails->setEnabled(false);
    ui->actionExport->setEnabled(false);
    dis = new Dispatcher;
    exp = new Export(ui->tableView);
    loadSettings();
    showSplashScreen();
    if(checkSettings()) dis->EmbyCheckHost(embySettings);
}

MainWindow::~MainWindow() {
    delete exp;
    delete dis;
    delete cbxCollection;
    delete ui;
}

void MainWindow::onActionQuit() {
    close();
}

void MainWindow::onActionSettings() {
    preferencesDialog.setSettings(embySettings);
    if(preferencesDialog.exec() == QDialog::Accepted) {
        embySettings = preferencesDialog.getSettings();
        if (!embySettings.font.isEmpty()) {
            try {
                QFont font;
                font.fromString(embySettings.font);
                ui->tableView->setFont(font);
                adjustTableView();
            }
            catch (...) {}
        }
        if(checkSettings()) {
            if (!dis->EmbyCheckHost(embySettings)) messageDialog.showMessage(this, MSG_ERR14);
        }
    }
}

void MainWindow::onCollectionChanged(int i) {   
    if(i >= 0) {
        UserCollectionType coll = collections[i];
        initTableView(coll.CollectionType);
        lblStatistics->clear();
        if (detailsVisible) {
            detailsVisible = false;
            detailsDialog.hide();
        }
        ui->actionDetails->setChecked(false);
        ui->actionDetails->setEnabled(false);
        ui->actionExport->setEnabled(false);
    }
}

void MainWindow::onActionAuthenticate() {
    int result;
    if(checkSettings()) {
        if (dis->EmbyCheckHost(embySettings)) result = dis->EmbyAuthenticate(embySettings);
        else result = MSG_ERR14;
        if (result == MSG_OK) {
            cbxCollection->clear();
            collections = dis->EmbyGetCollections();
            for (auto& ct : dis->CollectionTypes) {
                for (auto& coll : collections) {
                    if (coll.CollectionType == ct) cbxCollection->addItem(coll.Name);
                }
            }
            if (cbxCollection->count() >= 0) {
                cbxCollection->setCurrentIndex(0);
                splashDialog.close();
            }
        }
        else messageDialog.showMessage(this, result);
    }
}

void MainWindow::onActionFetch() {
    int i = 0; int j;
    QString id, id2;
    int cnt_movies = 0, cnt_series = 0, cnt_episodes = 0, cnt_videos = 0;
    QString stats {};
    DetailsDataType o;
    if (cbxCollection->currentIndex() >= 0) {
        UserCollectionType coll = collections[cbxCollection->currentIndex()];
        QVector<QVariant> collData = dis->EmbyGetCollectionData(coll.Id, coll.CollectionType);
        detailsBuffer.clear();
        for (auto &data : collData) {
            ui->tableView->insertRow(i);
            j = 0;
            if (coll.CollectionType == COLLECTION_MOVIES) {
                MovieDataType m = data.value<MovieDataType>();
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.MovieId));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Name));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.OriginalTitle));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.ProductionYear));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Runtime));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Actors));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Directors));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Studios));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Genres));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Container));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Codecs));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.Resolution));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.DateCreated));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.FileName));
                o = {
                    .ItemId = m.MovieId,
                    .ItemId2 = m.MovieId,
                    .Overview = m.Overview
                };
                cnt_movies++;
                detailsBuffer.append(o);
            }
            else if (coll.CollectionType == COLLECTION_TVSHOWS) {
                SeriesDataType s = data.value<SeriesDataType>();
                if (s.TypeKind == TYPEKIND_EPISODE) {
                    id = s.EpisodeId;
                    id2 = s.SeasonId;
                    cnt_episodes++;
                } else if (s.TypeKind == TYPEKIND_SERIES) {
                    id = s.SeriesId;
                    id2 = s.SeriesId;
                    cnt_series++;
                }
                ui->tableView->setItem(i, j++, new QTableWidgetItem(id));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Name));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Episode));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Season));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.ProductionYear));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Runtime));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Actors));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Studios));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Genres));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Container));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Codecs));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.Resolution));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.DateCreated));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(s.FileName));
                o = {
                    .ItemId = id,
                    .ItemId2 = id2,
                    .Overview = s.Overview
                };
                detailsBuffer.append(o);
            }
            else if (coll.CollectionType == COLLECTION_HOMEVIDEOS) {
                VideoDataType v = data.value<VideoDataType>();
                if (v.TypeKind == TYPEKIND_VIDEO) {
                    id = v.VideoId;
                    cnt_videos++;
                }
                else if (v.TypeKind == TYPEKIND_FOLDER) id = v.FolderId;
                ui->tableView->setItem(i, j++, new QTableWidgetItem(id));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Name));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Folder));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Runtime));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Container));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Codecs));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Resolution));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.DateCreated));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.FileName));
            }
            i++;
        }
        adjustTableView();
        if (coll.CollectionType == COLLECTION_MOVIES) {
            if (cnt_movies > 0) stats = QString(CAP_MOVIES_STATS).arg(cnt_movies);
        } else if (coll.CollectionType == COLLECTION_TVSHOWS) {
            if (cnt_series > 0) stats = QString(CAP_SERIES_STATS).arg(cnt_series).arg(cnt_episodes);
        } else if (coll.CollectionType == COLLECTION_HOMEVIDEOS) {
            if (cnt_videos > 0) stats = QString(CAP_VIDEOS_STATS).arg(cnt_videos);
        }
        lblStatistics->setText(stats);
        if (ui->tableView->children().count() > 0) {
            ui->tableView->selectRow(0);
            ui->actionDetails->setEnabled(coll.CollectionType != COLLECTION_HOMEVIDEOS);
            ui->actionExport->setEnabled(true);
        }
    }
}

void MainWindow::onActionDetails() {
    if (cbxCollection->currentIndex() >= 0) {
        UserCollectionType coll = collections[cbxCollection->currentIndex()];
        detailsVisible = !detailsVisible;
        if (detailsVisible) displayDetails(); else detailsDialog.hide();
    }
}

void MainWindow::onActionExport() {
    if (ui->tableView->rowCount() == 0) return;
    if (cbxCollection->currentIndex() >= 0) {
        UserCollectionType coll = collections[cbxCollection->currentIndex()];
        QVector<ColumnsType> columns = dis->getColumns(coll.CollectionType);
        QFileDialog saveDialog;
        QString preferredFileName = QString(EXPORT_PREFIX) + "_" + coll.Name + "_" +
                                    QDateTime::currentDateTime().date().toString("yyyyMMdd") +
                                    QDateTime::currentDateTime().time().toString("hhmmss") +
                                    QString(EXPORT_SUFFIX);
        QString folder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        saveDialog.setParent(this, Qt::Dialog|
                                   Qt::WindowSystemMenuHint|
                                   Qt::WindowCloseButtonHint|
                                   Qt::WindowTitleHint|
                                   Qt::CustomizeWindowHint);
        saveDialog.setOption(QFileDialog::DontUseNativeDialog, true);
        saveDialog.setDirectory(folder);
        saveDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveDialog.setFileMode(QFileDialog::AnyFile);
        saveDialog.setNameFilter(FILE_FILTER);
        saveDialog.setViewMode(QFileDialog::Detail);
        saveDialog.selectFile(preferredFileName);
        if (saveDialog.exec() != QDialog::Accepted) return;
        QStringList files = saveDialog.selectedFiles();
        QString fileName = files.first();
        int result = exp->ExportCollection(fileName, coll.Name, columns);
        if (result != MSG_OK) messageDialog.showMessage(this, result);
    }
}

void MainWindow::onActionAboutEmbyExplorer() {
    aboutDialog.show();
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    if (splashDialog.isVisible()) centerDialog(splashDialog);
    else if (detailsDialog.isVisible()) centerDialog(detailsDialog);
}

void MainWindow::closeEvent(QCloseEvent *e) {
    saveSettings();
    e->accept();
}

void MainWindow::onItemSelectionChanged() {
    if (detailsVisible) displayDetails();
}

void MainWindow::displayDetails() {
    QString overview {}, itemId2 {};
    QPixmap pix {};
    int row = ui->tableView->currentRow();
    if (row >= 0) {
        QString itemId = ui->tableView->item(row, 0)->text();
        for (auto &d : detailsBuffer) {
            if (d.ItemId == itemId) {
                overview = d.Overview;
                itemId2 = d.ItemId2;
                break;
            }
        }
        if (!itemId2.isEmpty()) {
            QByteArray picData = dis->EmbyGetPrimaryPictureForItem(itemId2);
            if (picData.length() > 0) {
                pix.loadFromData(picData);
            }
        }
        if (overview.isEmpty()) overview = dis->TXT_NA;
        detailsDialog.setParent(this);
        detailsDialog.setDetails(pix, overview);
        centerDialog(detailsDialog);
        detailsDialog.showNormal();
    }
}

void MainWindow::initTableView(QString collectionType) {
    QStringList tableHeaders;
    tableHeaders << "#Id"; // hidden column
    int i = 0;
    QVector<ColumnsType> columns = dis->getColumns(collectionType);
    for (i = 0; i < columns.length(); i++) tableHeaders << columns[i].Caption;
    ui->tableView->clearContents();
    ui->tableView->setRowCount(0);
    ui->tableView->setColumnCount(tableHeaders.count());
    ui->tableView->setHorizontalHeaderLabels(tableHeaders);
    ui->tableView->horizontalHeader()->setStretchLastSection(false);
    ui->tableView->setColumnHidden(0, true);
    // disable sorting for TV shows
    ui->tableView->setSortingEnabled(collectionType != COLLECTION_TVSHOWS);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(collectionType != COLLECTION_TVSHOWS);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    QFont fnt = ui->tableView->horizontalHeader()->font();
    fnt.setBold(true);
    ui->tableView->horizontalHeader()->setFont(fnt);
    adjustTableView();
}

bool MainWindow::checkSettings() {
    return !embySettings.address.isEmpty() && !embySettings.port.isEmpty() &&
           !embySettings.username.isEmpty() && !embySettings.password.isEmpty();
}

void MainWindow::saveSettings() {
    JBPreferences *prefs = new JBPreferences();
    prefs->PushArray(SET_WGEOMETRY, saveGeometry());
    prefs->PushArray(SET_WSTATE, saveState(0));
    prefs->PushBoolean(SET_EMBYHTTPS, embySettings.https);
    prefs->PushString(SET_EMBYADDRESS, embySettings.address);
    prefs->PushString(SET_EMBYPORT, embySettings.port);
    prefs->PushString(SET_EMBYUSERNAME, embySettings.username);
    prefs->PushString(SET_EMBYPASSWORD, embySettings.password);
    prefs->PushNumber(SET_COLLMAXACTORS, embySettings.maxactors);
    prefs->PushNumber(SET_COLLMAXDIRECTORS, embySettings.maxdirectors);
    prefs->PushNumber(SET_COLLMAXSTUDIOS, embySettings.maxstudios);
    prefs->PushNumber(SET_COLLMAXGENRES, embySettings.maxgenres);
    prefs->PushString(SET_OPTIONSFONT, embySettings.font);
    prefs->SavePreferencesToDefaultLocation(SET_COMPANY, APP_NAMEQT);
    delete prefs;
}

void MainWindow::loadSettings() {
    JBPreferences *prefs = new JBPreferences();
    if (prefs->LoadPreferencesFromDefaultLocation(SET_COMPANY, APP_NAMEQT)) {
        restoreGeometry(prefs->PopArray(SET_WGEOMETRY));
        restoreState(prefs->PopArray(SET_WSTATE));
        embySettings = {
            .https = prefs->PopBoolean(SET_EMBYHTTPS),
            .address = prefs->PopString(SET_EMBYADDRESS),
            .port = prefs->PopString(SET_EMBYPORT),
            .username = prefs->PopString(SET_EMBYUSERNAME),
            .password = prefs->PopString(SET_EMBYPASSWORD),
            .maxactors = static_cast<int>(prefs->PopNumber(SET_COLLMAXACTORS)),
            .maxdirectors = static_cast<int>(prefs->PopNumber(SET_COLLMAXDIRECTORS)),
            .maxstudios = static_cast<int>(prefs->PopNumber(SET_COLLMAXSTUDIOS)),
            .maxgenres = static_cast<int>(prefs->PopNumber(SET_COLLMAXGENRES))
        };
        try {
            QFont font;
            embySettings.font = prefs->PopString(SET_OPTIONSFONT);
            if (!embySettings.font.isEmpty()) {
                font.fromString(embySettings.font);
                ui->tableView->setFont(font);
            }
        }
        catch (...) {}
    }
    delete prefs;
}

void MainWindow::adjustTableView() {
    ui->tableView->resizeColumnsToContents();
    int size = ui->tableView->font().pointSize() + 4;
    QHeaderView* header = ui->tableView->verticalHeader();
    header->setDefaultSectionSize(size);
    header->sectionResizeMode(QHeaderView::Fixed);
}

void MainWindow::showSplashScreen() {
    splashDialog.setParent(this);
    centerDialog(splashDialog);
    splashDialog.showNormal();
}

void MainWindow::centerDialog(QDialog& dlg) {
    int pw = width();
    int ph = height() + ui->toolBar->height();
    int mw = dlg.width();
    int mh = dlg.height();
    if (pw >= mw && ph >= mh) {
        dlg.move((pw - mw) / 2, (ph - mh) / 2);
    }
}
