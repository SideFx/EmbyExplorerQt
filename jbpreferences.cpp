//---------------------------------------------------------------------------------
// App preferences (w) 2025 Jan Buchholz
//---------------------------------------------------------------------------------

#include "jbpreferences.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QByteArray>
#include <QString>
#include <QFont>
#include <QIODevice>
#include <QDir>
#include <QStandardPaths>
#include <QApplication>
#include "globals.h"

JBPreferences::JBPreferences(QObject *parent) : QObject{parent} {
    prefs.clear();
}

JBPreferences::~JBPreferences() {}

QString JBPreferences::GetPreferencesDefaultLocation() {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

void JBPreferences::PushArray(QString key, QByteArray values) {
    prefs.insert(key, QString(values.toBase64()));
}

void JBPreferences::PushString(QString key, QString value) {
    prefs.insert(key, QString(value.toUtf8().toBase64()));
}

void JBPreferences::PushNumber(QString key, quint64 value) {
    PushString(key, QString::number(value));
}

void JBPreferences::PushBoolean(QString key, bool value) {
    PushString(key, (value) ? "1" : "0");
}

void JBPreferences::PushFont(QString key, QFont font) {
    PushString(key, font.toString());
}

QByteArray JBPreferences::PopArray(QString key) {
    QByteArray ba = {};
    QString tmp = prefs.find(key).value().toString();
    if (!tmp.isEmpty()) {
        ba =  QByteArray::fromBase64(tmp.toUtf8());
    }
    return ba;
}

QString JBPreferences::PopString(QString key) {
    return QString(PopArray(key));
}

quint64 JBPreferences::PopNumber(QString key) {
    return PopString(key).toULongLong();
}

bool JBPreferences::PopBoolean(QString key) {
    return PopString(key) == "1";
}

QFont JBPreferences::PopFont(QString key) {
    QFont font;
    QString tmp = PopString(key);
    if (!tmp.isEmpty()) font.fromString(tmp); else font = QApplication::font();
    return font;
}

bool JBPreferences::SavePreferences(QString filePath, QString orgName, QString appName) {
    bool b = false;
    QJsonObject jprefs = QJsonObject::fromVariantMap(prefs);
    QJsonDocument doc(jprefs);
    if (!QDir(filePath).exists()) QDir().mkpath(filePath);
    if (!QDir(filePath).exists()) return false;
    QFile file(filePath + "/" + orgName + "." + appName + APP_OPTIONS_EXT);
    b = file.open(QIODevice::WriteOnly);
    if (b) {
        QTextStream out(&file);
        out << doc.toJson() << Qt::endl;
        file.close();
    }
    return b;
}

bool JBPreferences::LoadPreferences(QString filePath, QString orgName, QString appName) {
    prefs.clear();
    QJsonObject jprefs;
    QJsonDocument doc;
    QJsonParseError err;
    QString data = "";
    QFile file(filePath + "/" + orgName + "." + appName + APP_OPTIONS_EXT);
    if (!file.exists()) return false;
    bool b = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (b) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            data += in.readLine();
        }
        file.close();
        doc = QJsonDocument::fromJson(data.toLocal8Bit(), &err);
        if (err.error != QJsonParseError::NoError) return false;
        b = !doc.isNull();
        if (b) {
            jprefs = doc.object();
            prefs = jprefs.toVariantMap();
        }
    }
    return b;
}

bool JBPreferences::SavePreferencesToDefaultLocation(QString orgName, QString appName) {
    return SavePreferences(GetPreferencesDefaultLocation(), orgName, appName);
}

bool JBPreferences::LoadPreferencesFromDefaultLocation(QString orgName, QString appName) {
    return LoadPreferences(GetPreferencesDefaultLocation(), orgName, appName);
}
