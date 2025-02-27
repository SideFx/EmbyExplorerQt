//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Export to Excel file
// uses SimpleXlsx by Pavel Akimov <oxod.pavel@gmail.com> and Alexandr Belyak <programmeralex@bk.ru>
// https://sourceforge.net/projects/simplexlsx/
//-----------------------------------------------------------------------------------------------------------
#include "export.h"
//#include "include/Xlsx/Workbook.h"
//using namespace SimpleXlsx;

Export::Export(QTableWidget *parent) {
    table = parent;
}

Export::~Export() {}

int Export::ExportCollection(QString fileName, QString sheetName, QVector<ColumnsType> colDef) {
	/*
    int cols = table->columnCount() - 1; // first column is hidden
    int rows = table->rowCount();
    int i, j;
    setlocale(LC_ALL, "");
    CWorkbook book(QString(EXPORT_BOOKNAME).toStdString());
    std::vector<ColumnWidth> ColWidth;
    for (i = 0; i < colDef.length(); i++) ColWidth.push_back(ColumnWidth(i, i, colDef[i].Width));
    CWorksheet &sheet = book.AddSheet(sheetName.toStdString(), ColWidth);
    Style header;
    header.horizAlign = ALIGN_H_CENTER;
    header.font.attributes = FONT_BOLD;
    const size_t CenterStyleIndex = book.AddStyle(header);
    Style standard;
    standard.horizAlign = ALIGN_H_LEFT;
    standard.font.attributes = FONT_NORMAL;
    const size_t StandardStyleIndex = book.AddStyle(standard);
    sheet.BeginRow();
    for (i = 0; i < colDef.length(); i++) sheet.AddCell(colDef[i].Caption.toStdString(), CenterStyleIndex);
    sheet.EndRow();
    for (j = 0; j < rows; j++) {
        sheet.BeginRow();
        for (i = EXPORT_STARTCOLUMN; i <= cols; i++) {
            QString itemText = table->item(j, i)->text();
            sheet.AddCell(itemText.toStdString(), StandardStyleIndex);
        }
        sheet.EndRow();
    }
    if (book.Save(fileName.toStdString())) 	return MSG_OK;
	*/
    return MSG_ERR13;
}
