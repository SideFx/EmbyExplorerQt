//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Splash screen (Emby logo)
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QDialog>

namespace Ui {
class SplashDialog;
}

class SplashDialog : public QDialog {
    Q_OBJECT

public:
    explicit SplashDialog(QWidget *parent = nullptr);
    ~SplashDialog();

private:
    Ui::SplashDialog *ui;
};

