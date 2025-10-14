//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Export to Excel file
// uses SimpleXlsx by Pavel Akimov <oxod.pavel@gmail.com> and Alexandr Belyak <programmeralex@bk.ru>
// https://sourceforge.net/projects/simplexlsx/
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include "globals.h"

class Export : public QObject {
    Q_OBJECT

public:
    explicit Export(QTableWidget *parent = nullptr);
    ~Export();
    int ExportCollection(QString fileName, QString sheetName, QVector<ColumnsType> colDef);

private:
    QTableWidget *table;

signals:
};

