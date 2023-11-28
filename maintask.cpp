#include <Windows.h>

#include "maintask.h"
#include "utilities.h"
#include "dinoparcutils.h"
#include "neoparcapimanager.h"

#include <QCoreApplication>
#include <QVector>
#include <QProcess> // Pour flusher la console.
#include <QFile>
#include <QHash>

using namespace std;

MainTask::MainTask(int argc_, char *argv_[], QObject *parent_) : QObject(parent_) {
    parent = parent_;
    argc = argc_;
    argv = argv_;
}

MainTask::~MainTask(){
    for(qint64 i = 0; i < this->dinoz.length(); i++){delete this->dinoz[i];}
}



void MainTask::startingPoint(){

    uint defaultConsoleOutputCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    getDinozFromCsvFile(true);

    QTextStream qTextStreamIn(stdin);
    QString firstMenuChoice;

    do{
        QVector<QString> allowedFirstMenuChoices = displayFirstLevelMenu();
        Utilities::qPrint(tr("Votre choix : "));
        qTextStreamIn.readLineInto(&firstMenuChoice);
        if(allowedFirstMenuChoices.contains(firstMenuChoice)){
            switch(firstMenuChoice.toInt()){
            case 1:{    // Mes Dinoz
                Utilities::clearConsole();
                Utilities::qPrint(tr("Mes Dinoz") + "\n");
                Utilities::qPrint("--------------------------------\n");
                for(qint64 i = 0; i < this->dinoz.length(); i++){
                    Utilities::qPrint(this->dinoz[i]->toString() + "\n");
                }
                Utilities::qPrint(tr("Appuyez pour continuer..."));
                qTextStreamIn.readLineInto(Q_NULLPTR);
                Utilities::clearConsole();
            }break;

            case 2:{    // Import CSV
                Utilities::clearConsole();
                Utilities::qPrint(tr("Vérifiez que le fichier \"DinozDataExport.csv\" se trouve bien dans le même répertoire que cet exécutable avant de poursuivre.") + "\n");
                Utilities::qPrint(tr("Appuyez pour continuer..."));
                qTextStreamIn.readLineInto(Q_NULLPTR);
                Utilities::clearConsole();

                getDinozFromCsvFile();
            }break;

            case 3:{    // Créer un dinoz
                Utilities::clearConsole();

                createDinoz(qTextStreamIn);

                Utilities::qPrint(tr("Appuyez pour continuer..."));
                qTextStreamIn.readLineInto(Q_NULLPTR);
                Utilities::clearConsole();
            }break;

            case 4:{    // Simuler un raid
                Utilities::clearConsole();

                simulateRaid(qTextStreamIn);

                Utilities::qPrint(tr("Appuyez pour continuer..."));
                qTextStreamIn.readLineInto(Q_NULLPTR);
                Utilities::clearConsole();
            }break;

            case 5:{    // Classement Général des dinoz
                Utilities::clearConsole();

                Utilities::qPrint(tr("Votre token permettant la connexion à l'API : "));
                QString token;
                qTextStreamIn.readLineInto(&token);

                QList<QString> overallRanking = NeoparcApiManager::getOverallRanking(token);
                for(int i = 0; i < overallRanking.size(); i++){
                    Utilities::qPrint(overallRanking.at(i) + "\n");
                }

                Utilities::qPrint(tr("Appuyez pour continuer..."));
                qTextStreamIn.readLineInto(Q_NULLPTR);
                Utilities::clearConsole();
            }break;

            case 6:{    // Paramètres

            }break;

            case 7:{    // On quitte l'application à la sortie de la boucle.
            }break;
            }
        } else {
            handleErrorInSelection(allowedFirstMenuChoices);
        }
    } while(firstMenuChoice != "7");

    SetConsoleOutputCP(defaultConsoleOutputCP);
    closeApplication();
}

void MainTask::handleErrorInSelection(QVector<QString> allowedSelections){
    Utilities::clearConsole();
    Utilities::qPrint(tr("/!\\ ERREUR ! /!\\") + "\n");
    Utilities::qPrint(tr("Vous avez saisi une valeur non autorisée !") + "\n");
    Utilities::qPrint(tr("Veuillez saisir une valeur dans l'intervalle suivant : "));
    Utilities::qPrint(Utilities::vectorToString(allowedSelections) + "\n");
}

QVector<QString> MainTask::displayFirstLevelMenu(){
    Utilities::qPrint("NeoparcBestDinozSelectorForRaids V1.0.0\n");
    Utilities::qPrint("--------------------------------\n");
    Utilities::qPrint(tr("1 - Mes Dinoz.") + "\n");
    Utilities::qPrint(tr("2 - Importer mes dinoz depuis l'export CSV Neoparc.") + "\n");
    Utilities::qPrint(tr("3 - Créer un dinoz.") + "\n");
    Utilities::qPrint(tr("4 - Simuler un raid.") + "\n");
    Utilities::qPrint(tr("5 - Classement général.") + "\n");
    Utilities::qPrint(tr("6 - Paramètres.") + "\n");
    Utilities::qPrint(tr("7 - Quitter l'application.") + "\n");
    Utilities::qPrint("\n");

    return QVector<QString>({"1", "2", "3", "4", "5", "6", "7"});
}

bool MainTask::getDinozFromCsvFile(bool importAtStart){
    QString dinozDataExportFilePath(QCoreApplication::applicationDirPath() + "/" + CSV_DINOZ_DATA_EXPORT_FILENAME);
    QFile dinozDataExportFile(dinozDataExportFilePath);

    if (!dinozDataExportFile.open(QIODevice::ReadOnly)) {
        if(importAtStart){
            Utilities::qPrint(tr("/!\\ ERREUR ! /!\\") + "\n");
            Utilities::qPrint(dinozDataExportFile.errorString() + "\n");
        }
        return false;
    } else {
        for(qint64 i = 0; i < this->dinoz.length(); i++){delete this->dinoz[i];}

        bool isFirstLine        = true;
        qint8 indexOfName       = -1;
        qint8 indexOfFire       = -1;
        qint8 indexOfEarth      = -1;
        qint8 indexOfWater      = -1;
        qint8 indexOfLightning  = -1;
        qint8 indexOfWind       = -1;

        while (!dinozDataExportFile.atEnd()) {
            if((!isFirstLine) && (indexOfName != -1) && (indexOfFire != -1) && (indexOfEarth != -1) && (indexOfWater != -1) && (indexOfLightning != -1) && (indexOfWind != -1)) {
                QStringList DinozCsvProperties   = QString(dinozDataExportFile.readLine()).split(CSV_DINOZ_DATA_EXPORT_SEPARATOR);
                Dinoz * parsedDinow = new Dinoz(
                    DinozCsvProperties[indexOfName],
                    DinozCsvProperties[indexOfFire],
                    DinozCsvProperties[indexOfEarth],
                    DinozCsvProperties[indexOfWater],
                    DinozCsvProperties[indexOfLightning],
                    DinozCsvProperties[indexOfWind]
                );
                dinoz.append(parsedDinow);
            } else {
                QString headersLine = dinozDataExportFile.readLine();
                QStringList headers = headersLine.split(CSV_DINOZ_DATA_EXPORT_SEPARATOR);
                for( qint64 i = 0; i < headers.length(); i++){
                    if(headers[i] == "Name"){ indexOfName = i; }
                    if(headers[i] == "Fire"){ indexOfFire = i; }
                    if(headers[i] == "Earth"){ indexOfEarth = i; }
                    if(headers[i] == "Water"){ indexOfWater = i; }
                    if(headers[i] == "Lightning"){ indexOfLightning = i; }
                    if(headers[i] == "Wind"){ indexOfWind = i; }
                }
                isFirstLine = false;
            }
        }
        dinozDataExportFile.close();
        return true;
    }
}

void MainTask::createDinoz(QTextStream &qTextStreamIn){
    QString newDinozName;
    QString fireValue;
    QString earthValue;
    QString waterValue;
    QString lightningValue;
    QString windValue;
    do {
        if(newDinozName != Q_NULLPTR){
            if(newDinozName.trimmed().length() <= 0){
                Utilities::qPrint(tr("/!\\ ERREUR ! /!\\") + "\n");
            }
        }
        Utilities::qPrint(tr("Nom du dinoz : "));
        qTextStreamIn.readLineInto(&newDinozName);
    } while(newDinozName.trimmed().length() <= 0);

    bool firstTry           = true;
    bool isFireValid        = false;
    bool isEarthValid       = false;
    bool isWaterValid       = false;
    bool isLightningValid   = false;
    bool isWindValid        = false;

    do {
        if(!firstTry){
            if((!isFireValid) || (!isEarthValid) || (!isWaterValid) || (!isLightningValid) || (!isWindValid)){
                Utilities::qPrint(tr("/!\\ ERREUR ! /!\\") + "\n");
            }
        }

        Utilities::qPrint(tr("Feu : "));
        qTextStreamIn.readLineInto(&fireValue);

        Utilities::qPrint(tr("Terre : "));
        qTextStreamIn.readLineInto(&earthValue);

        Utilities::qPrint(tr("Eau : "));
        qTextStreamIn.readLineInto(&waterValue);

        Utilities::qPrint(tr("Foudre : "));
        qTextStreamIn.readLineInto(&lightningValue);

        Utilities::qPrint(tr("Vent : "));
        qTextStreamIn.readLineInto(&windValue);

        fireValue.toInt(&isFireValid);
        earthValue.toInt(&isEarthValid);
        waterValue.toInt(&isWaterValid);
        lightningValue.toInt(&isLightningValid);
        windValue.toInt(&isWindValid);

        firstTry = false;
    } while((!isFireValid) || (!isEarthValid) || (!isWaterValid) || (!isLightningValid) || (!isWindValid));

    Dinoz * newDinoz = new Dinoz(newDinozName, fireValue, earthValue, waterValue, lightningValue, windValue);
    dinoz.append(newDinoz);

    Utilities::qPrint(tr("Création de ") + newDinoz->toString());
}

void MainTask::simulateRaid(QTextStream &qTextStreamIn){
    QString firstElementString, secondElementString, thirdElementString;
    qint8   bossFirstElementValue   = 110 + 15;
    qint8   bossSecondElementValue  = 110 + 10;
    qint8   bossThirdElementValue   = 110 + 5;

    QVector<QString> allowedElements = QVector<QString>({"0", "1", "2", "3", "4"});

    bool firstTry = true;
    do {
        if(!firstTry){
            Utilities::qPrint(tr("/!\\ ERREUR ! /!\\") + "\n");
            Utilities::qPrint(tr("/!\\ Écrivez les trois chiffres des éléments principaux. (Et tous différents) ! /!\\") + "\n");
        }
        Utilities::qPrint(tr("Feu : 0 | Terre : 1 | Eau : 2 | Foudre : 3 | Vent : 4") + "\n");
        Utilities::qPrint(tr("Élément principal du boss de raid : ") + "\n");
        qTextStreamIn.readLineInto(&firstElementString);
        Utilities::qPrint(tr("Élément secondaire du boss de raid : ") + "\n");
        qTextStreamIn.readLineInto(&secondElementString);
        Utilities::qPrint(tr("Élément tertiaire du boss de raid : ") + "\n");
        qTextStreamIn.readLineInto(&thirdElementString);
        firstTry = false;
    } while((!allowedElements.contains(firstElementString)) || (!allowedElements.contains(secondElementString)) || (!allowedElements.contains(thirdElementString)) || (firstElementString == secondElementString) || (firstElementString == thirdElementString) || (secondElementString == thirdElementString));

    QHash<Dinoz *, qint16> mapDinozToBossDamages = QHash<Dinoz *, qint16>();

    qint16 firstRoundResult, secondRoundResult, thirdRoundResult;
    for(qint8 i = 0; i < this->dinoz.length(); i++){
        qint16 damagesToBoss = 0;

        qint8 firstRoundFactor = DinoparcUtils::getRoundFactorByElements(this->dinoz[i]->getFirstElement(), firstElementString.toInt());


        qint16 yourDinozFirstElementPower = this->dinoz[i]->getFirstElementPower();
        if (firstRoundFactor < 0) {
            firstRoundResult = (firstRoundFactor * bossFirstElementValue + yourDinozFirstElementPower);
        } else if (firstRoundFactor > 0) {
            firstRoundResult = (firstRoundFactor * yourDinozFirstElementPower - bossFirstElementValue);
        }

        qint8 secondRoundFactor = DinoparcUtils::getRoundFactorByElements(this->dinoz[i]->getSecondElement(), secondElementString.toInt());
        qint16 yourDinozSecondElementPower = this->dinoz[i]->getSecondElementPower();
        if (secondRoundFactor < 0) {
            secondRoundResult = (secondRoundFactor * bossSecondElementValue + yourDinozSecondElementPower);
        } else if (secondRoundFactor > 0) {
            secondRoundResult = (secondRoundFactor * yourDinozSecondElementPower - bossSecondElementValue);
        }

        qint8 thirdRoundFactor = DinoparcUtils::getRoundFactorByElements(this->dinoz[i]->getThirdElement(), thirdElementString.toInt());
        qint16 yourDinozThirdElementPower = this->dinoz[i]->getThirdElementPower();
        if (thirdRoundFactor < 0) {
            thirdRoundResult = (thirdRoundFactor * bossThirdElementValue + yourDinozThirdElementPower);
        } else if (thirdRoundFactor > 0) {
            thirdRoundResult = (thirdRoundFactor * yourDinozThirdElementPower - bossThirdElementValue);
        }

        if(firstRoundResult > 0){ damagesToBoss += firstRoundResult; }
        if(secondRoundResult > 0){ damagesToBoss += secondRoundResult; }
        if(thirdRoundResult > 0){ damagesToBoss += thirdRoundResult; }

        mapDinozToBossDamages[this->dinoz[i]] = damagesToBoss;

        Utilities::qPrint(this->dinoz[i]->getName() + " : " + QString::number(damagesToBoss) + " " + tr("dégats par combat.") + " [" + QString::number(firstRoundResult) + "; " + QString::number(secondRoundResult) + "; " + QString::number(thirdRoundResult) + "]\n");
    }

    qint8 maxDamageDinozIndex = 0;
    for(qint8 i = 0; i < this->dinoz.length(); i++){
        if(mapDinozToBossDamages[this->dinoz[i]] > mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]]){
            maxDamageDinozIndex = i;
        }
    }

    qint16 moneyMin = 0;
    qint16 moneyMax = 0;
    qreal bonusMax = mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] / 100 + 1;

    if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 2000) {
        moneyMin = 800;
        moneyMax = 800 + bonusMax;
    } else {
        if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 1500) {
            moneyMin = 775;
            moneyMax = 775 + bonusMax;
        } else {
            if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 1000) {
                moneyMin = 750;
                moneyMax = 750 + bonusMax;
            } else {
                if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 500) {
                    moneyMin = 700;
                    moneyMax = 700 + bonusMax;
                } else {
                    if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 300) {
                        moneyMin = 625;
                        moneyMax = 625 + bonusMax;
                    } else {
                        if (mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]] >= 100) {
                            moneyMin = 525;
                            moneyMax = 525 + bonusMax;
                        } else {
                            moneyMin = 400;
                            moneyMax = 400 + bonusMax;
                        }
                    }
                }
            }
        }
    }

    boolean victory = ((firstRoundResult + secondRoundResult + thirdRoundResult) > 0);

    Utilities::qPrint("\n");
    if(victory){
        Utilities::qPrint(tr("Si vous utilisez le dinoz conseillé, vous gagnez le combat."));
    } else {
        Utilities::qPrint(tr("Aucun de vos dinoz ne permet de remporter le combat..."));
    }

    Utilities::qPrint(tr("Le dinoz idéal pour ce raid est ") + this->dinoz[maxDamageDinozIndex]->getName() + tr(" avec ") + QString::number(mapDinozToBossDamages[this->dinoz[maxDamageDinozIndex]]) + tr(" dégâts par tour."));
    Utilities::qPrint(tr("Vos gains par combat seront situés dans l'intervalle suivant : ") + "[" + QString::number(moneyMin) + ";" + QString::number(moneyMax) + "]");
}

