#ifndef SOIL_H
#define SOIL_H

#include <Arduino.h>
#include "../settings.h"

class Soil
{
private:
  uint8_t pin;
  String name;
  int adcDry;
  int adcWet;

  int adcRaw;
  double adcSmoothed = 0;
  double moistureHistory[HISTORY_SLOTS];

public:
  Soil(uint8_t _pin, String _name, int _adcDry, int _adcWet);
  double getMoisture();

  int getAdcRaw();
  double getAdcSmoothed();
  void addHistoricState();
  double getHistoricState(int slot);
  String getName();
  void readAdc();
};
#endif