//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Splash screen (Emby logo)
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#include "splashdialog.h"
#include "ui_splashdialog.h"

SplashDialog::SplashDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SplashDialog) {
    ui->setupUi(this);
    setFixedSize(this->geometry().width(), this->geometry().height());
}

SplashDialog::~SplashDialog() {
    delete ui;
}

