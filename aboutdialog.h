//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, About dialog
// last change: 20251014
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
    QString const about_2 = tr("Danksagung an:\nDaniel Nicoletti (dantti) und Jay Two (j2Doll) für \"QXlsx\"\n" \
                            "https://github.com/QtExcel/QXlsx\n&\n" \
                            "Dazzle-UI für die \"Dazzle-UI\" SVG Icons");
    QString const about_3 = tr("Erstellt mit Qt Community Edition v");
};

