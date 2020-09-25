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

Ein Komplettbausatz ist, wie im Video erwähnt, in Planung. Es gibt allerdings noch keinen Shop. Das wird sich aber in näherer Zukunft ändern. Daher am besten meine Kanäle auf Youtube ( https://www.youtube.com/channel/UCFGTnG45v3j408juOHQ6vHA/ ) und Instagram ( https://www.instagram.com/shocker0815/ ) abonieren. Dort werde ich es bekanntgeben, wenn es soweit ist.

## Entwicklungsumgebung
Als Entwicklungsumgebung wird platform.io genutzt, da die Entwicklung deutlich komfortabler ist als mit der Arduino Entwicklungsumgebung. Außerdem ist das Teilen von Projekten deutlich einfacher, da alle Abhängigkeiten sauber angegeben werden können. Ein guter Einstieg ist folgender Artikel: https://www.heise.de/ct/artikel/Mikrocontroller-bequem-programmieren-mit-PlatformIO-4403209.html

## Konfiguration
Die wichtigsten Konfigurationseinstellungen, wie z.B. die Zugangsdaten fürs Wlan, befinden sich in der Datei SystemConfig.h. Da diese bei jedem unterschiedlich sind, gibt es eine Beispieldatei namens SystemConfig.h.example. Diese muss kopiert und dann angepasst werden.

Mit dem aktuellen Entwicklungsstand müssen die Anzahl der Feuchtigkeitssensoren (SOIL_COUNT und soils), die Anzahl der Ausgänge (VALVE_COUNT und valves) und die Verknüpfungen zur automatischen Bewässerung (WATERINGPLAN_COUNT und wateringPlans) vor dem Aufspielen auf den ESP konfiguriert werden.

Die Feuchtigkeitssensoren müssen wie in der späteren Benutzung angeschlossen sein und dann kalibriert werden, da diese leicht unterschiedliche Werte liefern und auch die ADC Eingänge des ESP unterschiedlich sind. Die Kalibrierung läuft wie folgt:
1. Sensoreingänge (soils) in der SystemConfig.h einrichten und Programm aufspielen
2. Sensor trocken in die Luft halten und Wert "raw" in der Weboberfläche ablesen
3. Sensor bis zur Einkerbung in Wasser stellen und Wert "raw" in der Weboberfläche ablesen
4. Werte beim entsprechenden Sensor in der SystemConfig.h eintragen (erster Wert trocken, zweiter Wert feucht)
5. Programm erneut aufspielen und testen ob die Werte Sinn ergeben (Achtung: "raw s" und der Prozentwert haben einen Tiefpass, sodass sich der Wert bei schnellen Änderungen der Feuchte nur langsam angleicht. Reset hilft beim Testen.)

Auch hier sei noch einmal gesagt, dass gerne nachgefragt werden kann, falls etwas unklar sein sollte.

## Hinweis
Ich übernehme keinerlei Verantwortung für Schäden (z.B. ausgelaufenes Wasser), die im Zusammenhang mit diesem Projekt entstehen. Die Benutzung geschieht auf eigene Gefahr.

## Todo
* Bewässerungszeiten
  * maximale Dauer bei manueller Bewässerung
  * automatische Bewässerung nur zu bestimmten Zeiten
* Parameter über Weboberfläche einstellbar machen
* initiales Aufspielen/OTA beschreiben
* Anpassung des Hostnames beschreiben