#ifndef WATERINGPLAN_H
#define WATERINGPLAN_H

#include <Arduino.h>

#include "classes/Soil.h"
#include "classes/Valve.h"

class WateringPlan
{
private:
  Soil *soil;
  Valve *valve;
  double moistureTarget;
  int wateringDurationMS;
  int wateringDeadtimeMS;

  unsigned long millisLastWatering = 0;
  int wateringCount = 0;

public:
  WateringPlan(Soil *_soil, Valve *_valve, double _moistureTarget, int _wateringDurationMS, int _wateringDeadtimeMS);
  void loop();
  Soil getSoil();
  Valve getValve();
  unsigned long getMSsinceLastWatering();
  double getMoistureTarget();
};
#endif