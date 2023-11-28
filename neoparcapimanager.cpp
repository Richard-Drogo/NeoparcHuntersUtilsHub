#include "neoparcapimanager.h"

const QString NeoparcApiManager::BASE_URL = QString("https://neoparc.eternaltwin.org/api");
const QString NeoparcApiManager::URL_RANKING = BASE_URL + QString("/dinozs/chapter-rankings");
const QList<QString> NeoparcApiManager::URL_RANKING_RESSOURCES = {
    "default",
    "Moueffe",
    "Picori",
    "Castivore",
    "Sirain",
    "Winks",
    "Gorilloz",
    "Cargou",
    "Hippoclamp",
    "Rokky",
    "Pigmou",
    "WanWan",
    "Goupignon",
    "Kump",
    "Pteroz",
    "Santaz",
    "Ouistiti",
    "Korgon",
    "Kabuki",
    "Serpantin",
    "Soufflet",
    "Feross"
};
const QString NeoparcApiManager::URL_RANKING_QUERY_PARAMS = QString("?limit=10&offset=0");
const QMap<qint8, qint8> NeoparcApiManager::RANKING_POSITION_MAPPED_TO_SCORE = {
    {1, 10},
    {2, 8},
    {3, 5},
    {4, 4},
    {5, 3},
    {6, 2},
    {7, 2},
    {8, 2},
    {9, 1},
    {10, 1}
};

NeoparcApiManager::NeoparcApiManager(QObject *parent) : QObject(parent)
{

}

QList<QString> NeoparcApiManager::getOverallRanking(QString token){
    QList<QString> overallRanking;
    QList<QJsonDocument> rankingApiResponses = NeoparcApiManager::getRankingApiResponses(token);

    QMap<QString, qint16> playerRanking;
    foreach (const QJsonDocument &rankingApiResponse, rankingApiResponses) {
        if (!rankingApiResponse.isNull() && rankingApiResponse.isArray()) {
            QJsonArray rankingJsonArray = rankingApiResponse.array();

            qint8 playerCounter = 1;
            for (const QJsonValue &value : rankingJsonArray) {
                if (value.isObject() && playerCounter <= 10) {
                    QJsonObject jsonObject = value.toObject();

                    QString player = jsonObject["masterName"].toString();
                    if(playerRanking.contains(player)){
                        playerRanking[player] += RANKING_POSITION_MAPPED_TO_SCORE[playerCounter];
                    } else {
                        playerRanking[player] = RANKING_POSITION_MAPPED_TO_SCORE[playerCounter];
                    }
                }
                playerCounter++;
            }
        } else {
            qDebug() << "Erreur d'analyse JSON";
        }
    }

    QList<QString> sortedPlayers = playerRanking.keys();
    std::sort(
        sortedPlayers.begin(),
        sortedPlayers.end(),
        [&playerRanking](const QString &a, const QString &b) {
          return playerRanking[a] > playerRanking[b];
        }
    );

    int count = 0;
    for (const QString &player : sortedPlayers) {
        QString playerOverallRanking = "[" + QString::number(count + 1) + "]\t" + player + " : \t" + QString::number(playerRanking[player]);
        overallRanking.append(playerOverallRanking);
        ++count;
        if (count == NUMBER_OF_PLAYERS_TO_SHOW) {
            break;
        }
    }
    return overallRanking;
}

QList<QJsonDocument> NeoparcApiManager::getRankingApiResponses(QString token){
    QList<QJsonDocument> rankingApiResponses;

    foreach (const QString &rankingResource, NeoparcApiManager::URL_RANKING_RESSOURCES) {
        QUrl url(URL_RANKING + '/' + rankingResource + URL_RANKING_QUERY_PARAMS);
        QUrlQuery query;
        url.setQuery(query);
        QNetworkRequest request(url);
        QString authHeaderValue = "Bearer " + token;
        request.setRawHeader("Authorization", authHeaderValue.toLocal8Bit());
        QNetworkAccessManager manager;
        QNetworkReply *reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QByteArray responseData;
        if (reply->error() == QNetworkReply::NoError) {
            responseData = reply->readAll();
            rankingApiResponses.append(QJsonDocument::fromJson(responseData));
        } else {
            qDebug() << reply->errorString();
        }

        reply->deleteLater();
    }

    return rankingApiResponses;
}
