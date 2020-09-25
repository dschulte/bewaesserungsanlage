#include <Arduino.h>
#include "WateringPlan.h"

WateringPlan::WateringPlan(Soil *_soil, Valve *_valve, double _moistureTarget, int _wateringDurationMS, int _wateringDeadtimeMS)
{
  soil = _soil;
  valve = _valve;
  moistureTarget = _moistureTarget;
  wateringDurationMS = _wateringDurationMS;
  wateringDeadtimeMS = _wateringDeadtimeMS;
}

void WateringPlan::loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - millisLastWatering >= wateringDeadtimeMS)
  {
    if (soil->getMoisture() < moistureTarget)
    {
      Serial.print("Watering ");
      Serial.print(valve->getName());
      Serial.print("... ");
      wateringCount++;
      valve->setState(VALVE_OPEN);
      delay(wateringDurationMS);
      valve->setState(VALVE_CLOSED);
      Serial.println("Done.");

      millisLastWatering = currentMillis;
    }
  }
}

Soil WateringPlan::getSoil()
{
  return *soil;
}

Valve WateringPlan::getValve()
{
  return *valve;
}

unsigned long WateringPlan::getMSsinceLastWatering()
{
  return millis() - millisLastWatering;
}

double WateringPlan::getMoistureTarget()
{
  return moistureTarget;
}