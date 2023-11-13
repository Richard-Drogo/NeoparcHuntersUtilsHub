#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QVector>
#include <iostream>
#include <QDebug>
#include <QProcess>
#include <QTextStream>

using namespace std;
/*!
 * \brief The Utilities class defines handy functions used by the application.
 */
class Utilities : public QObject
{
    Q_OBJECT
public:
    explicit    Utilities(QObject *parent = nullptr) : QObject(parent) { };

    static void qPrint(QString messageToBePrinted) {
        cout << messageToBePrinted.toStdString();
    }

    template<class T>
    static QString vectorToString(QVector<T> vector) {
        QString vectorStringified("[");

        QVectorIterator<T> itVector(vector);
        while (itVector.hasNext()) {
            T value = itVector.next();
            if(itVector.hasNext()){
                vectorStringified += value + ", ";
            } else {
                vectorStringified += value;
            }
        }
        vectorStringified += "]";
        return vectorStringified;
    }

    static void clearConsole(){
        #ifdef Q_OS_WIN
            QProcess::execute("cmd", QStringList({"/c", "CLS"}));
        #elif defined (Q_OS_LINUX)
            QProcess::execute("clear");
        #else
            Utilities::qPrint(tr("Cette plateforme n'est pas supportée...\n"));
        #endif
    }
Q_SIGNALS:

};

#endif // UTILITIES_H
