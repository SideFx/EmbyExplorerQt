//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Export to Excel file
// uses QXlsx by Daniel Nicoletti (dantti): https://github.com/QtExcel/QXlsx
//-----------------------------------------------------------------------------------------------------------
#include "export.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include "xlsxformat.h"

Export::Export(QTableWidget *parent) {
    table = parent;
}

Export::~Export() {}

int Export::ExportCollection(QString fileName, QString sheetName, QVector<ColumnsType> colDef) {
    QXLSX_USE_NAMESPACE
    int cols = table->columnCount() - 1; // first column is hidden
    int rows = table->rowCount();
    int i, j;
    QString cellName;
    Document xlsx;
    xlsx.addSheet(sheetName);
    Format header;
    header.setFontBold(true);
    header.setHorizontalAlignment(Format::AlignHCenter);
    for (i = 0; i < colDef.length(); i++) {
        cellName = colDef[i].Column + "1";
        xlsx.setColumnWidth(i + 1, colDef[i].Width);
        xlsx.write(cellName, colDef[i].Caption, header);
    }
    Format standard;
    standard.setFontBold(false);
    standard.setHorizontalAlignment(Format::AlignLeft);
    for (j = 0; j < rows; j++) {
        for (i = EXPORT_STARTCOLUMN; i <= cols; i++) {
            cellName = colDef[i - EXPORT_STARTCOLUMN].Column + QString::number(j + 2);
            QString itemText = table->item(j, i)->text();
            xlsx.write(cellName, itemText, standard);
        }
    }
    if (xlsx.saveAs(fileName)) return MSG_OK;
    return MSG_ERR13;
}
