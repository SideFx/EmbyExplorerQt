//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI, Details dialog (cover and narrative)
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QDialog>

namespace Ui {
class DetailsDialog;
}

class DetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit DetailsDialog(QWidget *parent = nullptr);
    ~DetailsDialog();
    void setDetails(QPixmap pix, QString overview);

private:
    Ui::DetailsDialog *ui;
};

