#include <Arduino.h>
#include "Valve.h"

Valve::Valve(uint8_t _pin, int _pinStateEnabled, String _name)
{
    pin = _pin;
    pinStateEnabled = _pinStateEnabled;
    name = _name;
}

void Valve::setState(bool state)
{
    if (state == VALVE_OPEN)
        wasOpenSinceLastHistoricState = true;

    Serial.print("Set valve: valve=");
    Serial.print(pin);
    Serial.print("; state=");
    Serial.println(state == VALVE_OPEN ? "open" : "close");

    pinMode(pin, OUTPUT);
    digitalWrite(pin, state == VALVE_OPEN ? (pinStateEnabled == HIGH ? HIGH : LOW) : (pinStateEnabled == HIGH ? LOW : HIGH));
}

bool Valve::getState()
{
    return (digitalRead(pin) == (pinStateEnabled == HIGH ? HIGH : LOW) ? VALVE_OPEN : VALVE_CLOSED);
}

void Valve::addHistoricState()
{
    for (int i = 0; i < HISTORY_SLOTS - 1; i++)
        valveStateHistory[i] = valveStateHistory[i + 1];

    valveStateHistory[HISTORY_SLOTS - 1] = wasOpenSinceLastHistoricState;
    wasOpenSinceLastHistoricState = getState() == VALVE_OPEN ? true : false;
}

bool Valve::getHistoricState(int slot)
{
    return valveStateHistory[slot];
}

String Valve::getName()
{
    return name;
}