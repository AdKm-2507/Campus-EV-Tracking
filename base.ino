//As simple as a base code gets, literally sends Coordinates 
//to the ESP32 webserver, refresh the page to update Coordinates
#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPS++.h>

#define RX2 16
#define TX2 17

double lat = 0.0;
double lng = 0.0;

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

const char* ssid = "ssid";
const char* password = "pwd";

WebServer webServer(80);

void coords()
{
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>GPS</title></head><body>";
  html += "<h3>GPS Coordinates</h3>";
  html += "<p>Latitude: " + String(lat, 6) + "</p>";
  html += "<p>Longitude: " + String(lng, 6) + "</p>";
  html += "</body></html>";

  webServer.send(200, "text/html", html);
}

void setup() 
{
  // setup code , run once:
  Serial.begin(115200);

  //GPS
  gpsSerial.begin(9600, SERIAL_8N1 ,RX2, TX2);
  Serial.println("GPS Serial started");

  //WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.print("Connected to IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  webServer.on("/", coords);
  webServer.begin();
  Serial.println("HTTP webServer started");
}

void loop() 
{
  //code here, to run repeatedly:

  while (gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }

  if(gps.location.isUpdated())
  {
    lat = gps.location.lat();
    lng = gps.location.lng();
  }

  webServer.handleClient();
}
#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPS++.h>

#define RX2 16
#define TX2 17

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

double lat = 0.0;
double lng = 0.0;

const char* ssid = "ssid";
const char* password = "pwd";

WebServer webServer(80);

/* JSON endpoint */
void location() {
  String json = "{";
  json += "\"lat\":" + String(lat, 6) + ",";
  json += "\"lng\":" + String(lng, 6);
  json += "}";

  webServer.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // GPS
  gpsSerial.begin(9600, SERIAL_8N1, RX2, TX2);
  Serial.println("GPS started");

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  // HTTP route
  webServer.on("/location", location);
  webServer.begin();
  Serial.println("ESP32 HTTP server started");
}

void loop() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    lat = gps.location.lat();
    lng = gps.location.lng();

    Serial.print("LAT: ");
    Serial.print(lat, 6);
    Serial.print(" LNG: ");
    Serial.println(lng, 6);
  }

  webServer.handleClient();
}
