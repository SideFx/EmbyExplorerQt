//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Control logic
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <QVariant>
#include <QOperatingSystemVersion>
#include <qregularexpression.h>
#include "globals.h"

class Dispatcher : public QObject {
    Q_OBJECT

public:
    explicit Dispatcher(QObject *parent = nullptr);
    ~Dispatcher();
    int EmbyAuthenticate(embySettings settings);
    QVector<UserCollectionType> EmbyGetCollections();
    QVector<QVariant> EmbyGetCollectionData(QString collectionId, QString collectionType);
    QByteArray EmbyGetPrimaryPictureForItem(QString itemId);
    QVector<ColumnsType> getColumns(QString collectionType);
    QString CollectionTypes[3] = {COLLECTION_MOVIES, COLLECTION_TVSHOWS, COLLECTION_HOMEVIDEOS};
    QString const TXT_NA = tr("n.v.");
    QString const imageFormat = "png";

 private:
    QString userId;
    QString accessToken;
    QString baseUrl;

    int maxActors = 1;
    int maxDirectors = 1;
    int maxGenres = 1;
    int maxStudios = 1;

    QString const maxWidth = "180";
    QString const maxHeight = "250";

    void buildDataTypesOutline();
    QString appendItem(QString source, QString append);
    MovieDataType evalMovieJsonData(QJsonValue value);
    SeriesDataType evalSeriesJsonData(QJsonValue value);
    VideoDataType evalVideoJsonData(QJsonValue value);
    QString createPair(QString key, QString value);
    QString createHeader();
    QString evalRuntimeTicks(qint64 ticks);
    QString evalJsonPeopleData(QJsonArray peopleArray, QString peopleType, int maxItems);
    QString evalJsonStudioData(QJsonArray studioArray, int maxItems);
    QString evalJsonGenreData(QJsonArray genreArray, int maxItems);
    QString evalJsonMediaSourceData(QJsonArray mediasourceArray);
    QString evalJsonResolutionData(QJsonValue value);
    QVector<QVariant> sortMovieData(MovieDataTable tm);
    QVector<QVariant> sortSeriesData(SeriesDataTable ts);
    QVector<QVariant> sortVideoData(VideoDataTable tv);

    QString const protoHttp = "http://";
    QString const protoHttps = "https://";
    QString const embyURI = "/emby";
    QString const pApiKey = "?api_key=";
    QString const kAuthUsername = "Username";
    QString const kAuthPassword = "Pw";
    QString const pAuthType = "Emby";
    QString const pAuthVersion = "1.0.0.0";
    QString const pAuthClient = "EmbyExplorerQt";
    QString const pContentJson = "application/json";
    QString const kContentType = "Content-Type";
    QString const kAuthHeader = "Authorization";
    QString const kAuthKeyUserId = "UserId";
    QString const kAuthKeyClient = "Client";
    QString const kAuthKeyDevice = "Device";
    QString const kAuthKeyDeviceId = "DeviceId";
    QString const kAuthKeyVersion = "Version";
    QString const pParentId = "ParentId=";
    QString const pRecursive = "Recursive=";
    QString const pFields = "Fields=";
    QString const pPrimaryImage = "Primary";
    QString const pImageFormat = "format=";
    QString const pMaxWidth = "MaxWidth=";
    QString const pMaxHeight = "MaxHeight=";

    QString const _UserId = "$1$";
    QString const _ItemId = "$2$";
    QString const endpointGETUsersPublic = "/Users/Public";
    QString const endpointPOSTAuthenticateUser = "/Users/AuthenticateByName";
    QString const endpointGETViews = "/Users/" + _UserId + "/Views";
    QString const endpointGETItems = "/Users/" + _UserId + "/Items";
    QString const endpointGETImages = "/Items/" + _ItemId + "/Images";

    QString const CAP_TITLE = tr("Titel");
    QString const CAP_ORIGINALTITLE = tr("Originaltitel");
    QString const CAP_YEAR = tr("Jahr");
    QString const CAP_TIME = tr("Dauer");
    QString const CAP_ACTORS = tr("Besetzung");
    QString const CAP_DIRECTOR = tr("Regisseur");
    QString const CAP_STUDIO = tr("Studio");
    QString const CAP_GENRE = tr("Genre");
    QString const CAP_EXTENSION = tr("Ext.");
    QString const CAP_CODEC = tr("Codec");
    QString const CAP_RESOLUTION = tr("Auflösung");
    QString const CAP_FILENAME = tr("Dateiname");
    QString const CAP_SERIES = tr("Serie");
    QString const CAP_SEASON = tr("Staffel");
    QString const CAP_EPISODE = tr("Episode");
    QString const CAP_FOLDER = tr("Ordner");

    QVector<DataType> dataTypes = {};

signals:
};

