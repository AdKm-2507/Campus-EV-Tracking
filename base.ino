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
  // put your setup code here, to run once:
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
  // put your main code here, to run repeatedly:

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
