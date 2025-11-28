//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, About dialog
// last change: 20251128
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;

    QString const about_1 = "(w) 2024 - 2025 Jan Buchholz";
    QString const about_2 = tr("Thanks to:\nDaniel Nicoletti (dantti) and Jay Two (j2Doll) for \"QXlsx\"\n" \
                            "https://github.com/QtExcel/QXlsx");
    QString const about_3 = tr("Created using Qt Community Edition v");
};

