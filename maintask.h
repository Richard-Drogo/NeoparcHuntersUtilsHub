#ifndef MAINTASK_H
#define MAINTASK_H

#include <QObject>
#include <QTextStream>
#include <QList>

#include "dinoz.h"
#include "neoparcapimanager.h"

class MainTask : public QObject
{
    Q_OBJECT

public:
    const QString CSV_DINOZ_DATA_EXPORT_FILENAME    = "DinozDataExport.csv";
    const QString CSV_DINOZ_DATA_EXPORT_SEPARATOR   = ",";
    explicit MainTask(int argc_, char *argv_[], QObject *parent_ = Q_NULLPTR);
    ~MainTask();


private:
    int         argc    = 0;
    char **     argv    = Q_NULLPTR;
    QObject *   parent  = Q_NULLPTR;
    QList<Dinoz *> dinoz;

    QVector<QString> displayFirstLevelMenu();
    void handleErrorInSelection(QVector<QString> allowedSelections);
    bool getDinozFromCsvFile(bool importAtStart = false);
    void createDinoz(QTextStream &);
    void simulateRaid(QTextStream &);
    qint16 getMoneyFromRaid(qint16 damagesDealtToRaidBoss);
    void promptToken(QTextStream &);

public Q_SLOTS:
    void closeApplication() { Q_EMIT quit(); };
    void startingPoint();

Q_SIGNALS:
    void quit();



};

#endif // MAINTASK_H
