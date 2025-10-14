//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, Details dialog (cover and narrative)
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#include "detailsdialog.h"
#include "ui_detailsdialog.h"

DetailsDialog::DetailsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DetailsDialog) {
    ui->setupUi(this);
}

DetailsDialog::~DetailsDialog() {
    delete ui;
}

void DetailsDialog::setDetails(QPixmap pix, QString overview) {
    ui->edt_Overview->setPlainText(overview);
    ui->lbl_CoverImage->setPixmap(pix);
}
