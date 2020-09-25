#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "time.h"
#include <ArduinoOTA.h>

#include "classes/Soil.h"
#include "classes/Valve.h"
#include "classes/WateringPlan.h"

#include "settings.h"
#include "SystemConfig.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PSK;

const char *www_username = HTTP_USERNAME;
const char *www_password = HTTP_PASSWORD;

//____________________
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

//____________________
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS 0
#define TFT_RST 4
#define TFT_DC 2 // A0
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//____________________

WebServer server(80);

void drawGraph()
{
  int id = constrain(server.arg("id").toInt(), 0, SOIL_COUNT - 1);

  int xPixel = HISTORY_SLOTS;
  int yPixel = 200;

  String out = "";
  char temp[500];
  out = out + "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" + (xPixel + 2) + "\" height=\"" + yPixel + "\">\n";
  out = out + "<rect width=\"" + (xPixel + 2) + "\" height=\"" + yPixel + "\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";

  out += "<g stroke=\"blue\">\n";
  for (int x = 1; x < xPixel - 1; x++)
  {
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x,
            valves[id].getHistoricState(x) ? yPixel / 2 : yPixel - 1,
            x + 1,
            valves[id].getHistoricState(x + 1) ? yPixel / 2 : yPixel - 1);

    out += temp;
  }
  out += "</g>\n";

  out += "<g stroke=\"black\">\n";
  for (int x = 1; x < xPixel - 1; x++)
  {
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x,
            (int)map(soils[id].getHistoricState(x), 0, 100, yPixel, 0),
            x + 1,
            (int)map(soils[id].getHistoricState(x + 1), 0, 100, yPixel, 0));

    out += temp;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

const String imgWater = "data:image/png;base64,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";
const String imgStop = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAACXBIWXMAAAsTAAALEwEAmpwYAAAC/klEQVRYw7WXy28TVxSHvzOPBkorVd2RNRLEYBWklgUKQiXiIQhJH4uWh9SKVf8soJBFhERbVSVAAIEUCSQEEi52F+2miwhYBjuOfefew8IzY7v1YzyOz2xmRvfO9zu/c+deHSFDrBfDL4E9jBal6ZJ5MmyQZIBfBK4C/ogCGsDX0yVzO7eA9WJ4AbgG+P6hs3iffJqJbN+8xpXvZRIhWeDh/E98vLgw3K441Dk2lpaIHi0NFSF94OeBnwE/PHWZj+bPgLrRCqDwbnmZaO3mQBHSA/49cB3ww+MX2XX6JIKSJ9Qp7279gn36a18R0g8ezH7LrhNzrVTGCVWqv/2Ofb7SU4R0wL+L4UFw+CwfHj82PjwJp9RW7mJfPkhEfDNdMn+kArrgh+bYefTI9sE71sTm6kNsZa1LhKwXw2PAKhD4hVl2HvkCVAd9p+8i0gzlqD9ew/79DGmJOBoo7AUCb/c+pj7bh6tuZPtYzpj6/CD11/+i1bdTwJ4gBe34AFvbyGxnhle9nfI9NAzT8UHitms2cLVqpsy139OgySItEZ6HsxGqLVGBJvNMA7tZBSWHiOGROuAJag0JN3DxjZoGUqvlh+lwusROWBulz4GNJ4ppYjdrY2WZyQUR1Eap02kJJGpCvT4wQfmPKBkgUvouBcE523YgOWLURrBVH8llzVEREUHVpmPSvwAb4RqNCXjefasI6try2vuAc2izyaRDhNbRrq37LgGYyQtQQLT9EHTu0y6yk8m6z7vkL/gHcIAX65hI1h37UHo4An95hbK5D/wAOElqhG77BdoFV5ifKZsXqTvlQnhJ4QrgK+C22QpfUlRdYL5QNg9IbAcolM114EfACuCJbBu841t14FwC77k+XhVajYhq7IQbzwnf68hcWNhfNqtDd8w/Z9rdkCrYnOXwPUkAW8DCgYq59z93ek08UDE34oVpRbrqlxe+2As+tDUrzbS7I833328Bi8WKuZu7Oe0UMaIJW8BXxYq5M1Z3HIvI056/LFbM02GD3gOGDYfylQPXuQAAAABJRU5ErkJggg==";

void handleRoot()
{
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  char buf[100];

  String message = "";
  message += "<html>\
  <head>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
    <title>Kliemannsland Bew&auml;sserungsanlage</title>\
    <style>\
      body { background-color: #f7b4bc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      th, td { border-bottom: 1px solid #777; padding-top: 1em; padding-bottom: 1em; }\
    </style>\
  </head>\
  <body>\
    <h1>Kliemannsland Bew&auml;sserungsanlage</h1>";

  message = message + "<h2>Ventile</h2><table><tr><th>Name</th><th>AN/AUS</th><th>Status</th></tr>";
  for (int i = 0; i < VALVE_COUNT; i++)
  {
    message = message + "<tr><td>" + valves[i].getName();
    message = message + "</td><td><a href=\"/setValveState?v=" + i + "&e=" + (valves[i].getState() == VALVE_OPEN ? "0" : "1") + "\"><img width='32' height='32' src='" + (valves[i].getState() == VALVE_OPEN ? imgStop : imgWater) + "'></a></td>";
    message = message + "</td><td>" + (valves[i].getState() == VALVE_OPEN ? "offen" : "geschlossen");
    message = message + "</tr>";
  }
  message += "</table>";

  message = message + "<h2>Sensoren</h2><table><tr><th>Name</th><th>Bodenf.</th><th>Bodenf. (raw)</th><th>Bodenf. (raw s)</th></tr>";
  for (int i = 0; i < SOIL_COUNT; i++)
  {
    message = message + "<tr><td>" + soils[i].getName();
    message = message + "</td><td>" + soils[i].getMoisture() + "%";

    message = message + "</td><td>" + soils[i].getAdcRaw() + "</td><td>" + soils[i].getAdcSmoothed() + "</td></tr>";
  }
  message += "</table>";

  message = message + "<h2>Bew&auml;sserungpl&auml;ne</h2><table><tr><th>Bodensensor</th><th>Ventil</th><th>Zielfeuchte</th><th>Bodenf.</th><th>LW</th></tr>";
  for (int i = 0; i < WATERINGPLAN_COUNT; i++)
  {
    message = message + "<tr><td>" + wateringPlans[i].getSoil().getName();
    message = message + "</td><td>" + wateringPlans[i].getValve().getName();
    message = message + "</td><td>" + wateringPlans[i].getMoistureTarget() + "%";
    message = message + "</td><td>" + wateringPlans[i].getSoil().getMoisture() + "%";

    message = message + "</td><td>" + wateringPlans[i].getMSsinceLastWatering() / 1000.0 / 60.0 + "m</td></tr>";
  }
  message += "</table>";

  message += "<h2>Status</h2>";
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  sprintf(buf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  message = message + "<p>Uhrzeit: " + String(buf) + "</p>";

  sprintf(buf, "%02d:%02d:%02d", hr, min % 60, sec % 60);
  message = message + "<p>Uptime: " + String(buf) + "</p>";

  message = message + "<h2>Historie</h2>";
  for (int i = 0; i < SOIL_COUNT; i++)
  {
    message = message + "<h3>" + soils[i].getName();
    message = message + "</h3><p><img src=\"/graph.svg?id=" + i + "\" /></p>";
  }

  // message = message + "<h2>Debug</h2>";
  // message = message + "<ul>";
  // for (int i = 0; i < HISTORY_SLOTS; i++)
  //   message = message + "<li>" + soils[0].getHistoricState(i) + "</li>";
  // message = message + "</ul>";

  message += "</body>\
</html>";
  server.send(200, "text/html", message);
}

void handleSetValveState()
{
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication();
  }

  int valveId = constrain(server.arg("v").toInt(), 0, VALVE_COUNT - 1);
  bool newState = server.arg("e").toInt() == 0 ? VALVE_CLOSED : VALVE_OPEN;

  valves[valveId].setState(newState);

  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

String hostname = "bewaesserung";

void checkWifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    int wifi_retry = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      wifi_retry++;

      if (wifi_retry > 10)
      {
        // TODO sinnvollen Ausweg finden
        Serial.println("\nReboot");
        // ESP.restart();
        return;
      }

      Serial.println("WiFi not connected. Try to reconnect...");

      WiFi.disconnect(true);
      delay(500);
      WiFi.mode(WIFI_STA);
      WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
      if (WiFi.macAddress() == "24:6F:28:B5:05:58")
      {
        hostname = "bewaesserung-garten";
      }
      else if (WiFi.macAddress() == "24:62:AB:CB:07:F0")
      {
        hostname = "bewaesserung-basilikum";
      }
      WiFi.setHostname(hostname.c_str());

      WiFi.begin(ssid, password);
      delay(500);
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(hostname.c_str()))
    {
      Serial.println("MDNS responder started");
    }
  }
}

int getYFor(double percent)
{
  const int lowerLimit = 0;
  const int upperLimit = 100;
  return tft.height() - map(percent * 100.0, lowerLimit * 100, upperLimit * 100, 0, 100);
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup...");

  Serial.println("Startup: pin mode...");
  for (uint8_t i = 0; i < VALVE_COUNT; i++)
  {
    valves[i].setState(VALVE_CLOSED);
  }

  Serial.println("Startup: tft...");
  tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 0);
  tft.print("Init...");

  tft.drawFastHLine(0, getYFor(35), tft.width(), 0x630C);
  tft.drawFastHLine(0, getYFor(40), tft.width(), 0x630C);
  tft.drawFastHLine(0, getYFor(45), tft.width(), 0x630C);
  tft.drawFastHLine(0, getYFor(50), tft.width(), 0x630C);

  Serial.println("Startup: wifi...");
  WiFi.disconnect(true);
  delay(500);
  checkWifi();

  Serial.println("Startup: time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.println("Startup: webserver...");
  server.on("/", handleRoot);
  server.on("/setValveState", handleSetValveState);
  server.on("/graph.svg", drawGraph);

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  Serial.println("Startup: OTA...");
  // ArduinoOTA.setPassword("admin");

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });

  ArduinoOTA.begin();

  Serial.println("Startup: done.");
}

unsigned long millisLastSensorRead = 0;
unsigned long millisLastDisplayUpdate = 0;
unsigned long millisLastSerialLog = 0;
unsigned long millisLastGraphUpdate = 0;
unsigned long millisLastHistoryUpdate = 0;

unsigned long currentGraphPosition = 0;

void loop(void)
{
  unsigned long currentMillis = millis();

  checkWifi();

  server.handleClient();

  if (currentMillis - millisLastSensorRead >= 1000)
  {
    for (uint8_t i = 0; i < SOIL_COUNT; i++)
    {
      soils[i].readAdc();
    }

    millisLastSensorRead = currentMillis;
  }

  if (currentMillis - millisLastDisplayUpdate >= 1000)
  {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("Failed to obtain time");
      return;
    }

    tft.fillRect(0, 0, tft.width(), 70, ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(true);

    tft.setCursor(0, 0);
    tft.print("Uhrzeit: ");
    tft.println(&timeinfo, "%H:%M:%S");

    for (uint8_t i = 0; i < SOIL_COUNT; i++)
    {
      tft.setCursor(0, 10 + i * 10);
      tft.print("S");
      tft.print(i);
      tft.print(": ");
      tft.print(soils[i].getMoisture());
      tft.print("%");
    }

    millisLastDisplayUpdate = currentMillis;
  }

  if (currentMillis - millisLastSerialLog >= 5000)
  {
    for (uint8_t i = 0; i < SOIL_COUNT; i++)
    {
      Serial.print(i);
      Serial.print("; \t");

      Serial.print(soils[i].getAdcRaw());
      Serial.print("; \t");

      Serial.print(soils[i].getAdcSmoothed());
      Serial.print("; \t");

      Serial.print(soils[i].getMoisture());
      Serial.print("; \t");

      float uptime = currentMillis / 1000.0 / 60.0 / 60.0;
      Serial.print(uptime);
      Serial.print("; \t");

      Serial.println();
    }

    millisLastSerialLog = currentMillis;
  }

  if (currentMillis - millisLastGraphUpdate >= 1000 * 60 * 15)
  {
    // alten Spalteninhalt ueberschreiben
    tft.drawFastVLine(currentGraphPosition % tft.width(), tft.height(), -100, currentGraphPosition % 16 ? ST77XX_BLACK : ST77XX_RED);

    // Legende malen
    tft.drawPixel(currentGraphPosition % tft.width(), getYFor(50), 0x630C);
    tft.drawPixel(currentGraphPosition % tft.width(), getYFor(75), 0x630C);
    tft.drawPixel(currentGraphPosition % tft.width(), getYFor(100), 0x630C);

    // Punkt malen
    tft.drawPixel(currentGraphPosition % tft.width(), getYFor(soils[0].getMoisture()), ST77XX_GREEN);

    currentGraphPosition++;

    // blaue Linie als Trenner malen
    tft.drawFastVLine(currentGraphPosition % tft.width(), tft.height(), -100, ST77XX_BLUE);

    millisLastGraphUpdate = currentMillis;
  }

  if (currentMillis - millisLastHistoryUpdate >= 1000 * 60 * 5)
  {
    for (int j = 0; j < SOIL_COUNT; j++)
      soils[j].addHistoricState();

    for (int j = 0; j < VALVE_COUNT; j++)
      valves[j].addHistoricState();

    millisLastHistoryUpdate = currentMillis;
  }

  for (int i = 0; i < WATERINGPLAN_COUNT; i++)
    wateringPlans[i].loop();

  ArduinoOTA.handle();
}