#ifndef DINOZ_H
#define DINOZ_H

#include <QObject>
#include <QMap>

class Dinoz : public QObject
{
    Q_OBJECT

    public:
        Dinoz(QString name_, QString fire_, QString earth_, QString water_, QString lightning_, QString wind_);
        ~Dinoz();
        QString toString();
        QMap<qint8, QString> elementsMapping = QMap<qint8, QString>({{0, tr("feu")}, {1, tr("terre")}, {2, tr("eau")}, {3, tr("foudre")}, {4, tr("vent")}});

        QString getName()               { return this->name; };
        qint8 getFirstElement()         { return this->firstElement; };
        qint8 getSecondElement()        { return this->secondElement; };
        qint8 getThirdElement()         { return this->thirdElement; };

        qint16 getFirstElementPower()    { return this->elements[this->firstElement]; };
        qint16 getSecondElementPower()   { return this->elements[this->secondElement]; };
        qint16 getThirdElementPower()    { return this->elements[this->thirdElement]; };

    private:
        QString name;
        QMap<qint8, qint16> elements;
        qint8 firstElement;
        qint8 secondElement;
        qint8 thirdElement;
};

#endif // DINOZ_H
