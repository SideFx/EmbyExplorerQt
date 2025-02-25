//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Main window
//-----------------------------------------------------------------------------------------------------------
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardPaths>
#include <QSettings>
#include <QHeaderView>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
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
    setCentralWidget(ui->tableView);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(onActionQuit()));
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(onActionSettings()));
    QObject::connect(ui->actionAuthenticate, SIGNAL(triggered()), this, SLOT(onActionAuthenticate()));
    QObject::connect(ui->actionFetch, SIGNAL(triggered()), this, SLOT(onActionFetch()));
    QObject::connect(ui->actionDetails, SIGNAL(triggered()), this, SLOT(onActionDetails()));
    QObject::connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(onActionExport()));
    QObject::connect(ui->actionAboutEmbyExplorer, SIGNAL(triggered()), this, SLOT(onActionAboutEmbyExplorer()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(onActionAboutQt()));
    QObject::connect(cbxCollection, SIGNAL(currentIndexChanged(int)), this, SLOT(onCollectionChanged(int)));
    QObject::connect(ui->tableView, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
    detailsVisible = false;
    dis = new Dispatcher;
    exp = new Export(ui->tableView);
    loadSettings();
    showSplashScreen();
}

MainWindow::~MainWindow() {
    delete exp;
    delete dis;
    delete cbxCollection;
    delete ui;
}

void MainWindow::onActionQuit() {
    saveSettings();
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
    }
}

void MainWindow::onCollectionChanged(int i) {   
    if(i >= 0) {
        UserCollectionType coll = collections[i];
        initTableView(coll.CollectionType);
        if (detailsVisible) {
            detailsVisible = false;
            detailsDialog.hide();
        }
    }
}

void MainWindow::onActionAuthenticate() {
    if(checkSettings()) {
        int result = dis->EmbyAuthenticate(embySettings);
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
    DetailsDataType o;
    if (cbxCollection->currentIndex() >= 0) {
        UserCollectionType coll = collections[cbxCollection->currentIndex()];
        QVector collData = dis->EmbyGetCollectionData(coll.Id, coll.CollectionType);
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
                ui->tableView->setItem(i, j++, new QTableWidgetItem(m.FileName));
                o = {
                    .ItemId = m.MovieId,
                    .ItemId2 = m.MovieId,
                    .Overview = m.Overview
                };
                detailsBuffer.append(o);
            }
            else if (coll.CollectionType == COLLECTION_TVSHOWS) {
                SeriesDataType s = data.value<SeriesDataType>();
                if (s.TypeKind == TYPEKIND_EPISODE) {
                    id = s.EpisodeId;
                    id2 = s.SeasonId;
                } else {
                    id = s.SeriesId;
                    id2 = s.SeriesId;
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
                if (v.TypeKind == TYPEKIND_VIDEO) id = v.VideoId;
                else if (v.TypeKind == TYPEKIND_FOLDER) id = v.FolderId;
                ui->tableView->setItem(i, j++, new QTableWidgetItem(id));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Name));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Folder));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Runtime));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Container));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Codecs));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.Resolution));
                ui->tableView->setItem(i, j++, new QTableWidgetItem(v.FileName));
            }
            i++;
        }
        adjustTableView();
    }
}

void MainWindow::onActionDetails() {
    if (cbxCollection->currentIndex() >= 0) {
        UserCollectionType coll = collections[cbxCollection->currentIndex()];
        if (coll.CollectionType == COLLECTION_HOMEVIDEOS) { // no details for home videos
            if (detailsVisible) {
                detailsVisible = false;
                detailsDialog.hide();
            }
            return;
        }
        detailsVisible = !detailsVisible;
        if (detailsVisible) displayDetails(); else detailsDialog.hide();
    }
}

void MainWindow::onActionExport() {
    if (ui->tableView->rowCount() == 0) {
        return;
    }
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
        saveDialog.setDirectory(folder);
        saveDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveDialog.setFileMode(QFileDialog::AnyFile);
        saveDialog.setNameFilter(FILE_FILTER);
        saveDialog.setOptions(QFileDialog::DontUseCustomDirectoryIcons);
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

void MainWindow::onActionAboutQt(){
    QApplication::aboutQt();
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    if (splashDialog.isVisible()) centerDialog(splashDialog);
    else if (detailsDialog.isVisible()) centerDialog(detailsDialog);
}

void MainWindow::onItemSelectionChanged() {
    if (detailsVisible) displayDetails();
}

void MainWindow::displayDetails() {
    QString overview = "", itemId2 = "";
    QPixmap pix = {};
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
    adjustTableView();
}


bool MainWindow::checkSettings() {
    return !embySettings.address.isEmpty() && !embySettings.port.isEmpty() &&
           !embySettings.username.isEmpty() && !embySettings.password.isEmpty();
}

void MainWindow::saveSettings() {
    QSettings settings(SET_COMPANY, APP_NAMEQT);
    settings.setDefaultFormat(QSettings::NativeFormat);
    settings.setValue(SET_WGEOMETRY, saveGeometry());
    settings.setValue(SET_WSTATE, saveState());
    settings.setValue(SET_EMBYHTTPS, embySettings.https);
    settings.setValue(SET_EMBYADDRESS, embySettings.address);
    settings.setValue(SET_EMBYPORT, embySettings.port);
    settings.setValue(SET_EMBYUSERNAME, embySettings.username);
    settings.setValue(SET_EMBYPASSWORD, embySettings.password);
    settings.setValue(SET_COLLMAXACTORS, embySettings.maxactors);
    settings.setValue(SET_COLLMAXDIRECTORS, embySettings.maxdirectors);
    settings.setValue(SET_COLLMAXSTUDIOS, embySettings.maxstudios);
    settings.setValue(SET_COLLMAXGENRES, embySettings.maxgenres);
    settings.setValue(SET_OPTIONSFONT, embySettings.font);
}

void MainWindow::loadSettings() {
    QSettings settings(SET_COMPANY, APP_NAMEQT);
    settings.setDefaultFormat(QSettings::NativeFormat);
    restoreGeometry(settings.value(SET_WGEOMETRY).toByteArray());
    restoreState(settings.value(SET_WSTATE).toByteArray());
    embySettings = {
        .https = settings.value(SET_EMBYHTTPS).toBool(),
        .address = settings.value(SET_EMBYADDRESS).toString(),
        .port = settings.value(SET_EMBYPORT).toString(),
        .username = settings.value(SET_EMBYUSERNAME).toString(),
        .password = settings.value(SET_EMBYPASSWORD).toString(),
        .maxactors = settings.value(SET_COLLMAXACTORS).toInt(),
        .maxdirectors = settings.value(SET_COLLMAXDIRECTORS).toInt(),
        .maxstudios = settings.value(SET_COLLMAXSTUDIOS).toInt(),
        .maxgenres = settings.value(SET_COLLMAXGENRES).toInt()
    };
    try {
        QFont font;
        embySettings.font = settings.value(SET_OPTIONSFONT).toString();
        if (!embySettings.font.isEmpty()) {
            font.fromString(embySettings.font);
            ui->tableView->setFont(font);
        }
    }
    catch (...) {}
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
