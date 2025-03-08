//---------------------------------------------------------------------------------
// App preferences (w) 2025 Jan Buchholz
//---------------------------------------------------------------------------------

#pragma once
#include <QObject>
#include <QJsonObject>
#include <QVariant>
#include <QVariantMap>

class JBPreferences : public QObject {
    Q_OBJECT

public:
    explicit JBPreferences(QObject *parent = nullptr);
    ~JBPreferences();
    QString GetPreferencesDefaultLocation();
    bool SavePreferences(QString filePath, QString orgName, QString appName);
    bool LoadPreferences(QString filePath, QString orgName, QString appName);
    bool SavePreferencesToDefaultLocation(QString orgName, QString appName);
    bool LoadPreferencesFromDefaultLocation(QString orgName, QString appName);
    void PushArray(QString key, QByteArray values);
    void PushString(QString key, QString value);
    void PushNumber(QString key, quint64 value);
    void PushBoolean(QString key, bool value);
    void PushFont(QString key, QFont font);
    QByteArray PopArray(QString key);
    QString PopString(QString key);
    quint64 PopNumber(QString key);
    bool PopBoolean(QString key);
    QFont PopFont(QString key);

private:
    QVariantMap prefs;

signals:
};

