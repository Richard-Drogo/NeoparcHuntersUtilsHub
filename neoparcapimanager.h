#ifndef NEOPARCAPIMANAGER_H
#define NEOPARCAPIMANAGER_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class NeoparcApiManager : public QObject
{
    Q_OBJECT
public:
    explicit NeoparcApiManager(QObject *parent = nullptr);
    static QList<QString> getOverallRanking(QString token);
private:
    static QList<QJsonDocument> getRankingApiResponses(QString token);

    static const QString BASE_URL;
    static const QString URL_RANKING;
    static const QList<QString> URL_RANKING_RESSOURCES;
    static const QString URL_RANKING_QUERY_PARAMS;
    static const QMap<qint8, qint8> RANKING_POSITION_MAPPED_TO_SCORE;
    static const qint16 NUMBER_OF_PLAYERS_TO_SHOW = 20;

signals:

};

#endif // NEOPARCAPIMANAGER_H
