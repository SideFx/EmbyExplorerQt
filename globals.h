//-----------------------------------------------------------------------------------------------------------
// Emby Explorer (Qt) (w) 2024-2025 by Jan Buchholz
// Public stuff
//-----------------------------------------------------------------------------------------------------------
#pragma once
#include <qhttpheaders.h>
#include <qobject.h>

#define EMBY_PORT "8096"

#define APP_NAME "Emby Explorer (Qt)"
#define APP_VERSION "v1.3"
#define APP_NAMEQT "EmbyExplorerQt"
#define SET_COMPANY "org.jan.buchholz"
#define SET_WGEOMETRY "mainwindow-geometry"
#define SET_WSTATE "mainwindow-windowState"
#define SET_EMBYADDRESS "emby-address"
#define SET_EMBYPORT "emby-port"
#define SET_EMBYUSERNAME "emby-username"
#define SET_EMBYPASSWORD "emby-password"
#define SET_EMBYHTTPS "emby-https"
#define SET_COLLMAXACTORS "collection-maxactors"
#define SET_COLLMAXDIRECTORS "collection-maxdirectors"
#define SET_COLLMAXSTUDIOS "collection-maxstudios"
#define SET_COLLMAXGENRES "collection-maxgenres"
#define SET_OPTIONSFONT "options-font"
#define APP_OPTIONS_EXT ".json"

#define COLLECTION_MOVIES  "movies"
#define COLLECTION_TVSHOWS "tvshows"
#define COLLECTION_HOMEVIDEOS "homevideos"

#define TYPEKIND_MOVIE "Movie"
#define TYPEKIND_SERIES "Series"
#define TYPEKIND_SEASON "Season"
#define TYPEKIND_EPISODE "Episode"
#define TYPEKIND_VIDEO "Video"
#define TYPEKIND_FOLDER "Folder"

#define PERSONTYPE_ACTOR "Actor"
#define PERSONTYPE_DIRECTOR "Director"

#define STREAMTYPE_AUDIO "Audio"
#define STREAMTYPE_VIDEO "Video"

#define EXPORT_STARTCOLUMN 1
#define EXPORT_BOOKNAME "Emby Collection"
#define EXPORT_PREFIX "Emby"
#define EXPORT_SUFFIX ".xlsx"

typedef struct {
    bool https;
    QString address;
    QString port;
    QString username;
    QString password;
    int maxactors;
    int maxdirectors;
    int maxstudios;
    int maxgenres;
    QString font;
} embySettings;

typedef struct {
    QString url;
    QHttpHeaders headers;
    QByteArray body;
} restParameters;

typedef struct {
    QString Caption;
    int Width;
    QString Column;
} ColumnsType;

typedef struct {
    QString CollectionType;
    QString APIFields;
    QVector<ColumnsType> Columns;
} DataType;

typedef struct {
    QString Name;
    QString Id;
    QString CollectionType;
} UserCollectionType;

typedef struct {
    QString Name;
    QString OriginalTitle;
    QString ProductionYear;
    QString Runtime;
    QString Actors;
    QString Directors;
    QString Studios;
    QString Genres;
    QString Container;
    QString Codecs;
    QString Resolution;
    QString DateCreated;
    QString FileName;
    QString Overview;
    QString MovieId;
    QString TypeKind;
} MovieDataType;
typedef QVector<MovieDataType> MovieDataTable;
Q_DECLARE_METATYPE(MovieDataType)
Q_DECLARE_METATYPE(MovieDataTable)

typedef struct {
    QString Name;
    QString Episode;
    QString Season;
    QString ProductionYear;
    QString Runtime;
    QString Actors;
    QString Studios;
    QString Genres;
    QString Container;
    QString Codecs;
    QString Resolution;
    QString DateCreated;
    QString FileName;
    QString Overview;
    QString SeriesId;
    QString SeasonId;
    QString EpisodeId;
    QString TypeKind;
    int SortIndex;
} SeriesDataType;
typedef QVector<SeriesDataType> SeriesDataTable;
Q_DECLARE_METATYPE(SeriesDataType)
Q_DECLARE_METATYPE(SeriesDataTable)

typedef struct {
    QString Name;
    QString Folder;
    QString Runtime;
    QString Container;
    QString Codecs;
    QString Resolution;
    QString DateCreated;
    QString FileName;
    QString FolderId;
    QString ParentId;
    QString VideoId;
    QString TypeKind;
} VideoDataType;
typedef QVector<VideoDataType> VideoDataTable;
Q_DECLARE_METATYPE(VideoDataType)
Q_DECLARE_METATYPE(VideoDataTable)

typedef struct {
    QString ItemId;
    QString ItemId2;
    QString Overview;
} DetailsDataType;

enum {
    MSG_OK,
    MSG_ERR10,
    MSG_ERR11,
    MSG_ERR12,
    MSG_ERR13,
    MSG_ERR14,
};

typedef struct {
    int msgNumber;
    QString msgString;
} message;
