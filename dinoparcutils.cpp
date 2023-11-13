#include "dinoparcutils.h"

DinoparcUtils::DinoparcUtils(QObject *parent) : QObject(parent)
{

}

// Portage de la fonction du projet source de Neoparc.
qint8 DinoparcUtils::getRoundFactorByElements(qint8 leftElement, qint8 rightElement) {
  switch (leftElement) {
    case FIRE_ELEMENT:
      switch (rightElement) {
        case FIRE_ELEMENT:
          return 1;
        case EARTH_ELEMENT:
          return 5;
        case WATER_ELEMENT:
          return 3;
        case THUNDER_ELEMENT:
          return -3;
        case WIND_ELEMENT:
          return -5;
      }
    case EARTH_ELEMENT:
      switch (rightElement) {
        case FIRE_ELEMENT:
          return -5;
        case EARTH_ELEMENT:
          return 1;
        case WATER_ELEMENT:
          return 5;
        case THUNDER_ELEMENT:
          return 3;
        case WIND_ELEMENT:
          return -3;
      }
    case WATER_ELEMENT:
      switch (rightElement) {
        case FIRE_ELEMENT:
          return -3;
        case EARTH_ELEMENT:
          return -5;
        case WATER_ELEMENT:
          return 1;
        case THUNDER_ELEMENT:
          return 5;
        case WIND_ELEMENT:
          return 3;
      }
    case THUNDER_ELEMENT:
      switch (rightElement) {
        case FIRE_ELEMENT:
          return 3;
        case EARTH_ELEMENT:
          return -3;
        case WATER_ELEMENT:
          return -5;
        case THUNDER_ELEMENT:
          return 1;
        case WIND_ELEMENT:
          return 5;
      }
    case WIND_ELEMENT:
      switch (rightElement) {
        case FIRE_ELEMENT:
          return 5;
        case EARTH_ELEMENT:
          return 3;
        case WATER_ELEMENT:
          return -3;
        case THUNDER_ELEMENT:
          return -5;
        case WIND_ELEMENT:
          return 1;
      }
    default:
      return 0;
  }
}
