//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// UI - Error messages
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QObject>
#include "globals.h"

class MessageDialog : public QObject {
    Q_OBJECT

public:
    explicit MessageDialog(QObject *parent = nullptr);
    ~MessageDialog();
    void showMessage(QWidget *parent, int number);

private:
    QVector<message> messages;

signals:
};

