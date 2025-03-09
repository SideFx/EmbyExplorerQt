//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Error messages
//-----------------------------------------------------------------------------------------------------------
#include "messagedialog.h"
#include <QMessageBox>

MessageDialog::MessageDialog(QObject *parent) : QObject{parent} {
    message m;
    m = {
        .msgNumber = MSG_ERR10,
        .msgString = tr("Der angegebene User wurde nicht gefunden.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR11,
         .msgString = tr("Die Abfrage auf existierende User schlug fehl.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR12,
         .msgString = tr("Fehler bei Authentifizierung.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR13,
         .msgString = tr("Fehler beim Speichern der Excel Datei.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR14,
         .msgString = tr("Es konnte keine Verbindung zum Emby Server hergestellt werden.")};
    messages.append(m);
}

MessageDialog::~MessageDialog() {}

void MessageDialog::showMessage(QWidget *parent, int number) {
    for (message &m : messages) {
        if (m.msgNumber == number) {
            QMessageBox msg;
            msg.setParent(parent, Qt::Dialog|
                                  Qt::WindowSystemMenuHint|
                                  Qt::WindowCloseButtonHint|
                                  Qt::WindowTitleHint|
                                  Qt::CustomizeWindowHint|
                                  Qt::MSWindowsFixedSizeDialogHint);
            msg.setText(m.msgString);
            msg.setWindowTitle(QString(APP_NAME) + " " + QString(APP_VERSION));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
            return;
        }
    }
}
