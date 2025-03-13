//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Control logic
//-----------------------------------------------------------------------------------------------------------
#include "dispatcher.h"
#include "restapi.h"
#include <QHostInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Dispatcher::Dispatcher(QObject *parent) : QObject{parent} {
    buildDataTypesOutline();
}

Dispatcher::~Dispatcher() {}

// MacOS Sequoia: This should trigger the grant network access popup
bool Dispatcher::EmbyCheckHost(embySettings settings) {
    QTcpSocket socket;
    socket.connectToHost(settings.address, settings.port.toInt());
    if (!socket.waitForConnected(5000)) return false;
    return true;
}

int Dispatcher::EmbyAuthenticate(embySettings settings) {
    int returnCode = MSG_OK;
    restParameters para = {};
    QNetworkReply::NetworkError e;
    QJsonDocument jsonDoc;
    userId.clear();
    accessToken.clear();
    maxActors = settings.maxactors;
    maxDirectors = settings.maxdirectors;
    maxStudios = settings.maxstudios;
    maxGenres = settings.maxgenres;
    baseUrl = ((settings.https) ? protoHttps : protoHttp) + settings.address +
              ":" + settings.port + embyURI;
    RestApi *api = new RestApi();
    para.url = baseUrl + endpointGETUsersPublic;
    QString ret = api->makeRestCallGET(para, &e);
    if (e == QNetworkReply::NoError) {
        jsonDoc = QJsonDocument::fromJson(ret.toUtf8());
        QJsonArray rootArray = jsonDoc.array();
        for (const QJsonValue &value : std::as_const(rootArray)) {
            QJsonObject obj = value.toObject();
            QString name = obj["Name"].toString();
            QString id = obj["Id"].toString();
            if (name.toUpper() == settings.username.toUpper()) {
                userId = id;
                break;
            }
        }
        if (!userId.isEmpty()) {
            para = {};
            para.url = baseUrl + endpointPOSTAuthenticateUser;
            QJsonObject auth;
            auth[kAuthUsername] = settings.username;
            auth[kAuthPassword] = settings.password;
            para.body = QJsonDocument(auth).toJson(QJsonDocument::Compact);
            para.headers.append(kContentType, pContentJson);
            para.headers.append(kAuthHeader, createHeader());
            QString ret = api->makeRestCallPOST(para, &e);
            if (e == QNetworkReply::NoError) {
                jsonDoc = QJsonDocument::fromJson(ret.toUtf8());
                if (jsonDoc.isObject()) {
                    QJsonObject jsonObj = jsonDoc.object();
                    accessToken = jsonObj["AccessToken"].toString();
                }
            } else returnCode = MSG_ERR12; // AuthenticateUser failed
        } else returnCode = MSG_ERR10; // User not found
    } else returnCode = MSG_ERR11; // GETUsersPublic failed
    delete api;
    return returnCode;
}

QVector<UserCollectionType> Dispatcher::EmbyGetCollections() {
    QVector<UserCollectionType> collections;
    restParameters para = {};
    QNetworkReply::NetworkError e;
    QJsonDocument jsonDoc;
    RestApi *api = new RestApi();
    para.url = baseUrl + endpointGETViews;
    para.url.replace(_UserId, userId, Qt::CaseSensitive);
    para.url = para.url + pApiKey + accessToken;
    QString ret = api->makeRestCallGET(para, &e);
    if (e == QNetworkReply::NoError) {
        jsonDoc = QJsonDocument::fromJson(ret.toUtf8());
        QJsonArray itemsArray = jsonDoc["Items"].toArray();
        for (const QJsonValue &value : std::as_const(itemsArray)) {
            QJsonObject obj = value.toObject();
            QString colType = obj["CollectionType"].toString();
            for (QString &c : CollectionTypes) {
                if (colType == c) {
                    UserCollectionType coll = {.Name = obj["Name"].toString(),
                                               .Id = obj["Id"].toString(),
                                               .CollectionType = colType};
                    collections.append(coll);
                }
            }
        }
    }
    delete api;
    return collections;
}

QVector<QVariant> Dispatcher::EmbyGetCollectionData(QString collectionId, QString collectionType) {
    QVector<QVariant> gen;
    MovieDataTable tm;
    SeriesDataTable ts;
    VideoDataTable tv;
    restParameters para = {};
    QNetworkReply::NetworkError e;
    QJsonDocument jsonDoc;
    QString typeKind;
    RestApi *api = new RestApi();
    para.url = baseUrl + endpointGETItems;
    para.url.replace(_UserId, userId, Qt::CaseSensitive);
    para.url = para.url + pApiKey + accessToken;
    para.url = para.url + "&" + pRecursive + "true";
    para.url = para.url + "&" + pParentId + collectionId;
    for (auto &d : dataTypes) {
        if (d.CollectionType == collectionType) {
            para.url = para.url + "&" + pFields + d.APIFields;
            break;
        }
    }
    QString ret = api->makeRestCallGET(para, &e);
    if (e == QNetworkReply::NoError) {
        jsonDoc = QJsonDocument::fromJson(ret.toUtf8());
        QJsonArray itemsArray = jsonDoc["Items"].toArray();
        for (const QJsonValue &value : std::as_const(itemsArray)) {
            typeKind = value["Type"].toString();
            if (collectionType == COLLECTION_MOVIES) {
                if (typeKind == TYPEKIND_MOVIE) {
                    MovieDataType m = evalMovieJsonData(value);
                    tm.append(m);
                }
            }
            else if (collectionType == COLLECTION_TVSHOWS) {
                if (typeKind == TYPEKIND_EPISODE || typeKind == TYPEKIND_SEASON || typeKind == TYPEKIND_SERIES) {
                    SeriesDataType s = evalSeriesJsonData(value);
                    ts.append(s);
                }
            }
            else if (collectionType == COLLECTION_HOMEVIDEOS) {
                if (typeKind == TYPEKIND_VIDEO || typeKind == TYPEKIND_FOLDER) {
                    VideoDataType v = evalVideoJsonData(value);
                    tv.append(v);
                }
            }
        }
        if (collectionType == COLLECTION_MOVIES) gen = sortMovieData(tm);
        else if (collectionType == COLLECTION_TVSHOWS) gen = sortSeriesData(ts);
        else if (collectionType == COLLECTION_HOMEVIDEOS) gen = sortVideoData(tv);
    }
    delete api;
    return gen;
}

QByteArray Dispatcher::EmbyGetPrimaryPictureForItem(QString itemId) {
    QByteArray pic;
    restParameters para = {};
    QNetworkReply::NetworkError e;
    RestApi *api = new RestApi();
    para.url = baseUrl + endpointGETImages;
    para.url.replace(_ItemId, itemId, Qt::CaseSensitive);
    para.url = para.url + "/" + pPrimaryImage + "/0";
    para.url = para.url + pApiKey + accessToken;
    para.url = para.url + "&" + pImageFormat + imageFormat;
    para.url = para.url + "&" + pMaxWidth + maxWidth;
    para.url = para.url + "&" + pMaxHeight + maxHeight;
    pic = api->makeRestCallGET(para, &e);
    if (e != QNetworkReply::NoError) pic.clear();
    delete api;
    return pic;
}

MovieDataType Dispatcher::evalMovieJsonData(QJsonValue value) {
    MovieDataType m;
    m.TypeKind = value["Type"].toString();
    m.Name = value["Name"].toString();
    m.OriginalTitle = value["OriginalTitle"].toString();
    m.MovieId = value["Id"].toString();
    QJsonArray peopleArray = value["People"].toArray();
    m.Actors = evalJsonPeopleData(peopleArray, PERSONTYPE_ACTOR, maxActors);
    m.Directors = evalJsonPeopleData(peopleArray, PERSONTYPE_DIRECTOR, maxDirectors);
    QJsonArray studioArray = value["Studios"].toArray();
    m.Studios = evalJsonStudioData(studioArray, maxStudios);
    QJsonArray genreArray = value["Genres"].toArray();
    m.Genres = evalJsonGenreData(genreArray, maxGenres);
    QJsonArray mediasourceArray = value["MediaSources"].toArray();
    m.Codecs = evalJsonMediaSourceData(mediasourceArray);
    m.Resolution = evalJsonResolutionData(value);
    m.Container = value["Container"].toString();
    if (m.Container.isEmpty()) m.Container = TXT_NA;
    m.ProductionYear = QString::number(value["ProductionYear"].toInt());
    qint64 ticks = value["RunTimeTicks"].toInteger();
    m.Runtime = evalRuntimeTicks(ticks);
    m.FileName = value["FileName"].toString(); 
    m.Overview = value["Overview"].toString();
    return m;
}

SeriesDataType Dispatcher::evalSeriesJsonData(QJsonValue value) {
    SeriesDataType s;
    s.TypeKind = value["Type"].toString();
    s.SortIndex = value["IndexNumber"].toInt();
    if (s.TypeKind == TYPEKIND_EPISODE) {
        s.Episode = value["Name"].toString();
        s.EpisodeId = value["Id"].toString();
        s.SeriesId = value["SeriesId"].toString();
        s.SeasonId = value["SeasonId"].toString();
        qint64 ticks = value["RunTimeTicks"].toInteger();
        s.Runtime = evalRuntimeTicks(ticks);
        s.Container = value["Container"].toString();
        if (s.Container.isEmpty()) s.Container = TXT_NA;
        s.Resolution = evalJsonResolutionData(value);
        s.ProductionYear = QString::number(value["ProductionYear"].toInt());
        s.FileName = value["FileName"].toString();
        QJsonArray peopleArray = value["People"].toArray();
        s.Actors = evalJsonPeopleData(peopleArray, PERSONTYPE_ACTOR, maxActors);
        QJsonArray mediasourceArray = value["MediaSources"].toArray();
        s.Codecs = evalJsonMediaSourceData(mediasourceArray);
        s.Overview = value["Overview"].toString();
    }
    else if (s.TypeKind == TYPEKIND_SEASON) {
        s.Season = value["Name"].toString();
        s.SeriesId = value["SeriesId"].toString();
        s.SeasonId = value["Id"].toString();
    }
    else if (s.TypeKind == TYPEKIND_SERIES) {
        s.Name = value["Name"].toString();
        s.SeriesId = value["Id"].toString();
        QJsonArray peopleArray = value["People"].toArray();
        s.Actors = evalJsonPeopleData(peopleArray, PERSONTYPE_ACTOR, maxActors);
        QJsonArray studioArray = value["Studios"].toArray();
        s.Studios = evalJsonStudioData(studioArray, maxStudios);
        QJsonArray genreArray = value["Genres"].toArray();
        s.Genres = evalJsonGenreData(genreArray, maxGenres);
    }
    return s;
}

VideoDataType Dispatcher::evalVideoJsonData(QJsonValue value) {
    VideoDataType v;
    v.TypeKind = value["Type"].toString();
    v.Name = value["Name"].toString();
    if (v.TypeKind == TYPEKIND_VIDEO) {
        v.VideoId = value["Id"].toString();
        v.Container = value["Container"].toString();
        if (v.Container.isEmpty()) v.Container = TXT_NA;
        v.Resolution = evalJsonResolutionData(value);
        qint64 ticks = value["RunTimeTicks"].toInteger();
        v.Runtime = evalRuntimeTicks(ticks);
        QJsonArray mediasourceArray = value["MediaSources"].toArray();
        v.Codecs = evalJsonMediaSourceData(mediasourceArray);
        v.ParentId = value["ParentId"].toString();
        v.FileName = value["FileName"].toString();
    }
    else if (v.TypeKind == TYPEKIND_FOLDER) {
        v.FolderId = value["Id"].toString();
    }
    return v;
}

QVector<QVariant> Dispatcher::sortMovieData(MovieDataTable tm) {
    QVector<QVariant> gen;
    QVariant vv;
    // sort movies by Name
    std::stable_sort(tm.begin(), tm.end(), [](const MovieDataType &a, const MovieDataType &b) {
        return a.Name < b.Name;
    });
    for (const MovieDataType &m : tm) {
        vv.setValue(m);
        gen.append(vv);
    }
    return gen;
}

QVector<QVariant> Dispatcher::sortSeriesData(SeriesDataTable ts) {
    QVector<QVariant> gen;
    QVariant vv;
    SeriesDataTable episodes;
    SeriesDataTable episodestmp;
    SeriesDataTable seasons;
    SeriesDataTable seasonstmp;
    SeriesDataTable series;
    for (const SeriesDataType &s : ts) {
        if (s.TypeKind == TYPEKIND_EPISODE) episodes.append(s);
        else if (s.TypeKind == TYPEKIND_SEASON) seasons.append(s);
        else if (s.TypeKind == TYPEKIND_SERIES) series.append(s);
    }
    // sort series by Name
    std::stable_sort(series.begin(), series.end(), [](const SeriesDataType &a, const SeriesDataType &b) {
        return a.Name < b.Name;
    });
    // pre-sort seasons by SeriesId
    std::stable_sort(seasons.begin(), seasons.end(), [](const SeriesDataType &a, const SeriesDataType &b) {
        return a.SeriesId < b.SeriesId;
    });
    // pre-sort episodes by SeriesId
    std::stable_sort(episodes.begin(), episodes.end(), [](const SeriesDataType &a, const SeriesDataType &b) {
        return a.SeriesId < b.SeriesId;
    });
    for (SeriesDataType &s : series) {
        vv.setValue(s);
        gen.append(vv);
        seasonstmp.clear();
        for (SeriesDataType &a : seasons) {
            if (a.SeriesId == s.SeriesId) {
                a.Name = s.Name;
                seasonstmp.append(a);
            }
        }
        // sort seasonstmp by Season
        std::stable_sort(seasonstmp.begin(), seasonstmp.end(), [](const SeriesDataType &a, const SeriesDataType &b) {
            return a.Season.toStdWString() < b.Season.toStdWString();
        });
        for (SeriesDataType &atmp : seasonstmp) {
            episodestmp.clear();
            for (SeriesDataType &e : episodes) {
                if (e.SeriesId == s.SeriesId && e.SeasonId == atmp.SeasonId) {
                    e.Name = s.Name;
                    e.Season = atmp.Season;
                    episodestmp.append(e);
                }
            }
            // sort episodestmp by SortIndex
            std::stable_sort(episodestmp.begin(), episodestmp.end(), [](const SeriesDataType &a, const SeriesDataType &b) {
                return a.SortIndex < b.SortIndex;
            });
            for (SeriesDataType &etmp : episodestmp) {
                vv.setValue(etmp);
                gen.append(vv);
            }
        }
    }
    return gen;
}

QVector<QVariant> Dispatcher::sortVideoData(VideoDataTable tv) {
    QVector<QVariant> gen;
    QVariant vv;
    VideoDataTable folders;
    VideoDataTable videos;
    for (const VideoDataType &v : tv) {
        if (v.TypeKind == TYPEKIND_VIDEO) videos.append(v);
        else if (v.TypeKind == TYPEKIND_FOLDER) folders.append(v);
    }
    // sort folders by Name
    std::stable_sort(folders.begin(), folders.end(), [](const VideoDataType &a, const VideoDataType &b) {
        return a.Name < b.Name;
    });
    // sort videos by Name
    std::stable_sort(videos.begin(), videos.end(), [](const VideoDataType &a, const VideoDataType &b) {
        return a.Name < b.Name;
    });
    for (VideoDataType &f : folders) {
        for (VideoDataType &v : videos) {
            if (v.ParentId == f.FolderId) {
                v.Folder = f.Name;
                vv.setValue(v);
                gen.append(vv);
            }
        }
    }
    return gen;
}

QString Dispatcher::createPair(QString key, QString value) {
    return key + "=\"" + value + "\"";
}

QString Dispatcher::createHeader() {
    return pAuthType + " " +
           createPair(kAuthKeyUserId, userId) + ", " +
           createPair(kAuthKeyClient, pAuthClient) + ", " +
           createPair(kAuthKeyDevice, QOperatingSystemVersion::current().name()) + ", " +
           createPair(kAuthKeyDeviceId, QHostInfo::localHostName()) + ", " +
           createPair(kAuthKeyVersion, pAuthVersion);
}

QString Dispatcher::evalJsonPeopleData(QJsonArray peopleArray, QString peopleType, int maxItems) {
    int count = 0;
    QString s = "";
    for (const QJsonValue &value : std::as_const(peopleArray)) {
        QString typeKind = value["Type"].toString();
        if (typeKind == peopleType) {
            if (maxItems == 0 || count < maxItems) {
                s = appendItem(s, value["Name"].toString());
                count++;
            }
        }
    }
    return s;
}

QString Dispatcher::evalJsonStudioData(QJsonArray studioArray, int maxItems) {
    int count = 0;
    QString s = "";
    for (const QJsonValue &value : std::as_const(studioArray)) {
        if (maxItems == 0 || count < maxItems) {
            s = appendItem(s, value["Name"].toString());
            count++;
        }
    }
    return s;
}

QString Dispatcher::evalJsonGenreData(QJsonArray genreArray, int maxItems) {
    int count = 0;
    QString s = "";
    for (const QJsonValue &value : std::as_const(genreArray)) {
        if (maxItems == 0 || count < maxItems) {
            s = appendItem(s, value.toString());
            count++;
        }
    }
    return s;
}

QString Dispatcher::evalJsonResolutionData(QJsonValue value) {
    qint32 w = value["Width"].toInt();
    qint32 h = value["Height"].toInt();
    if (w > 0 && h > 0) return QString::number(w) + "x" + QString::number(h);
    else return TXT_NA;
}

QString Dispatcher::evalJsonMediaSourceData(QJsonArray mediasourceArray) {
    QString video = "";
    QString audio = "";
    for (const QJsonValue &value : std::as_const(mediasourceArray)) {
        QJsonArray mediastreamArray = value["MediaStreams"].toArray();
        for (const QJsonValue &media : std::as_const(mediastreamArray)) {
            QString type = media["Type"].toString();
            if (type == STREAMTYPE_VIDEO && video.isEmpty()) {
                video = media["Codec"].toString();
            }
            if (type == STREAMTYPE_AUDIO && audio.isEmpty()) {
                audio = media["Codec"].toString();
            }
        }
        break;
    }
    if (video.isEmpty()) video = TXT_NA;
    if (audio.isEmpty()) audio = TXT_NA;
    return video + ", " + audio;
}

QString Dispatcher::evalRuntimeTicks(qint64 ticks) {
    QString s = "";
    qint64 r, hours, minutes;
    if (ticks > 0) {
        r = ticks / 10000000;
        hours = r / 3600;
        minutes = (r % 3600) / 60;
        if (hours > 0) {
            s = QString::number(hours) + "h";
        }
        if (minutes > 0) {
            s = s + QString::number(minutes) + "m";
        }
    } else s = TXT_NA;
    return s;
}

void Dispatcher::buildDataTypesOutline() {
    DataType d;
    d.CollectionType = COLLECTION_MOVIES;
    d.APIFields = QString("Name,OriginalTitle,MediaSources,FileName,Genres,") +
                  QString("ProductionYear,People,Studios,Width,Height,Container,") +
                  QString("Overview,RunTimeTicks,Type");
    d.Columns = {
        {CAP_TITLE, 70, "A"},
        {CAP_ORIGINALTITLE, 70, "B"},
        {CAP_YEAR, 10, "C"},
        {CAP_TIME, 10, "D"},
        {CAP_ACTORS, 90, "E"},
        {CAP_DIRECTOR, 50, "F"},
        {CAP_STUDIO, 40, "G"},
        {CAP_GENRE, 50, "H"},
        {CAP_EXTENSION, 10, "I"},
        {CAP_CODEC, 12, "J"},
        {CAP_RESOLUTION, 15, "K"},
        {CAP_FILENAME, 70, "L"}
    };
    dataTypes.append(d);
    d.CollectionType = COLLECTION_TVSHOWS;
    d.APIFields = QString("Name,MediaSources,Genres,ProductionYear,People,") +
                  QString("Studios,Width,Height,Container,RunTimeTicks,FileName,") +
                  QString("Overview,SeriesId,SeasonId,Id,ParentId,IndexNumber,Type");
    d.Columns = {
        {CAP_SERIES, 50, "A"},
        {CAP_EPISODE, 50, "B"},
        {CAP_SEASON, 30, "C"},
        {CAP_YEAR, 10, "D"},
        {CAP_TIME, 10, "E"},
        {CAP_ACTORS, 100, "F"},
        {CAP_STUDIO, 40, "G"},
        {CAP_GENRE, 50, "H"},
        {CAP_EXTENSION, 10, "I"},
        {CAP_CODEC, 12, "J"},
        {CAP_RESOLUTION, 15, "K"},
        {CAP_FILENAME, 70, "L"}
    };
    dataTypes.append(d);
    d.CollectionType = COLLECTION_HOMEVIDEOS;
    d.APIFields = QString("Name,MediaSources,Width,Height,Container,RunTimeTicks,") +
                  QString("FileName,ParentId,Type");
    d.Columns = {
        {CAP_TITLE, 70, "A"},
        {CAP_FOLDER, 30, "B"},
        {CAP_TIME, 10, "C"},
        {CAP_EXTENSION, 10, "D"},
        {CAP_CODEC, 12, "E"},
        {CAP_RESOLUTION, 15, "F"},
        {CAP_FILENAME, 70, "G"}
    };
    dataTypes.append(d);
}

QVector<ColumnsType> Dispatcher::getColumns(QString collectionType) {
    QVector<ColumnsType> cols;
    for (auto &d : dataTypes) {
        if (d.CollectionType == collectionType) {
            cols = d.Columns;
            break;
        }
    }
    return cols;
}

QString Dispatcher::appendItem(QString source, QString append) {
    return source.isEmpty() ? append : source + ", " + append;
}

