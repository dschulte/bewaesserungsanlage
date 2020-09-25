#ifndef VALVE_H
#define VALVE_H

#include <Arduino.h>
#include "../settings.h"

#define VALVE_OPEN true
#define VALVE_CLOSED false

class Valve
{
private:
  uint8_t pin;
  int pinStateEnabled;
  String name;

  bool wasOpenSinceLastHistoricState = false;
  bool valveStateHistory[HISTORY_SLOTS];

public:
  Valve(uint8_t _pin, int _pinStateEnabled, String _name);
  void setState(bool state);
  bool getState();
  void addHistoricState();
  bool getHistoricState(int slot);
  String getName();
};
#endif