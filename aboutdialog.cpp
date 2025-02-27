//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, About dialog
//-----------------------------------------------------------------------------------------------------------
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    setFixedSize(this->geometry().width(),this->geometry().height());
}

AboutDialog::~AboutDialog() {
    delete ui;
}

