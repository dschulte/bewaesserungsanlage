#include <Arduino.h>
#include "Soil.h"

Soil::Soil(uint8_t _pin, String _name, int _adcDry, int _adcWet)
{
    pin = _pin;
    name = _name;
    adcDry = _adcDry;
    adcWet = _adcWet;
}

double Soil::getMoisture()
{
    return map(adcSmoothed, adcDry, adcWet, 0, 10000) / 100.0;
}

int Soil::getAdcRaw()
{
    return adcRaw;
}

double Soil::getAdcSmoothed()
{
    return adcSmoothed;
}

void Soil::addHistoricState()
{
    for (int i = 0; i < HISTORY_SLOTS - 1; i++)
    {
        moistureHistory[i] = moistureHistory[i + 1];
    }
    moistureHistory[HISTORY_SLOTS - 1] = getMoisture();
}

double Soil::getHistoricState(int slot)
{
    return moistureHistory[slot];
}

String Soil::getName()
{
    return name;
}

void Soil::readAdc()
{
    pinMode(pin, INPUT);
    adcRaw = analogRead(pin);

    if (adcSmoothed == 0)
        adcSmoothed = adcRaw;

    double factor = 0.005;
    adcSmoothed = adcSmoothed * (1.0 - factor) + adcRaw * factor;
}