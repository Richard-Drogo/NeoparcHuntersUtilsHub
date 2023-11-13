#include "dinoz.h"
#include <QDebug>

Dinoz::Dinoz(QString name_, QString fire_, QString earth_, QString water_, QString lightning_, QString wind_)
{
    this->name = name_;
    this->elements[0]   = fire_.toInt();
    this->elements[1]   = earth_.toInt();
    this->elements[2]   = water_.toInt();
    this->elements[3]   = lightning_.toInt();
    this->elements[4]   = wind_.toInt();

    QMap<qint8, qint16> elementsCopy = QMap<qint8, qint16>(this->elements);
    this->firstElement = this->elements.key(0);

    for(qint8 i = 1; i < elementsCopy.values().length(); i++){
        if(elementsCopy.values()[i] > elementsCopy[this->firstElement]){
            this->firstElement = elementsCopy.keys()[i];
        }
    }
    elementsCopy.remove(this->firstElement);

    this->secondElement = elementsCopy.keys()[0];
    for(qint8 i = 1; i < elementsCopy.values().length(); i++){
        if(elementsCopy.values()[i] > elementsCopy[this->secondElement]){
            this->secondElement = elementsCopy.keys()[i];
        }
    }
    elementsCopy.remove(this->secondElement);


    this->thirdElement = elementsCopy.keys()[0];
    for(qint8 i = 1; i < elementsCopy.values().length(); i++){
        if(elementsCopy.values()[i] > elementsCopy[this->thirdElement]){
            this->thirdElement = elementsCopy.keys()[i];
        }
    }
    elementsCopy.remove(this->thirdElement);
}

Dinoz::~Dinoz(){}

QString Dinoz::toString(){
    QString dinozStringified("");
    dinozStringified += this->name + " : ";
    QList<qint8> keys = this->elements.keys();
    for(qint64 i = 0; i < keys.length(); i++){
        qint8 key = keys[i];
        dinozStringified += this->elementsMapping[key] + "(" + QString::number(this->elements[key]) + ")";
        if(i + 1 < keys.length()){
            dinozStringified += ", ";
        }
    }
    return dinozStringified;
}
