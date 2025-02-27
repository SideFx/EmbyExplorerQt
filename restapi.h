//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Simple REST API
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>
#include <QRestAccessManager>
#include <qnetworkreply.h>
#include "globals.h"

class RestApi : public QObject {
    Q_OBJECT

public:
    explicit RestApi(QObject *parent = nullptr);
    ~RestApi();
    QByteArray makeRestCallGET(restParameters para, QNetworkReply::NetworkError *e);
    QByteArray makeRestCallPOST(restParameters para, QNetworkReply::NetworkError *e);

private:
    QNetworkAccessManager *netManager;
    QNetworkRequestFactory *netFactory;

};

