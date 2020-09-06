# Bewässerungsanlage

## Mein Video
**Mein Video zur Bewässerungsanlage: https://youtu.be/8PhPUCQdjGw**

[![Gartenbewässerung fürs Kliemannsland](https://img.youtube.com/vi/8PhPUCQdjGw/0.jpg)](https://youtu.be/8PhPUCQdjGw)

## Einleitung
Dies ist die Dokumentation, die Fertigungsdateien und der Programmcode zur Bewässerungsanlage, die ich für den Garten im Kliemannsland entwickelt habe. Da im Video auf dem Kliemannslandkanal recht wenig von der Technik zu sehen ist, habe ich ein weiteres Video gedreht, in dem ich im Detail auf die Technik und die Hintergründe eingehe. Seit dem ersten Dreh hat sich außerdem noch viel verändert, was ich auch in meinem Video zeige. Reinschauen lohnt sich also ;)

Mein Video: https://youtu.be/8PhPUCQdjGw

Falls es noch Fragen gibt, gerne hier oder per Instagram ( https://www.instagram.com/shocker0815/ ) nachfragen.

Kliemannslandvideo: https://www.youtube.com/watch?v=iammw78OCXI

## BoM
Folgende Bauteile wurden für den Prototypen verwendet:
* Feuchtigkeitssensor
* ESP32
* Relais
* TFT
* Gehäuse
* Breadboard
* Jumper Kabel
* Netzteil

Eine Liste mit Links ist in der Beschreibung vom Video: https://youtu.be/8PhPUCQdjGw

Ein Komplettbausatz ist, wie im Video erwähnt in Planung, es gibt allerdings noch keinen Shop. Daher gerne anfragen und es findet sich auf jeden Fall eine Lösung.

## Entwicklungsumgebung
Als Entwicklungsumgebung wird platform.io genutzt, da die Entwicklung deutlich komfortabler ist als mit der Arduino Entwicklungsumgebung. Außerdem ist das Teilen von Projekten deutlich einfacher, da alle Abhängigkeiten sauber angegeben werden können. Ein guter Einstieg ist folgender Artikel: https://www.heise.de/ct/artikel/Mikrocontroller-bequem-programmieren-mit-PlatformIO-4403209.html

## Konfiguration
Die Library "Adafruit ST7735 and ST7789" hat eine Abhängigkeit auf eine SD Karten Library, die allerdings nicht gebraucht wird. Diese ist nicht vorbereitet für ESP32, sodass es einen Fehler gibt. Dieser kann behoben werden, indem die Abhängigkeit entfernt wird. Dazu nach dem ersten Versuch, nachdem die Libraries heruntergeladen wurden, die Abhängigkeit auf die SD Library (SD) aus der Datei ".pio/libdeps/az-delivery-devkit-v4/Adafruit ST7735 and ST7789 Library/library.properties" entfernen.

Die wichtigsten Konfigurationseinstellungen, wie z.B. die Zugangsdaten fürs Wlan, befinden sich in der Datei SystemConfig.h. Da diese bei jedem unterschiedlich sind, gibt es eine Beispieldatei namens SystemConfig.h.example. Diese muss kopiert und dann angepasst werden.

Mit dem aktuellen Entwicklungsstand müssen die Anzahl der Feuchtigkeitssensoren (SOIL_COUNT und soils), die Anzahl der Ausgänge (VALVE_COUNT und valves) und die Verknüpfungen zur automatischen Bewässerung (WATERINGPLAN_COUNT und wateringPlans) vor dem Aufspielen auf den ESP konfiguriert werden.

Auch hier sei noch einmal gesagt, dass gerne nachgefragt werden kann, falls etwas unklar sein sollte.

## Hinweis
Ich übernehme keinerlei Verantwortung für Schäden (z.B. ausgelaufenes Wasser), die im Zusammenhang mit diesem Projekt entstehen. Die Benutzung geschieht auf eigene Gefahr.

## Todo
* Bewässerungszeiten
* Parameter über Weboberfläche einstellbar machen