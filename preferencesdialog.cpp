//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Preferences dialog (connection, import options, font)
//-----------------------------------------------------------------------------------------------------------
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);
    setFixedSize(this->geometry().width(),this->geometry().height());
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::setSettings(embySettings s) {
    ui->tabWidget->setCurrentIndex(0);
    ui->chk_https->setChecked(s.https);
    ui->edt_address->setText(s.address);
    ui->edt_port->setText(s.port);
    if (s.port.isEmpty()) ui->edt_port->setText(EMBY_PORT);
    ui->edt_username->setText(s.username);
    ui->edt_password->setText(s.password);
    if (s.maxactors > 0) ui->spb_maxActors->setValue(s.maxactors);
    if (s.maxdirectors > 0) ui->spb_maxDirectors->setValue(s.maxdirectors);
    if (s.maxstudios > 0) ui->spb_maxStudios->setValue(s.maxstudios);
    if (s.maxgenres > 0) ui->spb_maxGenres->setValue(s.maxgenres);
    if (!s.font.isEmpty()) {
        try {
            QFont font;
            font.fromString(s.font);
            ui->cbx_fontfamily->setCurrentFont(font);
            ui->spb_fontsize->setValue(font.pointSize());
        }
        catch (...) {}
    }
}

embySettings PreferencesDialog::getSettings() {
    embySettings s = {
        .https = ui->chk_https->isChecked(),
        .address = ui->edt_address->text(),
        .port = ui->edt_port->text(),
        .username = ui->edt_username->text(),
        .password = ui->edt_password->text(),
        .maxactors = ui->spb_maxActors->value(),
        .maxdirectors = ui->spb_maxDirectors->value(),
        .maxstudios = ui->spb_maxStudios->value(),
        .maxgenres = ui->spb_maxGenres->value()
    };
    QFont font = ui->cbx_fontfamily->currentFont();
    font.setPointSize(ui->spb_fontsize->value());
    s.font = font.toString();
    return s;
}
