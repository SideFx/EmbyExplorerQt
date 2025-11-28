//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Simple REST API
// last change: 20251128
//-----------------------------------------------------------------------------------------------------------
#include "restapi.h"
#include <QEventLoop>
#include <QNetworkReply>
#include <QTimer>

RestApi::RestApi(QObject *parent) : QObject{parent} {
    netManager = new QNetworkAccessManager();
    netFactory = new QNetworkRequestFactory();
}

RestApi::~RestApi() {
    delete netFactory;
    delete netManager;
}

QByteArray RestApi::makeRestCallGET(restParameters para, QNetworkReply::NetworkError *e) {
    QByteArray r;
    QNetworkReply *reply;
    netFactory->setBaseUrl(para.url);
    netFactory->setCommonHeaders(para.headers);
    QNetworkRequest request = netFactory->createRequest();
    reply = netManager->get(request);
    QEventLoop loop;
    // finished
    QObject::connect(reply, &QNetworkReply::finished, &loop, [&]() {
        loop.quit();
    });
    auto* timer = new QTimer(reply);
    timer->setSingleShot(true);
    // timeout
    QObject::connect(timer, &QTimer::timeout, &loop, [&]() {
        loop.quit();
    });
    timer->start(5000);
    loop.exec();
    *e = reply->error();
    r = reply->readAll();
    delete reply;
    return r;
}


QByteArray RestApi::makeRestCallPOST(restParameters para, QNetworkReply::NetworkError *e) {
    QByteArray r;
    QNetworkReply *reply;
    netFactory->setBaseUrl(para.url);
    netFactory->setCommonHeaders(para.headers);
    QNetworkRequest request = netFactory->createRequest();
    reply = netManager->post(request, para.body);
    QEventLoop loop;
    // finished
    QObject::connect(reply, &QNetworkReply::finished, &loop, [&]() {
        loop.quit();
    });
    auto* timer = new QTimer(reply);
    timer->setSingleShot(true);
    // timeout
    QObject::connect(timer, &QTimer::timeout, &loop, [&]() {
        loop.quit();
    });
    timer->start(5000);
    loop.exec();
    *e = reply->error();
    r = reply->readAll();
    delete reply;
    return r;
}
