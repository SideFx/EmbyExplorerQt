//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Error messages
// last change: 20251014
//-----------------------------------------------------------------------------------------------------------
#include "messagedialog.h"
#include <QMessageBox>

MessageDialog::MessageDialog(QObject *parent) : QObject{parent} {
    message m;
    m = {
        .msgNumber = MSG_ERR10,
        .msgString = tr("User record not found.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR11,
         .msgString = tr("Failed to execute query for existing users.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR12,
         .msgString = tr("Login authentication unsuccessful.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR13,
         .msgString = tr("Error occurred during Excel file save.")};
    messages.append(m);
    m = {
         .msgNumber = MSG_ERR14,
         .msgString = tr("Could not establish a connection to the Emby server.")};
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
