//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, About dialog
//-----------------------------------------------------------------------------------------------------------

#include "globals.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    setFixedSize(this->geometry().width(),this->geometry().height());
    ui->lbl_1->setText(QString(APP_NAME) + " " + QString(APP_VERSION));
    ui->lbl_2->setText(about_1);
    ui->lbl_3->setText(about_2);
    ui->lbl_4->setText(about_3 + qVersion());
}

AboutDialog::~AboutDialog() {
    delete ui;
}

