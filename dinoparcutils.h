#ifndef DINOPARCUTILS_H
#define DINOPARCUTILS_H

#include <QObject>

class DinoparcUtils : public QObject
{
    Q_OBJECT

public:
    explicit DinoparcUtils(QObject *parent = nullptr);
    static qint8 getRoundFactorByElements(qint8 leftElement, qint8 rightElement);

    static const qint8 FIRE_ELEMENT     = 0;
    static const qint8 EARTH_ELEMENT    = 1;
    static const qint8 WATER_ELEMENT    = 2;
    static const qint8 THUNDER_ELEMENT  = 3;
    static const qint8 WIND_ELEMENT     = 4;

};

#endif // DINOPARCUTILS_H
