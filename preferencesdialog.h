//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Preferences dialog (connection, import options, font)
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QDialog>
#include "globals.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

    void setSettings(embySettings s);
    embySettings getSettings();

private:
    Ui::PreferencesDialog *ui;
};

